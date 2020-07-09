#include "qjsonutils.h"

QJsonUtils::QJsonUtils(QObject *parent) : QObject(parent){

}
QJsonArray QJsonUtils::readJsonArrFromFile(QString path){
    QString str = readStrFromFile(path);
    return QJsonDocument::fromJson(str.toUtf8()).array();
}
QJsonObject QJsonUtils::readJsonObjFromFile(QString path){
    QString str = readStrFromFile(path);
    return QJsonDocument::fromJson(str.toUtf8()).object();
}
QString QJsonUtils::readStrFromFile(QString path){;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QString str=file.readAll();
    file.close();
    return str;
}
