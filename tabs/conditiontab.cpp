#include "conditiontab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

ConditionTab::ConditionTab(QWidget *parent) : QWidget(parent){
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* conditionLayout = new QHBoxLayout;
    QPushButton* loadConditionPush = new QPushButton("Load Condition");
    conditionLayout->addWidget(conditionTree);
    conditionLayout->addWidget(conditionList);
    conditionLayout->addWidget(selectedConditionList);
    conditionLayout->addWidget(targetAssetTable);
    mainLayout->addWidget(loadConditionPush);
    mainLayout->addLayout(conditionLayout);
    this->setLayout(mainLayout);
    connect(loadConditionPush,&QPushButton::clicked,[=]{
         emit(requestCondition());
    });
    connect(conditionList,&QListWidget::doubleClicked,[=]{
        int currentRow = conditionList->currentRow();
        QJsonObject currentObj = conditionDocArr.at(currentRow).toObject();
        emit(appendCurrentCondition(currentObj));
    });
    connect(selectedConditionList,&QListWidget::doubleClicked,[=]{
        int currentRow = selectedConditionList->currentRow();
        emit(removeCurrentConditionAt(currentRow));
    });
}

QStringList ConditionTab::getConditionStrList(QJsonArray conditionArr){
    QStringList conditionStrList;
    for(QJsonValue condition : conditionArr){
        QJsonObject conditionObj = condition.toObject();
        QString conditionName = conditionObj.value("conditionName").toString();
        int conditionKey = conditionObj.value("conditionKey").toInt();
        conditionStrList.append(QString::number(conditionKey)+" "+conditionName);
    }
    return conditionStrList;
}
void ConditionTab::initConditions(QJsonArray conditionArr){
    conditionDocArr=conditionArr;
    conditionTree->appendJson(conditionDocArr);
    QStringList conditionStrList = getConditionStrList(conditionDocArr);
    conditionList->addItems(conditionStrList);
}
void ConditionTab::onStoreChanged(QJsonObject diffObj){
    qDebug()<<diffObj;
    if(diffObj.keys().contains("currentConditions")){
        QJsonArray currentConditionArr = diffObj.value("currentConditions").toArray();
        qDebug()<<currentConditionArr;
        selectedConditionList->clear();
        QStringList conditionStrList = getConditionStrList(currentConditionArr);
        selectedConditionList->addItems(conditionStrList);
    }
}
