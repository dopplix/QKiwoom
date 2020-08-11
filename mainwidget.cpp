#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QtSql/QSqlQuery>
#include <QTabWidget>
#include <QDateTime>
#include <QUuid>
#include <QThread>
#include <QEventLoop>
#include <QTimer>
#include "mainwidget.h"
#include "utils/qjsonutils.h"
#include "utils/qtimeutil.h"
#include "dispatcher.h"
#include "actionTypes.h"

extern Dispatcher* dispatcher;
extern QJsonObject krMapObj;

MainWidget::MainWidget(QWidget *parent) : ConnectedWidget(parent){
    //Initialize
    QTabWidget* mainTabWidget = new QTabWidget;
    QHBoxLayout* storeLayout = new QHBoxLayout;
    QHBoxLayout* resultLayout = new QHBoxLayout;
    QVBoxLayout* layout = new QVBoxLayout;
    QPushButton* testPush = new QPushButton("Test");
    fncTab = new FncTab;
        dispatcher->connectWidget(fncTab);
    trTab = new TrTab;
        dispatcher->connectWidget(trTab);
    assetTab = new AssetTab;
        dispatcher->connectWidget(assetTab);
    conditionTab = new ConditionTab;
        dispatcher->connectWidget(conditionTab);
    storeTree = new QJsonTreeWidget;
    resultTree = new QJsonTreeWidget;
    logEdit = new QTextEdit;
    //Render
    this->setLayout(layout);
        layout->addLayout(storeLayout);
            storeLayout->addWidget(mainTabWidget);
                mainTabWidget->addTab(fncTab,"Function");
                mainTabWidget->addTab(trTab,"TR");
                mainTabWidget->addTab(assetTab,"Asset");
                mainTabWidget->addTab(conditionTab,"Condition");
            storeLayout->addWidget(storeTree);
        layout->addLayout(resultLayout);
            resultLayout->addWidget(logEdit);
            resultLayout->addWidget(resultTree);
        layout->addWidget(testPush);
    //Style
    storeTree->setMaximumWidth(400);
    resultTree->setMaximumWidth(400);
    logEdit->setMaximumHeight(300);
    resultTree->setMaximumHeight(300);
    this->resize(1920,1200);
    //Connect
    connect(testPush,&QPushButton::clicked,[=]{
        emit(action(ActionTypes::TEST,QJsonObject()));
    });
    connectTabActions();
    initializeKoaEventRouter();
}
MainWidget::~MainWidget(){

}

void MainWidget::initializeKoaEventRouter(){

}
void MainWidget::connectTabActions(){
//    connect(assetTab,&AssetTab::requestAssetData,[=]{
//        QString codeStr = koa->getCodeListByMarket("0");
//        QStringList codeList = codeStr.split(";");
//        codeList.removeOne("");
//        QJsonArray arr;
//        for(QString code : codeList){
//            QJsonObject obj;
//            QString name = koa->getMasterCodeName(code);
//            int nStock = koa->getMasterListedStockCnt(code);
//            QString info = koa->getMasterConstruction(code);
//            QString listedDate = koa->getMasterListedStockDate(code);
//            QString lastPrice = koa->getMasterLastPrice(code);
//            obj.insert("name",name);
//            obj.insert("code",code);
//            obj.insert("listedDate",listedDate);
//            obj.insert("nStock",nStock);
//            obj.insert("lastPrice",lastPrice);
//            obj.insert("info",info);
//            arr.append(obj);
//        }
//        assetTab->initAssets(arr);
//    });
//    connect(conditionTab,&ConditionTab::requestCondition,[=]{
//        koa->getConditionLoad();
//    });
//    connect(conditionTab,&ConditionTab::appendCurrentCondition,[=](QJsonObject currentCondition){
//        QJsonArray currentCondtionArr = store->getValue("currentConditions").toArray();
//        if(currentCondtionArr.contains(currentCondition)){
//            qDebug()<<"Condition Already Registered"<<currentCondition;
//            return;
//        }
//        int key = currentCondition.value("conditionKey").toInt();
//        QString name = currentCondition.value("conditionName").toString();
//        int ok = koa->sendCondition("1300",name,key,1);
//        if(ok!=1){
//            qDebug()<<"Condition Register Failed"<<ok;
//            return;
//        }
//        currentCondtionArr.append(currentCondition);
//        store->setValue("currentConditions",currentCondtionArr);
//    });
//    connect(conditionTab,&ConditionTab::removeCurrentConditionAt,[=](int i){
//        QJsonArray currentCondtionArr = store->getValue("currentConditions").toArray();
//        QJsonObject currentCondition = currentCondtionArr.at(i).toObject();
//        int key = currentCondition.value("conditionKey").toInt();
//        QString name = currentCondition.value("conditionName").toString();
//        koa->sendConditionStop("1300",name,key);
//        currentCondtionArr.takeAt(i);
//        store->setValue("currentConditions",currentCondtionArr);
//    });
}
void MainWidget::onStoreChanged(QJsonObject diffObj){
    qDebug()<<"ONSTORECHANGED";
    QJsonObject state = getState();
    storeTree->clear();
    storeTree->appendJson(state);
    storeTree->expandAll();
    for(int i=0;i<storeTree->columnCount();i++){
        storeTree->resizeColumnToContents(i);
    }
}
