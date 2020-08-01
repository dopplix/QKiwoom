#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include "fnctab.h"
#include "global.h"

FncTab::FncTab(QWidget *parent) : ConnectedWidget(parent){
    //Declare
    QVBoxLayout* functionWidgetLayout = new QVBoxLayout;
    QHBoxLayout* functionLayout = new QHBoxLayout;
    QPushButton* reqFncPush = new QPushButton("Call Function");
    QPushButton* testRealInputPush = new QPushButton("Test Fnc");
    //Initialize
    functionTree = new QJsonTreeWidget();
        functionTree->appendJson(fncDocArr);
        functionTree->expandAll();
        for(int i=0;i<functionTree->columnCount();i++){
            functionTree->resizeColumnToContents(i);
        }
    functionList = new QListWidget;
        QStringList fncNameList;
        for(int i=0;i<fncDocArr.size();i++){
            fncNameList.append(fncDocArr.at(i).toObject().value("functionName").toString());
        }
        functionList->addItems(fncNameList);
    functionTable = new QTableWidget;
        functionTable->setColumnCount(2);
        functionTable->setHorizontalHeaderLabels({"Type","Input"});
        functionTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //Render
    this->setLayout(functionWidgetLayout);
        functionWidgetLayout->addLayout(functionLayout);
            functionLayout->addWidget(functionTree);
            functionLayout->addWidget(functionList);
            functionLayout->addWidget(functionTable);
        functionWidgetLayout->addWidget(reqFncPush);
        functionWidgetLayout->addWidget(testRealInputPush);
    //Connect
    connect(functionList,&QListWidget::currentRowChanged,[=]{
        QString currentFncName = functionList->currentItem()->text();
        QJsonObject payload;
        payload.insert("currentFncName",currentFncName);
        emit(action(ActionTypes::FncTab::CHANGE_CURRENT_FUNCTION,payload));
    });
    connect(reqFncPush,&QPushButton::clicked,[=]{
        QString currentFncName = functionList->currentItem()->text();
        int nParam = functionTable->rowCount();
        QJsonArray argArr;
        for(int i=0;i<nParam;i++){
            QString arg = functionTable->item(i,1)->text();
            argArr.append(arg);
        }
        QJsonObject payload;
        payload.insert("currentFncName",currentFncName);
        payload.insert("args",argArr);
        emit(action(ActionTypes::FncTab::CALL_CURRENT_FUNCTION,payload));
    });
    connect(testRealInputPush,&QPushButton::clicked,[=](){
        isRun = !isRun;
        for(int i=0;i<10000;i++){
            if(!isRun) break;
            QEventLoop loop;
            QTimer::singleShot(2000, &loop, SLOT(quit()));
            loop.exec();
            //logEdit->append("Loop : "+QString::number(i));
            QTimer::singleShot(0,[=]{
               qDebug()<<i;
               //qDebug()<<koa->setRealReg("1111","000060;000120;000180;005930;006400;066570;005380;001450",QString::number(i),"0");
            });
        }
    });
}
void FncTab::onStoreChanged(QJsonObject diffObj){
    if(diffObj.keys().contains("currentFnc")){
        QJsonObject obj = diffObj.value("currentFnc").toObject();
        QJsonArray paramsArr = obj.value("params").toArray();
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
    }
}
