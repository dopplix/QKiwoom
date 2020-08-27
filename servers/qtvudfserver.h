#ifndef QTVUDFSERVER_H
#define QTVUDFSERVER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThreadPool>
#include "qhttpserver/src/qhttpserver.h"
#include "qhttpserver/src/qhttprequest.h"
#include "qhttpserver/src/qhttpresponse.h"

class QTvUdfServer : public QObject{
    Q_OBJECT

public:
    explicit QTvUdfServer(quint16 port, QObject *parent = nullptr);
    ~QTvUdfServer();
    QJsonObject tempKline;
    QObject* worker;
    QThread* thread;
    QThreadPool* threadPool = new QThreadPool;
    QHttpServer *server = new QHttpServer(this);
    void handleBlockedKiwoomRequest(QJsonObject reqObj, QHttpResponse* res);

public slots:
    void handleRequest(QHttpRequest* req, QHttpResponse* res);
    void sendResponse(QHttpResponse* res, QByteArray msg);

signals:
    void requestKiwoomData(QJsonObject reqObj, QJsonObject* retObj);

};

#endif // QTVUDFSERVER_H
