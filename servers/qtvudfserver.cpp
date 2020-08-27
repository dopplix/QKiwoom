#include "qtvudfserver.h"
#include "utils/qhttputil.h"
#include "global.h"
#include <QtConcurrent>

QTvUdfServer::QTvUdfServer(quint16 port, QObject *parent) : QObject(parent){
    qDebug()<<"QTvUdfServer::QTvUdfServer(quint16 port, QObject *parent) : QObject(parent)";
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
    server->listen(QHostAddress::Any, port);
    threadPool->setMaxThreadCount(1);
}
QTvUdfServer::~QTvUdfServer(){
    qDebug()<<"QTvUdfServer::~QTvUdfServer()";
    delete server;
}
void QTvUdfServer::handleBlockedKiwoomRequest(QJsonObject reqObj, QHttpResponse *res){
    QFuture<QJsonObject> future = QtConcurrent::run(threadPool,[=]{
        qDebug()<<"# IN TO REQUEST KIWOOM"<<QThread::currentThread()<<threadPool->maxThreadCount()<<threadPool->activeThreadCount();
        QJsonObject* koaRetObj = new QJsonObject();
        emit(requestKiwoomData(reqObj,koaRetObj));
        qDebug()<<"# OUT FROM REQUEST KIWOOM"<<QThread::currentThread()<<threadPool->maxThreadCount()<<threadPool->activeThreadCount();
        QJsonObject resultObj = koaRetObj->value("result").toObject();
        delete koaRetObj;
        return resultObj;
    });
    QFutureWatcher<QJsonObject>* futWatcher = new QFutureWatcher<QJsonObject>;
    connect(futWatcher,&QFutureWatcher<QJsonObject>::finished,[=]{
        QJsonObject resultObj = futWatcher->result();
        qDebug()<<"FINAL SEND"<<resultObj.keys();
        if(resultObj.keys().contains("multi")){
            qDebug()<<"MULTI 0"<<resultObj.value("multi").toArray().at(0)<<"\n";
        }else{
            qDebug()<<"NOT MULTI"<<resultObj<<"\n";
        }
        //dispatcher->appendLog(resultObj);
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
    qDebug()<<"[HTTP REQUEST RECEIVED]"<<method<<path<<urlStr;
    if(method=="HTTP_POST"){
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
            res->end("Bad Request [Supported Post Path : {/tr, /fnc, /asset} ]");
            return;
        }
        connect(req,&QHttpRequest::data,[=](QByteArray body){
            QJsonObject reqObj = QJsonDocument::fromJson(body).object();
            qDebug()<<"POST BODY"<<reqObj;
            reqObj.insert("reqType",reqType);
            handleBlockedKiwoomRequest(reqObj,res);
        });
    }else if(method=="HTTP_GET"){
        QJsonObject paramObj = QHttpUtil::urlToObj(urlStr);
        qDebug()<<"[HTTP GET PARAM]"<<paramObj;
    }
}
void QTvUdfServer::sendResponse(QHttpResponse *res, QByteArray msg){
    res->setHeader("Access-Control-Allow-Origin", "*");
    res->writeHead(200);
    res->end(msg);
}
