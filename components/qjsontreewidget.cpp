#include "qjsontreewidget.h"
#include <QDebug>

QJsonTreeWidget::QJsonTreeWidget() : QTreeWidget(){
    setHeaderLabels({"Key","Value","Type"});
    this->setColumnCount(3);
    this->addTopLevelItem(rootItem);
}
void QJsonTreeWidget::appendJson(QTreeWidgetItem* item, QJsonObject& obj){
    QStringList keys = obj.keys();
    for(QString key : keys){
        QJsonValue value = obj.value(key);
        QTreeWidgetItem* childItem = new QTreeWidgetItem();
        childItem->setText(0,key);
        if(value.isBool()){
            childItem->setText(1,QVariant(value.toBool()).toString());
            childItem->setText(2,"bool");
        }else if(value.isDouble()){
            childItem->setText(1,QString::number(value.toDouble()));
            childItem->setText(2,"double");
        }else if(value.isString()){
            childItem->setText(1,value.toString());
            childItem->setText(2,"string");
        }else if(value.isArray()){
            QJsonArray childArr = value.toArray();
            childItem->setText(2,"array["+QString::number(childArr.size())+"]");
            appendJson(childItem,childArr);
        }else if(value.isObject()){
            QJsonObject childObj = value.toObject();
            childItem->setText(2,"object{"+QString::number(childObj.keys().size())+"}");
            appendJson(childItem,childObj);
        }else if(value.isNull()){
            childItem->setText(1,"null");
            childItem->setText(2,"null");
        }
        item->addChild(childItem);
    }
}
void QJsonTreeWidget::appendJson(QTreeWidgetItem *item, QJsonArray& arr){
    for(int i=0;i<arr.size();i++){
        QJsonObject obj;
        obj.insert(QString::number(i),arr.at(i));
        appendJson(item,obj);
    }
}
void QJsonTreeWidget::appendJson(QJsonObject &obj){
    QJsonObject rootObj;
    rootObj.insert("Object",obj);
    appendJson(rootItem,rootObj);
}
void QJsonTreeWidget::appendJson(QJsonArray &arr){
    QJsonObject rootObj;
    rootObj.insert("Array",arr);
    appendJson(rootItem,rootObj);
}
