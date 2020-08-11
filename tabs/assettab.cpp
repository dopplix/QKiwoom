#include "assettab.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "global.h"

AssetTab::AssetTab(QWidget *parent) : ConnectedWidget(parent){
    //Declare
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* assetLayout = new QHBoxLayout;
    QPushButton* requestAssetPush = new QPushButton("Load Asset Data");
    //Render
    this->setLayout(mainLayout);
        mainLayout->addWidget(requestAssetPush);
        mainLayout->addLayout(assetLayout);
            assetLayout->addWidget(assetTree);
            assetLayout->addWidget(assetList);
    //Connect
    connect(requestAssetPush,&QPushButton::clicked,[=]{
        emit(action(ActionTypes::AssetTab::REQUEST_ASSET_LIST,QJsonObject()));
    });
}
void AssetTab::onStoreChanged(QJsonObject diffObj){
    if(diffObj.keys().contains("assetDoc")){
        QJsonObject assetDocObj = diffObj.value("assetDoc").toObject();
        assetTree->clear();
        assetTree->appendJson(assetDocObj);
        initAssetList(assetDocObj);
    }
}
void AssetTab::initAssetList(QJsonObject docObj){
    QStringList assetStrList;
    for(QString key : docObj.keys()){
        QJsonObject assetObj = docObj.value(key).toObject();
        QString name = assetObj.value("name").toString();
        QString code = assetObj.value("code").toString();
        assetStrList.append(code+" "+name);
    }
    assetList->addItems(assetStrList);
}
