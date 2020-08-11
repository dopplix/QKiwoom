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
