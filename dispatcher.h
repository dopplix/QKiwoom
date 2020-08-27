#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include "qflux/dispatchercore.h"
#include "utils/qkoa.h"
#include "utils/qmysql.h"
#include "servers/qtvudfserver.h"
#include "servers/bmwsserver.h"
#include "qflux/store/qjsonobjectmutex.h"

class Dispatcher : public DispatcherCore{
    Q_OBJECT

public:
    Dispatcher();
    QKoa* koa = new QKoa;
    QTvUdfServer* udfServer = new QTvUdfServer(1204);
    BmWsServer* bmWsServer = new BmWsServer(1989);
    QThread* udfThread;
    QObject* udfWorker;
    bool isKoaBusy = false;
    QJsonObject getFncObj(QString fncName);
    QJsonObject getTrObj(QString optName);
    void initUdfWorker();
    void initConditions();
    void routeKoaEvents();
    QJsonObject processTr(QJsonObject trObj);
    bool sendCondToMysql(QString condIndex, QString condName, QString assetName, QString event, QString assetCode, QString sign, QString accAmount, QString accSize, QString rate, QString lastTrTime, QString bestAsk, QString bestBid, QString diffPrice, QString intense, QString size, QString price);
    QVariant callKoaFnc(QString fncName, QJsonArray argArr);
    QVariant callKoaTr(QString optName, QJsonArray argArr,QString sRQName);
    QJsonObject requestKoaTr(QString optName, QJsonArray argArr);
    QJsonObject parseAssets(QString& codeStr);
    QJsonObject parseConditions(QString& condStr);
    QJsonObjectMutex currentPriceObjM;
    int trScrNo = 0;

public slots:
    void dispatch(QString actionType, QJsonObject payload) override;

signals:
    void historyReceived(QJsonObject resultObj);
    void trResultReceived(QJsonObject resultObj);
    void appendLog(QJsonObject obj);

};

#endif // DISPATCHER_H
