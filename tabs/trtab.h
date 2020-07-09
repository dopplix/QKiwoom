#ifndef TRTAB_H
#define TRTAB_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include "../components/qjsontreewidget.h"
#include "../utils/qjsonobjectmutex.h"
#include <QJsonArray>

class TrTab : public QWidget{
    Q_OBJECT

public:
    explicit TrTab(QWidget *parent = nullptr);
    QJsonTreeWidget* trTree = new QJsonTreeWidget();
    QListWidget* trList = new QListWidget;
    QTableWidget* trTable = new QTableWidget;
    QJsonArray trDocArr;
    QJsonObjectMutex* store;

signals:
    void updateCurrentTr(QJsonObject currentTr);
    void requestTr(QStringList inputList);

public slots:
    void onStoreChanged(QJsonObject diffObj);
};

#endif // TRTAB_H
