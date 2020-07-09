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
#include <QTextCodec>
#include "widget.h"
#include "utils/qjsonutils.h"

Widget::Widget(QWidget *parent) : QWidget(parent){
    QTabWidget* mainTabWidget = new QTabWidget;
    QHBoxLayout* storeLayout = new QHBoxLayout;
    QHBoxLayout* resultLayout = new QHBoxLayout;
    QVBoxLayout* layout = new QVBoxLayout;
    QPushButton* testPush = new QPushButton("Test");
    mainTabWidget->addTab(fncTab,"Function");
    mainTabWidget->addTab(trTab,"TR");
    mainTabWidget->addTab(assetTab,"Asset");
    mainTabWidget->addTab(conditionTab,"Condition");
    storeLayout->addWidget(mainTabWidget);
    storeLayout->addWidget(storeTree);
    resultLayout->addWidget(logEdit);
    resultLayout->addWidget(resultTree);
    layout->addLayout(storeLayout);
    layout->addLayout(resultLayout);
    layout->addWidget(testPush);
    krMap = QJsonUtils::readJsonObjFromFile(":/doc/json/krmap.json");
    connect(testPush,&QPushButton::clicked,[=]{
//        koa->setInputValue(krMap.value("assetCode").toString(),"000060");
//        QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
//        koa->commRqData(uuid,"OPT10003",0,"1111");
        qDebug()<<sendCondToMysql("QString index","QString condName","QString assetName","QString event","QString assetCode","QString sign","QString accAmount","QString accSize","QString rate","QString lastTrTime","QString bestAsk","QString bestBid","QString diffPrice","QString intense","QString size","QString price");
    });
    this->setLayout(layout);
    store->setChangeCheck(true);
    connect(fncTab,&FncTab::updateCurrentFnc,[=](QJsonObject currentFnc){
        store->setValue("currentFnc",currentFnc);
    });
    connect(fncTab,&FncTab::callFnc,[=](QStringList paramList){
        QJsonObject obj = store->getObject();
        QJsonObject currentFnc = obj.value("currentFnc").toObject();
        QString currentFunctionName = currentFnc.value("functionName").toString();
        QJsonArray currentParams = currentFnc.value("params").toArray();
        int nParam = currentParams.size();
        QVariantList paramsVariantList;
        QStringList paramTypes;
        for(int i=0;i<nParam;i++){
            QString type = currentParams.at(i).toObject().value("paramType").toString();
            QString paramStr = paramList.at(i);
            if(type=="int"){
                paramsVariantList.append(paramStr.toInt());
            }else if(type=="QString"){
                paramsVariantList.append(paramStr);
            }
            paramTypes.append(type);
        }
        QString paramHolder = "("+paramTypes.join(", ")+")";
        QString functionFullName = currentFunctionName+paramHolder;
        QByteArray fullNameBytes = functionFullName.toLocal8Bit();
        char* buffer = fullNameBytes.data();
        QVariant result = koa->ax->dynamicCall(buffer,paramsVariantList);
        logEdit->append(result.toString());
    });
    connect(trTab,&TrTab::updateCurrentTr,[=](QJsonObject currentTr){
        store->setValue("currentTr",currentTr);
    });
    connect(trTab,&TrTab::requestTr,[=](QStringList inputList){
        QJsonObject obj = store->getValue("currentTr").toObject();
        QJsonArray inputArr = obj.value("inputArr").toArray();
        QString optName = obj.value("optName").toString();
        int nArr = inputArr.size();
        for(int i=0;i<nArr;i++){
            QString inputName = inputArr.at(i).toString();
            QString inputValue = inputList.at(i);
            koa->setInputValue(inputName,inputValue);
        }
        koa->commRqData("TrTab",optName,0,"1100");
    });
    connect(assetTab,&AssetTab::requestAssetData,[=]{
        QString codeStr = koa->getCodeListByMarket("0");
        QStringList codeList = codeStr.split(";");
        codeList.removeOne("");
        QJsonArray arr;
        for(QString code : codeList){
            QJsonObject obj;
            QString name = koa->getMasterCodeName(code);
            int nStock = koa->getMasterListedStockCnt(code);
            QString info = koa->getMasterConstruction(code);
            QString listedDate = koa->getMasterListedStockDate(code);
            QString lastPrice = koa->getMasterLastPrice(code);
            obj.insert("name",name);
            obj.insert("code",code);
            obj.insert("listedDate",listedDate);
            obj.insert("nStock",nStock);
            obj.insert("lastPrice",lastPrice);
            obj.insert("info",info);
            arr.append(obj);
        }
        assetTab->initAssets(arr);
    });
    connect(conditionTab,&ConditionTab::requestCondition,[=]{
        koa->getConditionLoad();
    });
    connect(conditionTab,&ConditionTab::appendCurrentCondition,[=](QJsonObject currentCondition){
        QJsonArray currentCondtionArr = store->getValue("currentConditions").toArray();
        if(currentCondtionArr.contains(currentCondition)){
            qDebug()<<"Condition Already Registered"<<currentCondition;
            return;
        }
        int key = currentCondition.value("conditionKey").toInt();
        QString name = currentCondition.value("conditionName").toString();
        int ok = koa->sendCondition("1300",name,key,1);
        if(ok!=1){
            qDebug()<<"Condition Register Failed"<<ok;
            return;
        }
        currentCondtionArr.append(currentCondition);
        store->setValue("currentConditions",currentCondtionArr);
    });
    connect(conditionTab,&ConditionTab::removeCurrentConditionAt,[=](int i){
        QJsonArray currentCondtionArr = store->getValue("currentConditions").toArray();
        QJsonObject currentCondition = currentCondtionArr.at(i).toObject();
        int key = currentCondition.value("conditionKey").toInt();
        QString name = currentCondition.value("conditionName").toString();
        koa->sendConditionStop("1300",name,key);
        currentCondtionArr.takeAt(i);
        store->setValue("currentConditions",currentCondtionArr);
    });
    connect(koa,&QKoa::onMessageReceived,[=](QJsonObject obj){
        //logEdit->append(QJsonDocument(obj).toJson(QJsonDocument::Indented));
        QString event = obj.value("event").toString();
        QString caller = obj.value("sScrNo").toString();
        if(event=="onReceiveTrData"){
            if(caller=="1100"){
                QJsonObject trObj = store->getValue("currentTr").toObject();
                QJsonObject finalResultObj = processTr(trObj);
                resultTree->clear();
                resultTree->appendJson(finalResultObj);
            }else if(caller=="1200"){
                QString uuid = obj.value("sRQName").toString();
                QJsonObject trOpt10003 = trTab->trDocArr.at(2).toObject();
                QJsonObject finalResultObj = processTr(trOpt10003);
                QJsonArray multiArr = finalResultObj.value("multi").toArray();
                QJsonObject targetObj = multiArr.at(0).toObject();
                QJsonObject queryObj = store->getValue(uuid).toObject();
                QString condIndex = queryObj.value("condIndex").toString();
                QString condName = queryObj.value("condName").toString();
                QString eventName = queryObj.value("event").toString();
                QString assetCode = queryObj.value("assetCode").toString();
                QString assetName = queryObj.value("assetName").toString();
                QString sign = targetObj.value("sign").toString();
                QString accAmount = targetObj.value(krMap.value("accAmount").toString()).toString();
                QString accSize = targetObj.value(krMap.value("accSize").toString()).toString();
                QString rate = targetObj.value(krMap.value("rate").toString()).toString();
                QString lastTrTime = targetObj.value(krMap.value("lastTrTime").toString()).toString();
                QString bestAsk = targetObj.value(krMap.value("bestAsk").toString()).toString();
                QString bestBid = targetObj.value(krMap.value("bestBid").toString()).toString();
                QString diffPrice = targetObj.value(krMap.value("diffPrice").toString()).toString();
                QString intense = targetObj.value(krMap.value("intense").toString()).toString();
                QString size = targetObj.value(krMap.value("size").toString()).toString();
                QString price = targetObj.value(krMap.value("price").toString()).toString();
                qDebug()<<"Query"<<sendCondToMysql(condIndex,condName,assetName,eventName,assetCode,sign,accAmount,accSize,rate,lastTrTime,bestAsk,bestBid,diffPrice,intense,size,price);
                store->removeValue(uuid);
            }
        }else if(event=="onReceiveConditionVer"){
            initializeConditions();
        }else if(event=="onReceiveRealCondition"){
            qDebug()<<obj;
            QString assetCode = obj.value("sTrCode").toString();
            QJsonObject queryObj;
            QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
            queryObj.insert("condIndex",obj.value("strConditionIndex").toString());
            queryObj.insert("condName",obj.value("strConditionName").toString());
            queryObj.insert("event",obj.value("strType").toString());
            queryObj.insert("assetCode",assetCode);
            queryObj.insert("assetName",koa->getMasterCodeName(assetCode));
            store->setValue(uuid,queryObj);
            koa->setInputValue(krMap.value("assetCode").toString(),assetCode);
            koa->commRqData(uuid,"OPT10003",0,"1200");
        }
    });
    this->resize(1920,1200);
    connect(store,&QJsonObjectMutex::somethingChanged,trTab,&TrTab::onStoreChanged);
    connect(store,&QJsonObjectMutex::somethingChanged,fncTab,&FncTab::onStoreChanged);
    connect(store,&QJsonObjectMutex::somethingChanged,conditionTab,&ConditionTab::onStoreChanged);
    connect(store,&QJsonObjectMutex::somethingChanged,[=](QJsonObject diffObj){
        QJsonObject storeObj = store->getObject();
        storeTree->clear();
        storeTree->appendJson(storeObj);
        storeTree->expandAll();
        for(int i=0;i<storeTree->columnCount();i++){
            storeTree->resizeColumnToContents(i);
        }
    });
    storeTree->setMaximumWidth(400);
    resultTree->setMaximumWidth(400);
    logEdit->setMaximumHeight(300);
    resultTree->setMaximumHeight(300);
}
Widget::~Widget(){

}
bool Widget::sendCondToMysql(QString condIndex, QString condName, QString assetName, QString event, QString assetCode, QString sign, QString accAmount, QString accSize, QString rate, QString lastTrTime, QString bestAsk, QString bestBid, QString diffPrice, QString intense, QString size, QString price){
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QSqlQuery query;

    query.prepare("INSERT INTO tb_cond (eventTime,condIndex,condName,assetName,event,assetCode,sign,accAmount,accSize,rate,lastTrTime,bestAsk,bestBid,diffPrice,intense,size,price)"
                  "VALUES (:eventTime,:condIndex,:condName,:assetName,:event,:assetCode,:sign,:accAmount,:accSize,:rate,:lastTrTime,:bestAsk,:bestBid,:diffPrice,:intense,:size,:price)");
    query.bindValue(":eventTime",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":condIndex",condIndex);
    query.bindValue(":condName",condName);
    query.bindValue(":assetName",assetName);
    query.bindValue(":event",event);
    query.bindValue(":assetCode",assetCode);
    query.bindValue(":sign",sign);
    query.bindValue(":accAmount",accAmount);
    query.bindValue(":accSize",accSize);
    query.bindValue(":rate",rate);
    query.bindValue(":lastTrTime",lastTrTime);
    query.bindValue(":bestAsk",bestAsk);
    query.bindValue(":bestBid",bestBid);
    query.bindValue(":diffPrice",diffPrice);
    query.bindValue(":intense",intense);
    query.bindValue(":size",size);
    query.bindValue(":price",price);
    return query.exec();
}
void Widget::initializeConditions(){
    QString conditionStr = koa->getConditionNameList();
    qDebug()<<conditionStr;
    QStringList conditionList = conditionStr.split(";");
    conditionList.removeOne("");
    QJsonArray conditionArr;
    for(QString condition : conditionList){
        QJsonObject conditionObj;
        QStringList conditionSplit = condition.split("^");
        conditionObj.insert("conditionKey",conditionSplit.at(0).toInt());
        conditionObj.insert("conditionName",conditionSplit.at(1));
        conditionArr.append(conditionObj);
    }
    conditionTab->initConditions(conditionArr);
}
QJsonObject Widget::processTr(QJsonObject trObj){
    QString optName = trObj.value("optName").toString();
    QJsonObject finalResultObj;
    //Single
    QString outputSingleName = trObj.value("outputSingleName").toString();
    if(!outputSingleName.isEmpty()){
        QJsonArray outputSingleArr = trObj.value("outputSingleArr").toArray();
        int nSingleArr = outputSingleArr.size();
        QJsonObject resultSingleObj;
        for(int i=0;i<nSingleArr;i++){
            QString recordName = outputSingleArr.at(i).toString();
            QString ret = koa->getCommData(optName,outputSingleName,0,recordName);
            resultSingleObj.insert(recordName,ret.replace(" ",""));
        }
        logEdit->append("Single Result");
        logEdit->append(QJsonDocument(resultSingleObj).toJson(QJsonDocument::Indented));
        finalResultObj.insert("single",resultSingleObj);
    }
    //Multi
    QString outputMultiName = trObj.value("outputMultiName").toString();
    if(!outputMultiName.isEmpty()){
        QJsonArray outputMultiArr = trObj.value("outputMultiArr").toArray();
        int nMultiArr = outputMultiArr.size();
        int nIdx = koa->getRepeatCnt(optName,outputMultiName);
        QJsonArray resultArr;
        for(int i=0;i<nIdx;i++){
            QJsonObject resultMultiObj;
            for(int j=0;j<nMultiArr;j++){
                QString recordName = outputMultiArr.at(j).toString();
                QString ret = koa->getCommData(optName,outputMultiName,i,recordName);
                resultMultiObj.insert(recordName,ret.replace(" ",""));
            }
            resultArr.append(resultMultiObj);
        }
        logEdit->append("Multi Result");
        logEdit->append(QJsonDocument(resultArr).toJson(QJsonDocument::Indented));
        finalResultObj.insert("multi",resultArr);
    }
    return finalResultObj;
}
