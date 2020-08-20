#include "qtvudfserver.h"
#include "utils/qhttputil.h"
#include "global.h"
#include <QtConcurrent>
#include <QThread>

QTvUdfServer::QTvUdfServer(quint16 port, QObject *parent) : QObject(parent){
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
    server->listen(QHostAddress::Any, port);
}

void QTvUdfServer::handleBlockedKiwoomRequest(QJsonObject reqObj, QHttpResponse *res){
    QFuture<QJsonObject> future = QtConcurrent::run([=]{
        QJsonObject* koaRetObj = new QJsonObject();
        emit(requestKiwoomData(reqObj,koaRetObj));
        QJsonObject resultObj = koaRetObj->value("result").toObject();
        delete koaRetObj;
        return resultObj;
    });
    QFutureWatcher<QJsonObject>* futWatcher = new QFutureWatcher<QJsonObject>;
    connect(futWatcher,&QFutureWatcher<QJsonObject>::finished,[=]{
        QJsonObject resultObj = futWatcher->result();
        qDebug()<<"FINAL SEND"<<resultObj;
        dispatcher->appendLog(resultObj);
        res->setHeader("Access-Control-Allow-Origin", "*");
        res->writeHead(200);
        res->end(QJsonDocument(resultObj).toJson());
    });
    connect(futWatcher,&QFutureWatcher<QJsonObject>::finished,futWatcher,&QFutureWatcher<QJsonObject>::deleteLater);
    futWatcher->setFuture(future);
}
void QTvUdfServer::handleRequest(QHttpRequest* req, QHttpResponse* res){
    QString method = req->methodString();
    QString path = req->path();
    QString urlStr = req->url().toString();
    qDebug()<<method<<path<<urlStr;
    QJsonObject paramObj = QHttpUtil::urlToObj(urlStr);
    qDebug()<<"GET PARAM:"<<paramObj;
    if(method=="HTTP_GET"){
        if(path=="/config"){
            handleConfig(res,paramObj);
        }else if(path=="/symbols"){
            handleSymbol(res,paramObj);
        }else if(path=="/history"){
            handleHistory(res,paramObj);
        }
    }
    else if(method=="HTTP_POST"){
        QString reqType;
        if(path=="/tr"){
            reqType = "POST_TR";
        }else if(path=="/fnc"){
            reqType = "POST_FNC";
        }else if(path=="/assets"){
            reqType = "POST_ASSETS";
        }
        else{
            res->writeHead(400);
            res->end("Bad Request [Post Path : /tr or /fnc]");
            return;
        }
        connect(req,&QHttpRequest::data,[=](QByteArray body){
            QJsonObject reqObj = QJsonDocument::fromJson(body).object();
            qDebug()<<"POST BODY"<<reqObj;
            reqObj.insert("reqType",reqType);
            handleBlockedKiwoomRequest(reqObj,res);
        });
    }
}
void QTvUdfServer::handleConfig(QHttpResponse *res, QJsonObject paramObj){
    QJsonObject resObj;
    resObj["supports_group_request"]= false;
    resObj["supports_marks"] = false;
    resObj["supports_search"] = true;
    resObj["supports_timescale_marks"] = false;
    sendResponse(res,QJsonDocument(resObj).toJson());
}
void QTvUdfServer::handleSymbol(QHttpResponse *res, QJsonObject paramObj){
    QString symbol = paramObj.value("symbol").toString();
    QString name = dispatcher->koa->getMasterCodeName(symbol);
    QJsonObject resObj;
    resObj["name"]=name;
    resObj["ticker"]=symbol;
    resObj["description"]=symbol+" "+name;
    resObj["type"]="stock";
    resObj["session"]="1900-1530";
    resObj["exchange"]="KRX";
    resObj["timezone"]="Asia/Seoul";
    resObj["data_status"]="delayed_streaming";
    sendResponse(res,QJsonDocument(resObj).toJson());
}
void QTvUdfServer::handleHistory(QHttpResponse* res, QJsonObject paramObj){
    QString symbol = paramObj.value("symbol").toString();
    QJsonArray tempTArr = tempKline.value("t").toArray();
    if(!tempTArr.isEmpty()){
        qDebug()<<tempTArr.size();
        int reqStart = paramObj.value("from").toString().toInt();
        int reqEnd = paramObj.value("to").toString().toInt();
        int tempStart = tempTArr.begin()->toInt();
        int tempEnd = (tempTArr.end()-1)->toInt();
        qDebug()<<"REQ_START"<<QDateTime::fromTime_t(uint(reqStart));
        qDebug()<<"REQ_END"<<QDateTime::fromTime_t(uint(reqEnd));
        qDebug()<<"TEMP_START"<<QDateTime::fromTime_t(uint(tempStart));
        qDebug()<<"TEMP_END"<<QDateTime::fromTime_t(uint(tempEnd));
        if(reqStart<tempStart){
            QJsonObject nodataObj = QJsonDocument::fromJson("{\"t\":[],\"o\":[],\"h\":[],\"l\":[],\"c\":[],\"v\":[],\"s\":\"no_data\"}").object();
            sendResponse(res,QJsonDocument(nodataObj).toJson());
            return;
        }
        sendResponse(res,QJsonDocument(tempKline).toJson());
        return;
    }
    QJsonObject reqObj;
    reqObj.insert("assetCode",symbol);
    reqObj.insert("reqType","GET_HISTORY");
    handleBlockedKiwoomRequest(reqObj,res);
}
void QTvUdfServer::sendResponse(QHttpResponse *res, QByteArray msg){
    res->setHeader("Access-Control-Allow-Origin", "*");
    res->writeHead(200);
    res->end(msg);
}
