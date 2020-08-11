#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QAxWidget>
#include <QAxBase>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QTextEdit>
#include <QFile>
#include "utils/qkoa.h"
#include "utils/qmysql.h"
#include "components/qjsontreewidget.h"
#include "tabs/fnctab.h"
#include "tabs/trtab.h"
#include "tabs/assettab.h"
#include "tabs/conditiontab.h"
#include "utils/qtvudfserver.h"
#include "qflux/connectedwidget.h"

class MainWidget : public ConnectedWidget{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    FncTab* fncTab;
    TrTab* trTab;
    AssetTab* assetTab;
    ConditionTab* conditionTab;
    QJsonTreeWidget* storeTree;
    QJsonTreeWidget* resultTree;
    QTextEdit* logEdit;
    bool sendCondToMysql(QString condIndex, QString condName, QString assetName, QString event, QString assetCode, QString sign, QString accAmount, QString accSize, QString rate, QString lastTrTime, QString bestAsk, QString bestBid, QString diffPrice, QString intense, QString size, QString price);
    void initializeKoaEventRouter();
    void connectTabActions();

public slots:
    void onStoreChanged(QJsonObject diffObj) override;

signals:
    void historyReceived(QJsonObject obj);
};
#endif // MAINWIDGET_H
