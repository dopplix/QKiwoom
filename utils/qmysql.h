#ifndef QMYSQL_H
#define QMYSQL_H

#include <QObject>

class QMysql : public QObject{
    Q_OBJECT

public:
    explicit QMysql(QObject *parent = nullptr);

signals:

};

#endif // QMYSQL_H
