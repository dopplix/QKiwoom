#ifndef ASSETTAB_H
#define ASSETTAB_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QJsonArray>
#include <QJsonObject>
#include "components/qjsontreewidget.h"
#include "qflux/connectedwidget.h"

class AssetTab : public ConnectedWidget{
    Q_OBJECT

public:
    explicit AssetTab(QWidget *parent = nullptr);
    QJsonTreeWidget* assetTree = new QJsonTreeWidget;
    QListWidget* assetList = new QListWidget;
    QJsonArray assetDocArr;

signals:
    void requestAssetData();

public slots:
    void onStoreChanged(QJsonObject diffObj) override;
    void initAssets(QJsonArray docArr);

};

#endif // ASSETTAB_H
