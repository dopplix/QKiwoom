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
    connect(udfServer,&QTvUdfServer::kiwoomTrReq,udfWorker,[=](QJsonObject req,QJsonObject* resObj){
        QString optName = req.value("optName").toString();
        QJsonArray argArr = req.value("argArr").toArray();
        QJsonObject resultObj = requestKoaTr(optName,argArr);
        resObj->insert("result",resultObj);
    },Qt::BlockingQueuedConnection);
    connect(udfServer,&QTvUdfServer::kiwoomFncReq,udfWorker,[=](QJsonObject req,QJsonObject* resObj){
        QString fncName = req.value("fncName").toString();
        QJsonArray argArr = req.value("argArr").toArray();
        QVariant result = callKoaFnc(fncName,argArr);
        QJsonObject resultObj;
        resultObj.insert("returnValue",result.toJsonValue());
        resObj->insert("result",resultObj);
    },Qt::BlockingQueuedConnection);
//    connect(udfServer,&QTvUdfServer::kiwoomObjReq,worker,[=](QJsonObject req,QJsonObject* resObj){
//        qDebug()<<"2 WORKER THREAD"<<QThread::currentThread();
//        qDebug()<<"3"<<req;
//        koa->setInputValue(krMapObj.value("assetCode").toString(),"000060");
//        koa->setInputValue(krMapObj.value("targetDate").toString(),"20200716");
//        koa->setInputValue(krMapObj.value("modifiedPriceIndex").toString(),"0");
//        koa->commRqData("UDF","OPT10081",0,"1989");
//        QEventLoop* loop = new QEventLoop;
//        connect(this,&Dispatcher::historyReceived,[=](QJsonObject resultObj){
//            QJsonArray klineArr = resultObj.value("multi").toArray();
//            QJsonArray tArr;
//            QJsonArray oArr;
//            QJsonArray hArr;
//            QJsonArray lArr;
//            QJsonArray cArr;
//            QJsonArray vArr;
//            for(QJsonValue v : klineArr){
//                QJsonObject obj = v.toObject();
//                QString date = obj.value(krMapObj.value("date").toString()).toString();
//                QString open = obj.value(krMapObj.value("open").toString()).toString();
//                QString high = obj.value(krMapObj.value("high").toString()).toString();
//                QString low = obj.value(krMapObj.value("low").toString()).toString();
//                QString close = obj.value(krMapObj.value("close").toString()).toString();
//                QString volume = obj.value(krMapObj.value("volume").toString()).toString();
//                tArr.insert(0,double(QTimeUtil::kiwoomToDt(date).toTime_t()));
//                oArr.insert(0,open.toDouble());
//                hArr.insert(0,high.toDouble());
//                lArr.insert(0,low.toDouble());
//                cArr.insert(0,close.toDouble());
//                vArr.insert(0,volume.toDouble());
//            }
//            resObj->insert("s","ok");
//            resObj->insert("t",tArr);
//            resObj->insert("o",oArr);
//            resObj->insert("h",hArr);
//            resObj->insert("l",lArr);
//            resObj->insert("c",cArr);
//            resObj->insert("v",vArr);
//            QTimer::singleShot(0,loop,&QEventLoop::quit);
//        });
//        QTimer::singleShot(5000,loop,[=]{
//            resObj->insert("s","error");
//            resObj->insert("errmsg","Time Out");
//            QTimer::singleShot(0,loop,&QEventLoop::quit);
//        });
//        loop->exec();
//    },Qt::BlockingQueuedConnection);

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
    qDebug()<<"void Dispatcher::routeKoaEvents()";
    connect(koa,&QKoa::onMessageReceived,[=](QJsonObject obj){
        qDebug()<<"connect(koa,&QKoa::onMessageReceived,[=](QJsonObject obj)"<<obj;
        //logEdit->append(QJsonDocument(obj).toJson(QJsonDocument::Indented));
        QString event = obj.value("event").toString();
        QString sScrNo = obj.value("sScrNo").toString();
        if(event=="onReceiveTrData"){
            if(sScrNo=="1100"){
                QString optName = obj.value("sTrCode").toString();
                QJsonObject trObj = getTrObj(optName);
                QJsonObject finalResultObj = processTr(trObj);
                emit(trResultReceived(finalResultObj));
            }
//            else if(sScrNo=="1200"){
//                QString uuid = obj.value("sRQName").toString();
//                QJsonObject trOpt10003 = trTab->trDocArr.at(2).toObject();
//                QJsonObject finalResultObj = processTr(trOpt10003);
//                QJsonArray multiArr = finalResultObj.value("multi").toArray();
//                QJsonObject targetObj = multiArr.at(0).toObject();
//                QJsonObject queryObj = store->getValue(uuid).toObject();
//                QString condIndex = queryObj.value("condIndex").toString();
//                QString condName = queryObj.value("condName").toString();
//                QString eventName = queryObj.value("event").toString();
//                QString assetCode = queryObj.value("assetCode").toString();
//                QString assetName = queryObj.value("assetName").toString();
//                QString sign = targetObj.value("sign").toString();
//                QString accAmount = targetObj.value(krMapObj.value("accAmount").toString()).toString();
//                QString accSize = targetObj.value(krMapObj.value("accSize").toString()).toString();
//                QString rate = targetObj.value(krMapObj.value("rate").toString()).toString();
//                QString lastTrTime = targetObj.value(krMapObj.value("lastTrTime").toString()).toString();
//                QString bestAsk = targetObj.value(krMapObj.value("bestAsk").toString()).toString();
//                QString bestBid = targetObj.value(krMapObj.value("bestBid").toString()).toString();
//                QString diffPrice = targetObj.value(krMapObj.value("diffPrice").toString()).toString();
//                QString intense = targetObj.value(krMapObj.value("intense").toString()).toString();
//                QString size = targetObj.value(krMapObj.value("size").toString()).toString();
//                QString price = targetObj.value(krMapObj.value("price").toString()).toString();
//                qDebug()<<"Query"<<sendCondToMysql(condIndex,condName,assetName,eventName,assetCode,sign,accAmount,accSize,rate,lastTrTime,bestAsk,bestBid,diffPrice,intense,size,price);
//                store.removeValue(uuid);
//            }
//            else if(sScrNo=="1989"){
//                //TODO Get TR Obj by OptName
//                QJsonObject finalResultObj = processTr(trDocArr.at(77).toObject());
//                //store->setValue("UDF",finalResultObj);
//                emit(historyReceived(finalResultObj));
//            }
        }
        else if(event=="onReceiveConditionVer"){
            initConditions();
        }
        else if(event=="onReceiveRealCondition"){
            qDebug()<<obj;
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
        //logEdit->append("Single Result");
        //logEdit->append(QJsonDocument(resultSingleObj).toJson(QJsonDocument::Indented));
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
        //logEdit->append("Multi Result");
        //logEdit->append(QJsonDocument(resultArr).toJson(QJsonDocument::Indented));
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
        qDebug()<<result;
    }
    else if(actionType==ActionTypes::TrTab::CHANGE_CURRENT_TR){
        QString currentOptName = payload.value("currentOptName").toString();
        QJsonObject trObj = getTrObj(currentOptName);
        store.setValue("currentTr",trObj);
    }
    else if(actionType==ActionTypes::TrTab::CALL_CURRENT_TR){
        QString currentOptName = payload.value("currentOptName").toString();
        QJsonArray argArr = payload.value("args").toArray();
        qDebug()<<"TR"<<requestKoaTr(currentOptName,argArr);
    }
}
bool Dispatcher::sendCondToMysql(QString condIndex, QString condName, QString assetName, QString event, QString assetCode, QString sign, QString accAmount, QString accSize, QString rate, QString lastTrTime, QString bestAsk, QString bestBid, QString diffPrice, QString intense, QString size, QString price){
    QSqlQuery query;
    query.prepare("INSERT INTO tb_cond (eventTime,condIndex,condName,assetName,event,assetCode,sign,accAmount,accSize,rate,lastTrTime,bestAsk,bestBid,diffPrice,intense,size,price)"
                  "VALUES (:eventTime,:condIndex,:condName,:assetName,:event,:assetCode,:sign,:accAmount,:accSize,:rate,:lastTrTime,:bestAsk,:bestBid,:diffPrice,:intense,:size,:price)");
    query.bindValue(":eventTime",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":condIndex",condIndex);
    query.bindValue(":condName",condName);
    query.bindValue(":assetName",assetName);
    query.bindValue(":event",event);
    query.bindValue(":assetCode",assetCode);
    query.bindValue(":sign",sign);
    query.bindValue(":accAmount",accAmount);
    query.bindValue(":accSize",accSize);
    query.bindValue(":rate",rate);
    query.bindValue(":lastTrTime",lastTrTime);
    query.bindValue(":bestAsk",bestAsk);
    query.bindValue(":bestBid",bestBid);
    query.bindValue(":diffPrice",diffPrice);
    query.bindValue(":intense",intense);
    query.bindValue(":size",size);
    query.bindValue(":price",price);
    return query.exec();
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
QVariant Dispatcher::callKoaTr(QString optName, QJsonArray argArr){
    qDebug()<<"QVariant Dispatcher::callKoaTr(QString optName, QJsonArray argArr)"<<optName<<argArr;
    QJsonObject trObj = getTrObj(optName);
    QJsonArray inputArr = trObj.value("inputArr").toArray();
    int nArr = inputArr.size();
    for(int i=0;i<nArr;i++){
        QString inputName = inputArr.at(i).toString();
        QString inputValue = argArr.at(i).toString();
        koa->setInputValue(inputName,inputValue);
    }
    //QString sRQName = "TrTab";
    QString sRQName = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString sTrCode = optName;
    int nPrevNext = 0;
    QString sScrNo = "1100";
    QVariant result = koa->commRqData(sRQName,sTrCode,nPrevNext,sScrNo);
    return result;
}
QJsonObject Dispatcher::requestKoaTr(QString optName, QJsonArray argArr){
    qDebug()<<"Busy1"<<isKoaBusy<<optName<<argArr;
    if(isKoaBusy){
        qDebug()<<"Busy";
        QJsonObject busyObj;
        busyObj.insert("event","Dispatcher::requestKoaTr Koa Is Busy");
        return busyObj;
    }
    isKoaBusy=true;
    QEventLoop* loopPtr = new QEventLoop;
    QJsonObject* resultHolder = new QJsonObject;
    QTimer timer;
    QObject receiver;
    qDebug()<<"2"<<"requestKoaTr";
    connect(this,&Dispatcher::trResultReceived,&receiver,[resultHolder,loopPtr](QJsonObject resultObj){
        qDebug()<<"3"<<"requestKoaTr";
        resultHolder->insert("result",resultObj);
        loopPtr->quit();
    });
    QTimer::singleShot(5000,&receiver,[resultHolder,loopPtr]{
        QJsonObject errorObj;
        errorObj.insert("event","Dispatcher::requestKoaTr Time Out Error");
        resultHolder->insert("result",errorObj);
        loopPtr->quit();
    });
    callKoaTr(optName,argArr);
    loopPtr->exec();
    QJsonObject result = resultHolder->value("result").toObject();
    delete loopPtr;
    delete resultHolder;
    isKoaBusy=false;
    qDebug()<<"Busy2"<<isKoaBusy;
    return result;
}
