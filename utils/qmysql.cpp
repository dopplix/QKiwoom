#include "qmysql.h"
#include <QSqlDatabase>
#include <QDebug>

QMysql::QMysql(QObject *parent) : QObject(parent){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("vesta.cmu3vcb7qcix.ap-northeast-2.rds.amazonaws.com");
    db.setDatabaseName("vesta_utf8");
    db.setUserName("msjeon");
    db.setPassword("wjsalstjd120");
    qDebug()<<"DB Connection: "<<db.open();
}
