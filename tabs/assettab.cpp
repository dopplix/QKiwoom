#include "assettab.h"
#include <QVBoxLayout>
#include <QPushButton>

AssetTab::AssetTab(QWidget *parent) : QWidget(parent){
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* assetLayout = new QHBoxLayout;
    QPushButton* requestAssetPush = new QPushButton("Load Asset Data");
    assetLayout->addWidget(assetTree);
    assetLayout->addWidget(assetList);
    mainLayout->addWidget(requestAssetPush);
    mainLayout->addLayout(assetLayout);
    this->setLayout(mainLayout);
    connect(requestAssetPush,&QPushButton::clicked,[=]{
        emit(requestAssetData());
    });

}
void AssetTab::initAssets(QJsonArray docArr){
    assetDocArr = docArr;
    assetTree->appendJson(docArr);
    QStringList assetStrList;
    for(QJsonValue assetValue : assetDocArr){
        QJsonObject assetObj = assetValue.toObject();
        QString name = assetObj.value("name").toString();
        QString code = assetObj.value("code").toString();
        assetStrList.append(code+" "+name);
    }
    assetList->addItems(assetStrList);
}
