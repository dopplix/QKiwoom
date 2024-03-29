#ifndef CONDITIONTAB_H
#define CONDITIONTAB_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QJsonArray>
#include "components/qjsontreewidget.h"
#include "qflux/connectedwidget.h"

class ConditionTab : public ConnectedWidget{
    Q_OBJECT

public:
    explicit ConditionTab(QWidget *parent = nullptr);
    QJsonTreeWidget* conditionTree = new QJsonTreeWidget;
    QListWidget* conditionList = new QListWidget;
    QListWidget* selectedConditionList = new QListWidget;
    QTableWidget* targetAssetTable = new QTableWidget;
    QJsonArray conditionDocArr;
    QStringList getConditionStrList(QJsonArray conditionArr);

signals:
    void requestCondition();
    void removeCurrentConditionAt(int i);
    void appendCurrentCondition(QJsonObject conditionObj);

public slots:
    void initConditions(QJsonArray conditionArr);
    void onStoreChanged(QJsonObject diffObj) override;

};

#endif // CONDITIONTAB_H
