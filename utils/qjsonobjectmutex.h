#ifndef MYOBJECT_H
#define MYOBJECT_H
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMutex>

class QJsonObjectMutex : public QObject, public QJsonObject{
    Q_OBJECT

public:
    explicit QJsonObjectMutex(QObject *parent = nullptr);
    void setValue(QString key, QJsonValue value);
    void setObject(QJsonObject obj);
    void removeValue(QString key);
    QJsonValue getValue(QString key);
    QJsonObject getObject();
    void setChangeCheck(bool ok){ needChangeCheck=ok; }
    QMutex mutex;

private:
    bool needChangeCheck = false;

signals:
    void somethingChanged(QJsonObject);
};

#endif // MYOBJECT_H
