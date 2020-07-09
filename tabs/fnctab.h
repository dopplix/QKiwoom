#ifndef FNCTAB_H
#define FNCTAB_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QJsonArray>
#include "../utils/qjsonobjectmutex.h"
#include "../components/qjsontreewidget.h"

class FncTab : public QWidget{
    Q_OBJECT

public:
    explicit FncTab(QWidget *parent = nullptr);
    QJsonTreeWidget* functionTree = new QJsonTreeWidget();
    QListWidget* functionList = new QListWidget;
    QTableWidget* functionTable = new QTableWidget;
    QJsonArray fncDocArr;
    QJsonObjectMutex* store;
    bool isRun = false;

signals:
    void updateCurrentFnc(QJsonObject currentFnc);
    void callFnc(QStringList paramList);

public slots:
    void onStoreChanged(QJsonObject diffObj);

};

#endif // FNCTAB_H
