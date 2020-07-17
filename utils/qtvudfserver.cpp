#include "qtvudfserver.h"
#include <QtConcurrent>
#include <QThread>

QTvUdfServer::QTvUdfServer(QObject *parent) : QObject(parent){
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
    server->listen(QHostAddress::Any,1989);
}
void QTvUdfServer::handleRequest(QHttpRequest* req, QHttpResponse* res){
    QString method = req->methodString();
    QString path = req->path();
    QString urlStr = req->url().toString();
    qDebug()<<method<<path;
    QJsonObject paramObj;
    if(urlStr.split("?").length()>1){
        QString paramStr = urlStr.split("?").at(1);
        QStringList paramStrList = paramStr.split("&");
        for(QString param : paramStrList){
            QString key = param.split("=").at(0);
            QString value = param.split("=").at(1);
            paramObj.insert(key,value);
        }
    }
    res->setHeader("Access-Control-Allow-Origin", "*");
    res->writeHead(200);
    if(path=="/config"){
        QJsonObject resObj;
        resObj["supports_group_request"]= false;
        resObj["supports_marks"] = false;
        resObj["supports_search"] = true;
        resObj["supports_timescale_marks"] = false;
        res->end(QJsonDocument(resObj).toJson());
    }else if(path=="/symbols"){
        QJsonObject resObj;
        resObj["name"]="NAME_APPLE";
        resObj["ticker"]="TICKER_MYAPPLE";
        resObj["description"]="DESC_AMYDESC";
        resObj["type"]="stock";
        resObj["session"]="1900-1530";
        resObj["exchange"]="KRX";
        resObj["timezone"]="Asia/Seoul";
        resObj["data_status"]="delayed_streaming";
        res->end(QJsonDocument(resObj).toJson());
    }else if(path=="/history"){
        qDebug()<<paramObj;
        qDebug()<<"0 Before Function"<<QThread::currentThread();
        QFuture<QJsonObject> future = QtConcurrent::run([=]{
            qDebug()<<"1 CALL"<<QThread::currentThread();
            QJsonObject reqObj;
            QJsonObject* koaRetObj = new QJsonObject();
            reqObj["REQ"]="REQ VALUE";
            koaRetObj->insert("REQ_IN","REQ VALUE");
            emit(kiwoomObjReq(reqObj,koaRetObj));
            qDebug()<<"4"<<*koaRetObj;
            return *koaRetObj;
        });
        QFutureWatcher<QJsonObject>* futWatcher = new QFutureWatcher<QJsonObject>;
        connect(futWatcher,&QFutureWatcher<QJsonObject>::finished,[=]{
            qDebug()<<"5 FUTURE WATCHER"<<QThread::currentThread();
            QJsonObject resObj = futWatcher->result();
            res->end(QJsonDocument(resObj).toJson());
        });
        connect(futWatcher,&QFutureWatcher<QJsonObject>::finished,futWatcher,&QFutureWatcher<QJsonObject>::deleteLater);
        futWatcher->setFuture(future);
        qDebug()<<"7 After Function"<<QThread::currentThread();
    }
}
