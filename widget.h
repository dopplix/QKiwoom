#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAxWidget>
#include <QAxBase>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QTextEdit>
#include <QFile>
#include "utils/qkoa.h"
#include "utils/qjsonobjectmutex.h"
#include "utils/qmysql.h"
#include "components/qjsontreewidget.h"
#include "tabs/fnctab.h"
#include "tabs/trtab.h"
#include "tabs/assettab.h"
#include "tabs/conditiontab.h"
#include "utils/qtvudfserver.h"

class Widget : public QWidget{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    FncTab* fncTab = new FncTab;
    TrTab* trTab = new TrTab;
    AssetTab* assetTab = new AssetTab;
    ConditionTab* conditionTab = new ConditionTab;
    QJsonTreeWidget* storeTree = new QJsonTreeWidget;
    QJsonTreeWidget* resultTree = new QJsonTreeWidget;
    QTextEdit* logEdit = new QTextEdit;
    QKoa* koa = new QKoa;
    QMysql* mysql = new QMysql;
    QTvUdfServer* udfServer = new QTvUdfServer;
    QJsonObject krMap;
    QJsonObjectMutex* store = new QJsonObjectMutex;
    bool sendCondToMysql(QString condIndex, QString condName, QString assetName, QString event, QString assetCode, QString sign, QString accAmount, QString accSize, QString rate, QString lastTrTime, QString bestAsk, QString bestBid, QString diffPrice, QString intense, QString size, QString price);
    void initializeConditions();
    QJsonObject processTr(QJsonObject obj);

public slots:

signals:
    void historyReceived(QJsonObject obj);
};
#endif // WIDGET_H
