#ifndef QHTTPUTIL_H
#define QHTTPUTIL_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class QHttpUtil{
    
public:
    QHttpUtil();
    static QJsonObject urlToObj(QString urlStr);
};

#endif // QHTTPUTIL_H
