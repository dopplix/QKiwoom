#ifndef FNCTAB_H
#define FNCTAB_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QJsonArray>
#include "components/qjsontreewidget.h"
#include "qflux/connectedwidget.h"

class FncTab : public ConnectedWidget{
    Q_OBJECT

public:
    explicit FncTab(QWidget *parent = nullptr);
    QJsonTreeWidget* functionTree;
    QListWidget* functionList;
    QTableWidget* functionTable;
    bool isRun = false;

signals:
    void updateCurrentFnc(QJsonObject currentFnc);
    void callFnc(QStringList paramList);

public slots:
    void onStoreChanged(QJsonObject diffObj);

};

#endif // FNCTAB_H
