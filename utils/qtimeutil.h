#ifndef QTIMEUTIL_H
#define QTIMEUTIL_H

#include <QObject>
#include <QDateTime>

class QTimeUtil{

public:
    QTimeUtil();
    static QDateTime kiwoomToDt(QString kiwoomDtStr);
    static QString timestampToKiwoom(int timestamp);

};

#endif // QTIMEUTIL_H
