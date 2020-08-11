#include "qtvudfserver.h"
#include "utils/qhttputil.h"
#include <QtConcurrent>
#include <QThread>

QTvUdfServer::QTvUdfServer(QObject *parent) : QObject(parent){
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
    server->listen(QHostAddress::Any,1204);
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
    qDebug()<<"PARAM:"<<paramObj;
    if(method=="HTTP_GET"){
        if(path=="/config"){
            processConfig(res,paramObj);
        }else if(path=="/symbols"){
            processSymbol(res,paramObj);
        }else if(path=="/history"){
            processHistory(res,paramObj);
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
            reqObj.insert("reqType",reqType);
            handleBlockedKiwoomRequest(reqObj,res);
        });
    }
}
void QTvUdfServer::processConfig(QHttpResponse *res, QJsonObject paramObj){
    QJsonObject resObj;
    resObj["supports_group_request"]= false;
    resObj["supports_marks"] = false;
    resObj["supports_search"] = true;
    resObj["supports_timescale_marks"] = false;
    sendResponse(res,QJsonDocument(resObj).toJson());
}
void QTvUdfServer::processSymbol(QHttpResponse *res, QJsonObject paramObj){
    QJsonObject resObj;
    resObj["name"]="NAME_APPLE";
    resObj["ticker"]="TICKER_MYAPPLE";
    resObj["description"]="DESC_AMYDESC";
    resObj["type"]="stock";
    resObj["session"]="1900-1530";
    resObj["exchange"]="KRX";
    resObj["timezone"]="Asia/Seoul";
    resObj["data_status"]="delayed_streaming";
    sendResponse(res,QJsonDocument(resObj).toJson());
}
void QTvUdfServer::processHistory(QHttpResponse* res, QJsonObject paramObj){
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
    reqObj.insert("reqType","GET_HISTORY");
    handleBlockedKiwoomRequest(reqObj,res);
}
void QTvUdfServer::sendResponse(QHttpResponse *res, QByteArray msg){
    res->setHeader("Access-Control-Allow-Origin", "*");
    res->writeHead(200);
    res->end(msg);
}
