#ifndef TRTAB_H
#define TRTAB_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QJsonArray>
#include "components/qjsontreewidget.h"
#include "qflux/connectedwidget.h"

class TrTab : public ConnectedWidget{
    Q_OBJECT

public:
    explicit TrTab(QWidget *parent = nullptr);
    QString getCurrentOptName();
    QJsonTreeWidget* trTree;
    QListWidget* trList;
    QTableWidget* trTable;

signals:
    void updateCurrentTr(QJsonObject currentTr);
    void requestTr(QStringList inputList);

public slots:
    void onStoreChanged(QJsonObject diffObj);
};

#endif // TRTAB_H
