#ifndef QJSONUTILS_H
#define QJSONUTILS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class QJsonUtils : public QObject{
    Q_OBJECT

public:
    explicit QJsonUtils(QObject *parent = nullptr);
    static QJsonArray readJsonArrFromFile(QString path);
    static QJsonObject readJsonObjFromFile(QString path);
    static QString readStrFromFile(QString path);
    static QJsonObject findObjFromArrByKey(QJsonArray objArr, QString key, QJsonValue value);
    static QString objToStr(QJsonObject obj);
    static QString arrToStr(QJsonArray arr);

signals:

};

#endif // QJSONUTILS_H
