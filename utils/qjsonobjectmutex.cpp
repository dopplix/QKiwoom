#include "qjsonobjectmutex.h"
#include <QDebug>

QJsonObjectMutex::QJsonObjectMutex(QObject *parent) : QObject(parent), QJsonObject(){

}
void QJsonObjectMutex::setValue(QString key, QJsonValue value){
    if(!mutex.tryLock()){
        mutex.lock();
    }
    bool isChanged = false;
    QJsonObject diffObj;
    if(needChangeCheck){
        QJsonValue oldValue = this->value(key);
        if(oldValue!=value){
            diffObj.insert(key,value);
            isChanged = true;
        }
    }
    this->insert(key,value);
    mutex.unlock();
    if(isChanged){
        emit(somethingChanged(diffObj));
    }
}
void QJsonObjectMutex::setObject(QJsonObject obj){
    QStringList keys = obj.keys();
    for(int i=0;i<keys.size();i++){
        QString key = keys.at(i);
        this->setValue(key,obj.value(key));
    }
}
void QJsonObjectMutex::removeValue(QString key){
    if(!this->keys().contains(key)){
        return;
    }
    if(!mutex.tryLock()){
        mutex.lock();
    }
    this->remove(key);
    mutex.unlock();
    if(needChangeCheck){
        QJsonObject diffObj;
        diffObj.insert(key,QJsonValue(NULL));
        emit(somethingChanged(QJsonObject()));
    }
}
QJsonValue QJsonObjectMutex::getValue(QString key){
    if(!mutex.tryLock()){
        mutex.lock();
    }
    QJsonValue value = this->value(key);
    mutex.unlock();
    return value;
}
QJsonObject QJsonObjectMutex::getObject(){
    if(!mutex.tryLock()){
        mutex.lock();
    }
    QJsonObject obj = *this; //Upcasting - implicit casting allowed
    mutex.unlock();
    return obj;
}
