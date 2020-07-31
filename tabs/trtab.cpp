#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include "trtab.h"
#include "../utils/qjsonutils.h"

extern QJsonArray trDocArr;

TrTab::TrTab(QWidget *parent) : ConnectedWidget(parent){
    //Declare
    QVBoxLayout* trWidgetLayout = new QVBoxLayout;
    QHBoxLayout* trLayout = new QHBoxLayout;
    QPushButton* reqTrPush = new QPushButton("Request TR");
    //Initialize
    trTree = new QJsonTreeWidget();
        trTree->appendJson(trDocArr);
        trTree->expandAll();
    trList = new QListWidget;
        QStringList trNameList;
        for(int i=0;i<trDocArr.size();i++){
            QString optName = trDocArr.at(i).toObject().value("optName").toString();
            QString trDetail = trDocArr.at(i).toObject().value("trDetail").toString();
            trNameList.append(optName+" "+trDetail);
        }
        trList->addItems(trNameList);
    trTable = new QTableWidget;
        trTable->setColumnCount(1);
        trTable->setHorizontalHeaderLabels({"Input"});
        trTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //Render
    this->setLayout(trWidgetLayout);
        trWidgetLayout->addLayout(trLayout);
            trLayout->addWidget(trTree);
            trLayout->addWidget(trList);
            trLayout->addWidget(trTable);
        trWidgetLayout->addWidget(reqTrPush);
    //Connect
    connect(trList,&QListWidget::currentRowChanged,[=]{
       int i = trList->currentRow();
       QJsonObject currentTr = trDocArr.at(i).toObject();
       emit(updateCurrentTr(currentTr));
    });
    connect(reqTrPush,&QPushButton::clicked,[=]{
        int nInput = trTable->rowCount();
        QStringList inputList;
        for(int i=0;i<nInput;i++){
//            QString inputName = inputArr.at(i).toString();
            QString inputValue = trTable->item(i,0)->text();
            inputList.append(inputValue);
//            koa->setInputValue(inputName,inputValue);
        }
//        koa->commRqData("Test",optName,0,"1111");
        emit(requestTr(inputList));
    });
}
void TrTab::onStoreChanged(QJsonObject diffObj){
    if(diffObj.keys().contains("currentTr")){
        QJsonObject obj = diffObj.value("currentTr").toObject();
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
    }
}
