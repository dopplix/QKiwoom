#ifndef QTVUDFSERVER_H
#define QTVUDFSERVER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "qhttpserver/src/qhttpserver.h"
#include "qhttpserver/src/qhttprequest.h"
#include "qhttpserver/src/qhttpresponse.h"

class QTvUdfServer : public QObject{
    Q_OBJECT

public:
    explicit QTvUdfServer(QObject *parent = nullptr);
    QJsonObject tempKline;
    QObject* worker;
    QThread* thread;

public slots:
    void handleRequest(QHttpRequest* req, QHttpResponse* res);
    void processConfig(QHttpResponse* res, QJsonObject paramObj);
    void processSymbol(QHttpResponse* res, QJsonObject paramObj);
    void processHistory(QHttpResponse* res, QJsonObject paramObj);
    void sendResponse(QHttpResponse* res, QByteArray msg);

signals:
    void kiwoomTrReq(QJsonObject reqObj, QJsonObject* retObj);
    void kiwoomFncReq(QJsonObject reqObj, QJsonObject* retObj);
    void kiwoomObjReq(QJsonObject reqObj, QJsonObject* retObj);
};

#endif // QTVUDFSERVER_H
