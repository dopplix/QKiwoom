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
    explicit QTvUdfServer(quint16 port, QObject *parent = nullptr);
    QJsonObject tempKline;
    QObject* worker;
    QThread* thread;
    void handleBlockedKiwoomRequest(QJsonObject reqObj, QHttpResponse* res);

public slots:
    void handleRequest(QHttpRequest* req, QHttpResponse* res);
    void handleConfig(QHttpResponse* res, QJsonObject paramObj);
    void handleSymbol(QHttpResponse* res, QJsonObject paramObj);
    void handleHistory(QHttpResponse* res, QJsonObject paramObj);
    void sendResponse(QHttpResponse* res, QByteArray msg);

signals:
    void requestKiwoomData(QJsonObject reqObj, QJsonObject* retObj);

};

#endif // QTVUDFSERVER_H
