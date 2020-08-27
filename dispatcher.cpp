#include <QEventLoop>
#include <QTimer>
#include <QUuid>
#include <QSqlQuery>
#include <QSharedPointer>
#include "dispatcher.h"
#include "global.h"

Dispatcher::Dispatcher(){
    initUdfWorker();
    routeKoaEvents();
}
QJsonObject Dispatcher::getFncObj(QString fncName){
    return QJsonUtils::findObjFromArrByKey(fncDocArr,"functionName",fncName);
}
QJsonObject Dispatcher::getTrObj(QString optName){
    return QJsonUtils::findObjFromArrByKey(trDocArr,"optName",optName);
}
void Dispatcher::initUdfWorker(){
    udfThread = new QThread;
    udfWorker = new QObject;
    udfThread->start();
    udfWorker->moveToThread(udfThread);
    connect(udfWorker,&QObject::destroyed,udfThread,&QThread::quit);
    connect(udfThread,&QThread::finished,udfThread,&QThread::deleteLater);
    connect(udfServer,&QTvUdfServer::requestKiwoomData,udfWorker,[=](QJsonObject req,QJsonObject* resObj){
        QString reqType = req.value("reqType").toString();
        if(reqType=="POST_TR"){
            QString optName = req.value("optName").toString();
            QJsonArray argArr = req.value("argArr").toArray();
            QJsonObject resultObj = requestKoaTr(optName,argArr);
            resObj->insert("result",resultObj);
        }
        else if(reqType=="POST_FNC"){
            QString fncName = req.value("fncName").toString();
            QJsonArray argArr = req.value("argArr").toArray();
            QVariant result = callKoaFnc(fncName,argArr);
            QJsonObject resultObj;
            resultObj.insert("returnValue",result.toJsonValue());
            resObj->insert("result",resultObj);
        }
        else if(reqType=="POST_ASSETS"){
            QJsonArray assetCodes = req.value("assets").toArray();
            QJsonObject assetObjs = getAssets();
            if(assetCodes.size()==0){
                resObj->insert("result",assetObjs);
                return;
            }
            QJsonObject filteredObj = QJsonObject();
            for(QJsonValue code : assetCodes){
                QString key = code.toString();
                filteredObj.insert(key,assetObjs.value(key));
            }
            resObj->insert("result",filteredObj);
        }
    },Qt::BlockingQueuedConnection);
}
void Dispatcher::initConditions(){
    QString conditionStr = koa->getConditionNameList();
    qDebug()<<conditionStr;
    QStringList conditionList = conditionStr.split(";");
    conditionList.removeOne("");
    QJsonArray conditionArr;
    for(QString condition : conditionList){
        QJsonObject conditionObj;
        QStringList conditionSplit = condition.split("^");
        conditionObj.insert("conditionKey",conditionSplit.at(0).toInt());
        conditionObj.insert("conditionName",conditionSplit.at(1));
        conditionArr.append(conditionObj);
    }
    //conditionTab->initConditions(conditionArr);
}
void Dispatcher::routeKoaEvents(){
    connect(koa,&QKoa::onMessageReceived,[=](QJsonObject obj){
        QString event = obj.value("event").toString();
        QString sScrNo = obj.value("sScrNo").toString();
        int scrInt = sScrNo.toInt();
        if(event=="onReceiveTrData"){
            qDebug()<<"SCRNO"<<scrInt;
            if(scrInt<4000 && scrInt>=3000){
                QString sRQName = obj.value("sRQName").toString();
                QString optName = obj.value("sTrCode").toString();
                QJsonObject trObj = getTrObj(optName);
                QJsonObject finalResultObj = processTr(trObj);
                finalResultObj.insert("sRQName",sRQName);
                emit(trResultReceived(finalResultObj));
            }
        }
        else if(event=="onReceiveRealData"){
            //qDebug()<<obj;
            QString assetCode = obj.value("sRealKey").toString();
            QString type = obj.value("sRealType").toString();
            QJsonObject fidObj = fidDocObj.value(type).toObject();
            QJsonObject resultObj;
            for(QString fid : fidObj.keys()){
                QString key = fidObj.value(fid).toString();
                QString data = koa->getCommRealData(assetCode,fid.toInt());
                resultObj.insert(key,data);
            }
            resultObj.insert("code",assetCode);
            resultObj.insert("type",type);
            resultObj.insert("asset",koa->getMasterCodeName(assetCode));
            bmWsServer->sendMessageToAllClient(QJsonUtils::objToStr(resultObj));
        }
        else if(event=="onReceiveChejanData"){
            qDebug()<<"ON_RECEIVE_CHEJAN_DATA";
            QString sGubun = obj.value("sGubun").toString();
            QString fids = obj.value("sFIdList").toString();
            QStringList fidList = fids.split(";");
            QString realType;
            if(sGubun=="0"){
                realType = krMapObj.value("filled").toString();
            }else if(sGubun=="1"){
                realType = krMapObj.value("balance").toString();
            }else{
                qDebug()<<"OTHER SGUBUN"<<obj;
                return;
            }
            QJsonObject fidObj = fidDocObj.value(realType).toObject();
            QJsonObject resultObj;
            for(QString fid : fidObj.keys()){
                QString key = fidObj.value(fid).toString();
                QString data = koa->getChejanData(fid.toInt());
                resultObj.insert(key,data);
            }
            resultObj.insert("type",realType);
            qDebug()<<resultObj;
            bmWsServer->sendMessageToAllClient(QJsonUtils::objToStr(resultObj));
        }
        else if(event=="onReceiveConditionVer"){
            initConditions();
        }
        else if(event=="onReceiveRealCondition"){
            QString assetCode = obj.value("sTrCode").toString();
            QJsonObject queryObj;
            QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
            queryObj.insert("condIndex",obj.value("strConditionIndex").toString());
            queryObj.insert("condName",obj.value("strConditionName").toString());
            queryObj.insert("event",obj.value("strType").toString());
            queryObj.insert("assetCode",assetCode);
            queryObj.insert("assetName",koa->getMasterCodeName(assetCode));
            store.setValue(uuid,queryObj);
            koa->setInputValue(krMapObj.value("assetCode").toString(),assetCode);
            koa->commRqData(uuid,"OPT10003",0,"1200");
        }
    });
}
QJsonObject Dispatcher::processTr(QJsonObject trObj){
    QString optName = trObj.value("optName").toString();
    QJsonObject finalResultObj;
    finalResultObj.insert("optName",optName);
    //Single
    QString outputSingleName = trObj.value("outputSingleName").toString();
    if(!outputSingleName.isEmpty()){
        QJsonArray outputSingleArr = trObj.value("outputSingleArr").toArray();
        int nSingleArr = outputSingleArr.size();
        QJsonObject resultSingleObj;
        for(int i=0;i<nSingleArr;i++){
            QString recordName = outputSingleArr.at(i).toString();
            QString ret = koa->getCommData(optName,outputSingleName,0,recordName);
            resultSingleObj.insert(recordName,ret.replace(" ",""));
        }
        finalResultObj.insert("single",resultSingleObj);
    }
    //Multi
    QString outputMultiName = trObj.value("outputMultiName").toString();
    if(!outputMultiName.isEmpty()){
        QJsonArray outputMultiArr = trObj.value("outputMultiArr").toArray();
        int nMultiArr = outputMultiArr.size();
        int nIdx = koa->getRepeatCnt(optName,outputMultiName);
        QJsonArray resultArr;
        for(int i=0;i<nIdx;i++){
            QJsonObject resultMultiObj;
            for(int j=0;j<nMultiArr;j++){
                QString recordName = outputMultiArr.at(j).toString();
                QString ret = koa->getCommData(optName,outputMultiName,i,recordName);
                resultMultiObj.insert(recordName,ret.replace(" ",""));
            }
            resultArr.append(resultMultiObj);
        }
        finalResultObj.insert("multi",resultArr);
    }
    return finalResultObj;
}
void Dispatcher::dispatch(QString actionType, QJsonObject payload){
    qDebug()<<"DISPATCH"<<actionType<<payload;
    if(actionType==ActionTypes::TEST){
        koa->setInputValue(krMapObj.value("assetCode").toString(),"000060");
        koa->setInputValue(krMapObj.value("targetDate").toString(),"20200716");
        koa->setInputValue(krMapObj.value("modifiedPriceIndex").toString(),"0");
        koa->commRqData("UDF","OPT10081",0,"1989");
    }
    else if(actionType==ActionTypes::FncTab::CHANGE_CURRENT_FUNCTION){
        QString currentFncName = payload.value("currentFncName").toString();
        QJsonObject fncObj = getFncObj(currentFncName);
        store.setValue("currentFnc",fncObj);
    }
    else if(actionType==ActionTypes::FncTab::CALL_CURRENT_FUNCTION){
        QString currentFncName = payload.value("currentFncName").toString();
        QJsonArray argArr = payload.value("args").toArray();
        QVariant result = callKoaFnc(currentFncName,argArr);
        QJsonObject resultObj;
        resultObj.insert("result",result.toJsonValue());
        appendLog(resultObj);
    }
    else if(actionType==ActionTypes::TrTab::CHANGE_CURRENT_TR){
        QString currentOptName = payload.value("currentOptName").toString();
        QJsonObject trObj = getTrObj(currentOptName);
        store.setValue("currentTr",trObj);
    }
    else if(actionType==ActionTypes::TrTab::CALL_CURRENT_TR){
        QString currentOptName = payload.value("currentOptName").toString();
        QJsonArray argArr = payload.value("args").toArray();
        QJsonObject resultObj = requestKoaTr(currentOptName,argArr);
        appendLog(resultObj);
    }
    else if(actionType==ActionTypes::AssetTab::REQUEST_ASSET_LIST){
        QJsonObject assets = getAssets();
        store.setValue("assetDoc",assets);
        appendLog(assets);
    }
}
QVariant Dispatcher::callKoaFnc(QString fncName, QJsonArray argArr){
    QJsonObject fncObj = getFncObj(fncName);
    QJsonArray paramArr = fncObj.value("params").toArray();
    int nParam = paramArr.size();
    QVariantList argVariantList;
    QStringList paramTypes;
    for(int i=0;i<nParam;i++){
        QString type = paramArr.at(i).toObject().value("paramType").toString();
        QString argStr = argArr.at(i).toString();
        if(type=="int"){
            argVariantList.append(argStr.toInt());
        }else if(type=="QString"){
            argVariantList.append(argStr);
        }
        paramTypes.append(type);
    }
    QString paramHolder = "("+paramTypes.join(", ")+")";
    QString functionFullName = fncName+paramHolder;
    QByteArray fullNameBytes = functionFullName.toLocal8Bit();
    char* fullNameBuffer = fullNameBytes.data();
    return koa->ax->dynamicCall(fullNameBuffer,argVariantList);
}
QVariant Dispatcher::callKoaTr(QString optName, QJsonArray argArr, QString sRQName){
    //qDebug()<<"QVariant Dispatcher::callKoaTr(QString optName, QJsonArray argArr)"<<optName<<argArr;
    QJsonObject trObj = getTrObj(optName);
    QJsonArray inputArr = trObj.value("inputArr").toArray();
    int nArr = inputArr.size();
    for(int i=0;i<nArr;i++){
        QString inputName = inputArr.at(i).toString();
        QString inputValue = argArr.at(i).toString();
        koa->setInputValue(inputName,inputValue);
    }
    //QString sRQName = "TrTab";
    QString sTrCode = optName;
    int nPrevNext = 0;
    QString sScrNo = QString::number(3000+trScrNo%1000);
    trScrNo++;
    QVariant result = koa->commRqData(sRQName,sTrCode,nPrevNext,sScrNo);
    return result;
}
QJsonObject Dispatcher::requestKoaTr(QString optName, QJsonArray argArr){
    QString sRQName = QUuid::createUuid().toString(QUuid::WithoutBraces);
    //qDebug()<<"QJsonObject Dispatcher::requestKoaTr(QString optName, QJsonArray argArr)";
    //qDebug()<<optName<<argArr<<sRQName<<"\n";
    QEventLoop* loopPtr = new QEventLoop;
    QJsonObject* resultHolder = new QJsonObject;
    QTimer timer;
    QObject receiver;
    connect(this,&Dispatcher::trResultReceived,&receiver,[resultHolder,loopPtr,sRQName,optName](QJsonObject resultObj){
        QString receivedSRQName = resultObj.value("sRQName").toString();
        QString receivedOptName = resultObj.value("optName").toString();
        qDebug()<<"# IN REQUEST KOA TR LOOP ELIMINATOR"<<"\n"<<"sRQ"<<sRQName<<"receivedSRQ"<<receivedSRQName;
        if(receivedSRQName==sRQName || receivedOptName==optName){
            resultHolder->insert("result",resultObj);
            loopPtr->quit();
        }
    });
    QTimer::singleShot(10000,&receiver,[resultHolder,loopPtr]{
        QJsonObject errorObj;
        errorObj.insert("event","Dispatcher::requestKoaTr Time Out Error");
        resultHolder->insert("result",errorObj);
        loopPtr->quit();
    });
    callKoaTr(optName,argArr,sRQName);
    loopPtr->exec();
    QJsonObject result = resultHolder->value("result").toObject();
    delete loopPtr;
    delete resultHolder;
    return result;
}
QJsonObject Dispatcher::getAssets(){
    QString codeStr = koa->getCodeListByMarket("0");
    QStringList codeList = codeStr.split(";");
    codeList.removeOne("");
    QJsonObject assets;
    for(QString code : codeList){
        QJsonObject obj;
        obj.insert("code",code);
        obj.insert("name",koa->getMasterCodeName(code));
        obj.insert("listedDate",koa->getMasterListedStockDate(code));
        obj.insert("nStock",koa->getMasterListedStockCnt(code));
        obj.insert("lastPrice",koa->getMasterLastPrice(code));
        obj.insert("info",koa->getMasterConstruction(code));
        assets.insert(code,obj);
    }
    return assets;
}
