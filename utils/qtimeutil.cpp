#include "qtimeutil.h"

QTimeUtil::QTimeUtil(){

}
QDateTime QTimeUtil::kiwoomToDt(QString kiwoomDtStr){
    QString yearStr = kiwoomDtStr.mid(0,4);
    QString monthStr = kiwoomDtStr.mid(4,2);
    QString dayStr = kiwoomDtStr.mid(6,2);
    QString hourStr = "0";
    QString minuteStr = "0";
    QString secondStr = "0";
    if(kiwoomDtStr.length()>6){
        hourStr = kiwoomDtStr.mid(8,2);
        minuteStr = kiwoomDtStr.mid(10,2);
        secondStr = kiwoomDtStr.mid(12,2);
    }
    return QDateTime(QDate(yearStr.toInt(),monthStr.toInt(),dayStr.toInt()),QTime(hourStr.toInt(),minuteStr.toInt(),secondStr.toInt()));
}
QString QTimeUtil::timestampToKiwoom(int timestamp){
    QDateTime dt = QDateTime::fromTime_t(uint(timestamp));
    QString kiwoomDtStr;
    kiwoomDtStr.append(QString::number(dt.date().year()).rightJustified(4,'0'));
    kiwoomDtStr.append(QString::number(dt.date().month()).rightJustified(2,'0'));
    kiwoomDtStr.append(QString::number(dt.date().day()).rightJustified(2,'0'));
    kiwoomDtStr.append(QString::number(dt.time().hour()).rightJustified(2,'0'));
    kiwoomDtStr.append(QString::number(dt.time().minute()).rightJustified(2,'0'));
    kiwoomDtStr.append(QString::number(dt.time().second()).rightJustified(2,'0'));
    return kiwoomDtStr;
}
