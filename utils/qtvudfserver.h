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
public slots:
    void handleRequest(QHttpRequest* req, QHttpResponse* res);
    void processHistory(QHttpResponse* res, QJsonObject paramsObject);
    void sendResponse(QHttpResponse* res, QByteArray msg);

signals:
    void kiwoomObjReq(QJsonObject reqObj, QJsonObject* retObj);

};

#endif // QTVUDFSERVER_H
