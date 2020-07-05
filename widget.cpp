#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QtSql/QSqlDatabase>
#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent){
    QHBoxLayout* functionLayout = new QHBoxLayout;
    QHBoxLayout* trLayout = new QHBoxLayout;
    QVBoxLayout* fncTrLayout = new QVBoxLayout;
    QHBoxLayout* storeLayout = new QHBoxLayout;
    QVBoxLayout* layout = new QVBoxLayout;
    QPushButton* testPush = new QPushButton("Test");
    QPushButton* reqTrPush = new QPushButton("Request TR");
    QTextEdit* logEdit = new QTextEdit;
    functionLayout->addWidget(functionTree);
    functionLayout->addWidget(functionList);
    functionLayout->addWidget(functionTable);
    trLayout->addWidget(trTree);
    trLayout->addWidget(trList);
    trLayout->addWidget(trTable);
    fncTrLayout->addLayout(functionLayout);
    fncTrLayout->addWidget(testPush);
    fncTrLayout->addLayout(trLayout);
    fncTrLayout->addWidget(reqTrPush);
    storeLayout->addLayout(fncTrLayout);
    storeLayout->addWidget(storeTree);
    layout->addLayout(storeLayout);
    layout->addWidget(logEdit);
    this->setLayout(layout);
    functionTable->setColumnCount(2);
    functionTable->setHorizontalHeaderLabels({"Type","Input"});
    trTable->setColumnCount(1);
    trTable->setHorizontalHeaderLabels({"Input"});
    //Function
    QString fncFileName(":/json/functions.json");
    QFile fncFile(fncFileName);
    fncFile.open(QIODevice::ReadOnly);
    QString fncJsonText=fncFile.readAll();
    fncFile.close();
    fncDocArr = QJsonDocument::fromJson(fncJsonText.toUtf8()).array();
    functionTree->appendJson(fncDocArr);
    int nFncArr = fncDocArr.size();
    QStringList fncNameList;
    for(int i=0;i<nFncArr;i++){
        fncNameList.append(fncDocArr.at(i).toObject().value("functionName").toString());
    }
    functionList->addItems(fncNameList);
    //Tr
    QString trFileName(":/json/tr.json");
    QFile trFile(trFileName);
    trFile.open(QIODevice::ReadOnly);
    QString trJsonText=trFile.readAll();
    trFile.close();
    trDocArr = QJsonDocument::fromJson(trJsonText.toUtf8()).array();
    trTree->appendJson(trDocArr);
    int nTrArr = trDocArr.size();
    QStringList trNameList;
    for(int i=0;i<nTrArr;i++){
        QString optName = trDocArr.at(i).toObject().value("optName").toString();
        QString trDetail = trDocArr.at(i).toObject().value("trDetail").toString();
        trNameList.append(optName+" "+trDetail);
    }
    trList->addItems(trNameList);
    //Signals
    connect(functionList,&QListWidget::currentRowChanged,[=]{
        int i = functionList->currentRow();
        QJsonObject currentObj = fncDocArr.at(i).toObject();
        QString functionName = currentObj.value("functionName").toString();
        QJsonArray paramsArr = currentObj.value("params").toArray();
        QString returnType = currentObj.value("returnType").toString();
        store->setValue("currentFunction",functionName);
        store->setValue("currentParams",paramsArr);
    });
    connect(trList,&QListWidget::currentRowChanged,[=]{
       int i = trList->currentRow();
       QJsonObject currentTr = trDocArr.at(i).toObject();
       store->setValue("currentTr",currentTr);
    });
    store->setChangeCheck(true);
    connect(store,&QJsonObjectMutex::somethingChanged,[=](QJsonObject diffObj){
        QJsonObject storeObj = store->getObject();
        storeTree->clear();
        storeTree->appendJson(storeObj);
        storeTree->expandAll();
        for(int i=0;i<storeTree->columnCount();i++){
            storeTree->resizeColumnToContents(i);
        }
    });
    connect(store,&QJsonObjectMutex::somethingChanged,[=](QJsonObject diffObj){
        QJsonArray paramsArr = store->getValue("currentParams").toArray();
        int nArr = paramsArr.size();
        functionTable->setRowCount(nArr);
        QStringList paramNameList;
        QStringList paramTypeList;
        for(int i=0;i<nArr;i++){
            QJsonObject paramObj = paramsArr.at(i).toObject();
            QString paramName = paramObj.value("paramName").toString();
            QString paramType = paramObj.value("paramType").toString();
            paramNameList.append(paramName);
            QTableWidgetItem* typeItem = new QTableWidgetItem(paramType);
            QTableWidgetItem* inputItem = new QTableWidgetItem;
            functionTable->setItem(i,0,typeItem);
            functionTable->setItem(i,1,inputItem);
            typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        }
        functionTable->setVerticalHeaderLabels(paramNameList);
    });
    connect(store,&QJsonObjectMutex::somethingChanged,[=](QJsonObject diffObj){
        QJsonObject obj = store->getValue("currentTr").toObject();
        QJsonArray inputArr = obj.value("inputArr").toArray();
        int nArr = inputArr.size();
        trTable->setRowCount(nArr);
        QStringList inputList;
        for(int i=0;i<nArr;i++){
            inputList.append(inputArr.at(i).toString());
            QTableWidgetItem* inputItem = new QTableWidgetItem;
            trTable->setItem(i,0,inputItem);
        }
        trTable->setVerticalHeaderLabels(inputList);
    });
    connect(testPush,&QPushButton::clicked,[=]{
        QJsonObject obj = store->getObject();
        QString currentFunctionName = obj.value("currentFunction").toString();
        QJsonArray currentParams = obj.value("currentParams").toArray();
        int nParam = currentParams.size();
        QVariantList paramsList;
        QStringList paramTypes;
        for(int i=0;i<nParam;i++){
            QString type = currentParams.at(i).toObject().value("paramType").toString();
            QString arg = functionTable->item(i,1)->text();
            if(type=="int"){
                paramsList.append(arg.toInt());
            }else if(type=="QString"){
                paramsList.append(arg);
            }
            paramTypes.append(type);
        }
        qDebug()<<paramsList;
        QString paramHolder = "("+paramTypes.join(", ")+")";
        QString functionFullName = currentFunctionName+paramHolder;
        QByteArray bytes = functionFullName.toLocal8Bit();
        char* buffer = bytes.data();
        QVariant result = koa->ax->dynamicCall(buffer,paramsList);
        logEdit->append(result.toString());
    });
    connect(reqTrPush,&QPushButton::clicked,[=]{
        QJsonObject obj = store->getValue("currentTr").toObject();
        QJsonArray inputArr = obj.value("inputArr").toArray();
        QString optName = obj.value("optName").toString();
        int nArr = inputArr.size();
        for(int i=0;i<nArr;i++){
            QString inputName = inputArr.at(i).toString();
            QString inputValue = trTable->item(i,0)->text();
            koa->setInputValue(inputName,inputValue);
        }
        koa->commRqData("Test",optName,0,"1111");
    });
    connect(koa,&QKoa::onMessageReceived,[=](QJsonObject obj){
        logEdit->append(QJsonDocument(obj).toJson(QJsonDocument::Indented));
        QString event = obj.value("event").toString();
        if(event=="onReceiveTrData"){
           QJsonObject trObj = store->getValue("currentTr").toObject();
           QString outputSingleName = trObj.value("outputSingleName").toString();
           QString optName = trObj.value("optName").toString();
           QJsonArray outputSingleArr = trObj.value("outputSingleArr").toArray();
           int nArr = outputSingleArr.size();
           QJsonObject resultObj;
           for(int i=0;i<nArr;i++){
               QString recordName = outputSingleArr.at(i).toString();
               QString ret = koa->getCommData(optName,outputSingleName,0,recordName);
               resultObj.insert(recordName,ret.replace(" ",""));
           }
           logEdit->append(QJsonDocument(resultObj).toJson(QJsonDocument::Indented));
        }
    });
    for(int i=0;i<functionTree->columnCount();i++){
        functionTree->resizeColumnToContents(i);
    }
    functionTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    trTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    this->resize(1920,1200);
}
Widget::~Widget(){

}
void Widget::test(){
    qDebug()<<"TEST";
}
