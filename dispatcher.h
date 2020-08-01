#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include "qflux/dispatchercore.h"
#include "utils/qkoa.h"
#include "utils/qmysql.h"
#include "utils/qtvudfserver.h"

class Dispatcher : public DispatcherCore{
    Q_OBJECT

public:
    Dispatcher();
    QKoa* koa = new QKoa;
    QMysql* mysql = new QMysql;
    QTvUdfServer* udfServer = new QTvUdfServer;
    QJsonObject getFncObj(QString fncName);
    QJsonObject getTrObj(QString optName);
    void initUdfWorker();
    void initConditions();
    void initKoaEventRouter();
    QJsonObject processTr(QJsonObject trObj);
    bool sendCondToMysql(QString condIndex, QString condName, QString assetName, QString event, QString assetCode, QString sign, QString accAmount, QString accSize, QString rate, QString lastTrTime, QString bestAsk, QString bestBid, QString diffPrice, QString intense, QString size, QString price);
    QVariant callKoaFnc(QString fncName, QJsonArray argsArr);
    QVariant callKoaTr(QString optName, QJsonArray argsArr);
    QJsonObject requestKoaTr(QString optName, QJsonArray argsArr);

public slots:
    void dispatch(QString actionType, QJsonObject payload) override;

signals:
    void historyReceived(QJsonObject resultObj);
    void trResultReceived(QJsonObject resultObj);

};

#endif // DISPATCHER_H
