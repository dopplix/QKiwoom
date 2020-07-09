#ifndef ASSETTAB_H
#define ASSETTAB_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QJsonArray>
#include <QJsonObject>
#include "components/qjsontreewidget.h"

class AssetTab : public QWidget{
    Q_OBJECT

public:
    explicit AssetTab(QWidget *parent = nullptr);
    QJsonTreeWidget* assetTree = new QJsonTreeWidget;
    QListWidget* assetList = new QListWidget;
    QJsonArray assetDocArr;

signals:
    void requestAssetData();

public slots:
    void initAssets(QJsonArray docArr);

};

#endif // ASSETTAB_H
