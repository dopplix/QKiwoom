#include "qkoa.h"

QKoa::QKoa(QObject *parent) : QObject(parent){
    QObject::connect(ax, SIGNAL(OnEventConnect(int)), this, SLOT(onEventConnect(int)));
    QObject::connect(ax, SIGNAL(OnReceiveChejanData(QString, int, QString)), this, SLOT(onReceiveChejanData(QString, int, QString)));
    QObject::connect(ax, SIGNAL(OnReceiveConditionVer(int, QString)), this, SLOT(onReceiveConditionVer(int, QString)));
    QObject::connect(ax, SIGNAL(OnReceiveInvestRealData(QString)), this, SLOT(onReceiveInvestRealData(QString)));
    QObject::connect(ax, SIGNAL(OnReceiveMsg(QString, QString, QString, QString)), this, SLOT(onReceiveMsg(QString, QString, QString, QString)));
    QObject::connect(ax, SIGNAL(OnReceiveRealCondition(QString, QString, QString, QString)), this, SLOT(onReceiveRealCondition(QString, QString, QString, QString)));
    QObject::connect(ax, SIGNAL(OnReceiveRealData(QString, QString, QString)), this, SLOT(onReceiveRealData(QString, QString, QString)));
    QObject::connect(ax, SIGNAL(OnReceiveTrCondition(QString, QString, QString, int, int)), this, SLOT(onReceiveTrCondition(QString, QString, QString, int, int)));
    QObject::connect(ax, SIGNAL(OnReceiveTrData(QString, QString, QString, QString, QString, int, QString, QString, QString)), this, SLOT(onReceiveTrData(QString, QString, QString, QString, QString, int, QString, QString, QString)));
    QObject::connect(ax, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(onException(int, QString, QString, QString)));
    QObject::connect(ax, SIGNAL(propertyChanged(QString)), this, SLOT(onPropertyChanged(QString)));
    QObject::connect(ax, SIGNAL(signal(QString, int, void*)), this, SLOT(onSignal(QString, int, void*)));
}
int QKoa::commConnect(){
    return ax->dynamicCall("CommConnect()").toInt();
}
QString QKoa::commGetData(QString sJongmokCode, QString sRealType, QString sFieldName, int nIndex, QString sInnerFieldName){
    QVariantList params = {sJongmokCode, sRealType, sFieldName, nIndex, sInnerFieldName};
    return ax->dynamicCall("CommGetData(QString, QString, QString, int, QString)", params).toString();
}
int QKoa::commInvestRqData(QString sMarketGb, QString sRQName, QString sScreenNo){
    QVariantList params = {sMarketGb, sRQName, sScreenNo};
    return ax->dynamicCall("CommInvestRqData(QString, QString, QString)", params).toInt();
}
int QKoa::commKwRqData(QString sArrCode, int bNext, int nCodeCount, int nTypeFlag, QString sRQName, QString sScreenNo){
    QVariantList params = {sArrCode, bNext, nCodeCount, nTypeFlag, sRQName, sScreenNo};
    return ax->dynamicCall("CommKwRqData(QString, int, int, int, QString, QString)", params).toInt();
}
int QKoa::commRqData(QString sRQName, QString sTrCode, int nPrevNext, QString sScreenNo){
    QVariantList params = {sRQName, sTrCode, nPrevNext, sScreenNo};
    return ax->dynamicCall("CommRqData(QString, QString, int, QString)", params).toInt();
}
void QKoa::commTerminate(){
    ax->dynamicCall("CommTerminate()");
}
void QKoa::disconnectRealData(QString sScnNo){
    QVariantList params = {sScnNo};
    ax->dynamicCall("DisconnectRealData(QString)", params);
}
QString QKoa::getAPIModulePath(){
    return ax->dynamicCall("GetAPIModulePath()").toString();
}
QString QKoa::getActPriceList(){
    return ax->dynamicCall("GetActPriceList()").toString();
}
QString QKoa::getBranchCodeName(){
    return ax->dynamicCall("GetBranchCodeName()").toString();
}
QString QKoa::getChejanData(int nFid){
    QVariantList params = {nFid};
    return ax->dynamicCall("GetChejanData(int)", params).toString();
}
QString QKoa::getCodeListByMarket(QString sMarket){
    QVariantList params = {sMarket};
    return ax->dynamicCall("GetCodeListByMarket(QString)", params).toString();
}
QString QKoa::getCommData(QString strTrCode, QString strRecordName, int nIndex, QString strItemName){
    QVariantList params = {strTrCode, strRecordName, nIndex, strItemName};
    return ax->dynamicCall("GetCommData(QString, QString, int, QString)", params).toString();
}
QVariant QKoa::getCommDataEx(QString strTrCode, QString strRecordName){
    QVariantList params = {strTrCode, strRecordName};
    return ax->dynamicCall("GetCommDataEx(QString, QString)", params);
}
QString QKoa::getCommRealData(QString sTrCode, int nFid){
    QVariantList params = {sTrCode, nFid};
    return ax->dynamicCall("GetCommRealData(QString, int)", params).toString();
}
int QKoa::getConditionLoad(){
    return ax->dynamicCall("GetConditionLoad()").toInt();
}
QString QKoa::getConditionNameList(){
    return ax->dynamicCall("GetConditionNameList()").toString();
}
int QKoa::getConnectState(){
    return ax->dynamicCall("GetConnectState()").toInt();
}
int QKoa::getDataCount(QString strRecordName){
    QVariantList params = {strRecordName};
    return ax->dynamicCall("GetDataCount(QString)", params).toInt();
}
QString QKoa::getFutureCodeByIndex(int nIndex){
    QVariantList params = {nIndex};
    return ax->dynamicCall("GetFutureCodeByIndex(int)", params).toString();
}
QString QKoa::getFutureList(){
    return ax->dynamicCall("GetFutureList()").toString();
}
QString QKoa::getLoginInfo(QString sTag){
    QVariantList params = {sTag};
    return ax->dynamicCall("GetLoginInfo(QString)", params).toString();
}
int QKoa::getMarketType(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMarketType(QString)", params).toInt();
}
QString QKoa::getMasterCodeName(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMasterCodeName(QString)", params).toString();
}
QString QKoa::getMasterConstruction(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMasterConstruction(QString)", params).toString();
}
QString QKoa::getMasterLastPrice(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMasterLastPrice(QString)", params).toString();
}
int QKoa::getMasterListedStockCnt(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMasterListedStockCnt(QString)", params).toInt();
}
QString QKoa::getMasterListedStockDate(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMasterListedStockDate(QString)", params).toString();
}
QString QKoa::getMasterStockState(QString sTrCode){
    QVariantList params = {sTrCode};
    return ax->dynamicCall("GetMasterStockState(QString)", params).toString();
}
QString QKoa::getMonthList(){
    return ax->dynamicCall("GetMonthList()").toString();
}
QString QKoa::getOptionATM(){
    return ax->dynamicCall("GetOptionATM()").toString();
}
QString QKoa::getOptionCode(QString strActPrice, int nCp, QString strMonth){
    QVariantList params = {strActPrice, nCp, strMonth};
    return ax->dynamicCall("GetOptionCode(QString, int, QString)", params).toString();
}
QString QKoa::getOptionCodeByActPrice(QString sTrCode, int nCp, int nTick){
    QVariantList params = {sTrCode, nCp, nTick};
    return ax->dynamicCall("GetOptionCodeByActPrice(QString, int, int)", params).toString();
}
QString QKoa::getOptionCodeByMonth(QString sTrCode, int nCp, QString strMonth){
    QVariantList params = {sTrCode, nCp, strMonth};
    return ax->dynamicCall("GetOptionCodeByMonth(QString, int, QString)", params).toString();
}
QString QKoa::getOutputValue(QString strRecordName, int nRepeatIdx, int nItemIdx){
    QVariantList params = {strRecordName, nRepeatIdx, nItemIdx};
    return ax->dynamicCall("GetOutputValue(QString, int, int)", params).toString();
}
int QKoa::getRepeatCnt(QString sTrCode, QString sRecordName){
    QVariantList params = {sTrCode, sRecordName};
    return ax->dynamicCall("GetRepeatCnt(QString, QString)", params).toInt();
}
QString QKoa::getSActPriceList(QString strBaseAssetGb){
    QVariantList params = {strBaseAssetGb};
    return ax->dynamicCall("GetSActPriceList(QString)", params).toString();
}
QString QKoa::getSFOBasisAssetList(){
    return ax->dynamicCall("GetSFOBasisAssetList()").toString();
}
QString QKoa::getSFutureCodeByIndex(QString strBaseAssetCode, int nIndex){
    QVariantList params = {strBaseAssetCode, nIndex};
    return ax->dynamicCall("GetSFutureCodeByIndex(QString, int)", params).toString();
}
QString QKoa::getSFutureList(QString strBaseAssetCode){
    QVariantList params = {strBaseAssetCode};
    return ax->dynamicCall("GetSFutureList(QString)", params).toString();
}
QString QKoa::getSMonthList(QString strBaseAssetGb){
    QVariantList params = {strBaseAssetGb};
    return ax->dynamicCall("GetSMonthList(QString)", params).toString();
}
QString QKoa::getSOptionATM(QString strBaseAssetGb){
    QVariantList params = {strBaseAssetGb};
    return ax->dynamicCall("GetSOptionATM(QString)", params).toString();
}
QString QKoa::getSOptionCode(QString strBaseAssetGb, QString strActPrice, int nCp, QString strMonth){
    QVariantList params = {strBaseAssetGb, strActPrice, nCp, strMonth};
    return ax->dynamicCall("GetSOptionCode(QString, QString, int, QString)", params).toString();
}
QString QKoa::getSOptionCodeByActPrice(QString strBaseAssetGb, QString sTrCode, int nCp, int nTick){
    QVariantList params = {strBaseAssetGb, sTrCode, nCp, nTick};
    return ax->dynamicCall("GetSOptionCodeByActPrice(QString, QString, int, int)", params).toString();
}
QString QKoa::getSOptionCodeByMonth(QString strBaseAssetGb, QString sTrCode, int nCp, QString strMonth){
    QVariantList params = {strBaseAssetGb, sTrCode, nCp, strMonth};
    return ax->dynamicCall("GetSOptionCodeByMonth(QString, QString, int, QString)", params).toString();
}
QString QKoa::getThemeGroupCode(QString strThemeCode){
    QVariantList params = {strThemeCode};
    return ax->dynamicCall("GetThemeGroupCode(QString)", params).toString();
}
QString QKoa::getThemeGroupList(int nType){
    QVariantList params = {nType};
    return ax->dynamicCall("GetThemeGroupList(int)", params).toString();
}
QString QKoa::koa_Functions(QString sFunctionName, QString sParam){
    QVariantList params = {sFunctionName, sParam};
    return ax->dynamicCall("KOA_Functions(QString, QString)", params).toString();
}
int QKoa::sendCondition(QString strScrNo, QString strConditionName, int nIndex, int nSearch){
    QVariantList params = {strScrNo, strConditionName, nIndex, nSearch};
    return ax->dynamicCall("SendCondition(QString, QString, int, int)", params).toInt();
}
void QKoa::sendConditionStop(QString strScrNo, QString strConditionName, int nIndex){
    QVariantList params = {strScrNo, strConditionName, nIndex};
    ax->dynamicCall("SendConditionStop(QString, QString, int)", params);
}
int QKoa::sendOrder(QString sRQName, QString sScreenNo, QString sAccNo, int nOrderType, QString sCode, int nQty, int nPrice, QString sHogaGb, QString sOrgOrderNo){
    QVariantList params = {sRQName, sScreenNo, sAccNo, nOrderType, sCode, nQty, nPrice, sHogaGb, sOrgOrderNo};
    return ax->dynamicCall("SendOrder(QString, QString, QString, int, QString, int, int, QString, QString)", params).toInt();
}
int QKoa::sendOrderCredit(QString sRQName, QString sScreenNo, QString sAccNo, int nOrderType, QString sCode, int nQty, int nPrice, QString sHogaGb, QString sCreditGb, QString sLoanDate, QString sOrgOrderNo){
    QVariantList params = {sRQName, sScreenNo, sAccNo, nOrderType, sCode, nQty, nPrice, sHogaGb, sCreditGb, sLoanDate, sOrgOrderNo};
    return ax->dynamicCall("SendOrderCredit(QString, QString, QString, int, QString, int, int, QString, QString, QString, QString)", params).toInt();
}
int QKoa::sendOrderFO(QString sRQName, QString sScreenNo, QString sAccNo, QString sCode, int lOrdKind, QString sSlbyTp, QString sOrdTp, int lQty, QString sPrice, QString sOrgOrdNo){
    QVariantList params = {sRQName, sScreenNo, sAccNo, sCode, lOrdKind, sSlbyTp, sOrdTp, lQty, sPrice, sOrgOrdNo};
    return ax->dynamicCall("SendOrderFO(QString, QString, QString, QString, int, QString, QString, int, QString, QString)", params).toInt();
}
int QKoa::setInfoData(QString sInfoData){
    QVariantList params = {sInfoData};
    return ax->dynamicCall("SetInfoData(QString)", params).toInt();
}
void QKoa::setInputValue(QString sID, QString sValue){
    QVariantList params = {sID, sValue};
    ax->dynamicCall("SetInputValue(QString, QString)", params);
}
int QKoa::setOutputFID(QString sID){
    QVariantList params = {sID};
    return ax->dynamicCall("SetOutputFID(QString)", params).toInt();
}
int QKoa::setRealReg(QString strScreenNo, QString strCodeList, QString strFidList, QString strOptType){
    QVariantList params = {strScreenNo, strCodeList, strFidList, strOptType};
    return ax->dynamicCall("SetRealReg(QString, QString, QString, QString)", params).toInt();
}
void QKoa::setRealRemove(QString strScrNo, QString strDelCode){
    QVariantList params = {strScrNo, strDelCode};
    ax->dynamicCall("SetRealRemove(QString, QString)", params);
}
void QKoa::onEventConnect(int nErrCode){
    qDebug()<<"void QKoa::onEventConnect(int nErrCode)";
    qDebug()<<"nErrCode"<<nErrCode;
    QJsonObject obj;
    obj.insert("event","onEventConnect");
    obj.insert("nErrCode",nErrCode);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveChejanData(QString sGubun, int nItemCnt, QString sFIdList){
    qDebug()<<"void QKoa::onReceiveChejanData(QString sGubun, int nItemCnt, QString sFIdList)";
    qDebug()<<"sGubun"<<sGubun<<"nItemCnt"<<nItemCnt<<"sFIdList"<<sFIdList;
    QJsonObject obj;
    obj.insert("event","onReceiveChejanData");
    obj.insert("sGubun",sGubun);
    obj.insert("nItemCnt",nItemCnt);
    obj.insert("sFIdList",sFIdList);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveConditionVer(int lRet, QString sMsg){
    qDebug()<<"void QKoa::onReceiveConditionVer(int lRet, QString sMsg)";
    qDebug()<<"lRet"<<lRet<<"sMsg"<<sMsg;
    QJsonObject obj;
    obj.insert("event","onReceiveConditionVer");
    obj.insert("lRet",lRet);
    obj.insert("sMsg",sMsg);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveInvestRealData(QString sRealKey){
    qDebug()<<"void QKoa::onReceiveInvestRealData(QString sRealKey)";
    qDebug()<<"sRealKey"<<sRealKey;
    QJsonObject obj;
    obj.insert("event","onReceiveInvestRealData");
    obj.insert("sRealKey",sRealKey);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveMsg(QString sScrNo, QString sRQName, QString sTrCode, QString sMsg){
    qDebug()<<"void QKoa::onReceiveMsg(QString sScrNo, QString sRQName, QString sTrCode, QString sMsg)";
    qDebug()<<"sScrNo"<<sScrNo<<"sRQName"<<sRQName<<"sTrCode"<<sTrCode<<"sMsg"<<sMsg;
    QJsonObject obj;
    obj.insert("event","onReceiveMsg");
    obj.insert("sScrNo",sScrNo);
    obj.insert("sRQName",sRQName);
    obj.insert("sTrCode",sTrCode);
    obj.insert("sMsg",sMsg);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveRealCondition(QString sTrCode, QString strType, QString strConditionName, QString strConditionIndex){
    qDebug()<<"void QKoa::onReceiveRealCondition(QString sTrCode, QString strType, QString strConditionName, QString strConditionIndex)";
    qDebug()<<"sTrCode"<<sTrCode<<"strType"<<strType<<"strConditionName"<<strConditionName<<"strConditionIndex"<<strConditionIndex;
    QJsonObject obj;
    obj.insert("event","onReceiveRealCondition");
    obj.insert("sTrCode",sTrCode);
    obj.insert("strType",strType);
    obj.insert("strConditionName",strConditionName);
    obj.insert("strConditionIndex",strConditionIndex);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveRealData(QString sRealKey, QString sRealType, QString sRealData){
    //qDebug()<<"void QKoa::onReceiveRealData(QString sRealKey, QString sRealType, QString sRealData)";
    //qDebug()<<"sRealKey"<<sRealKey<<"sRealType"<<sRealType<<"sRealData"<<sRealData;
    QJsonObject obj;
    obj.insert("event","onReceiveRealData");
    obj.insert("sRealKey",sRealKey);
    obj.insert("sRealType",sRealType);
    obj.insert("sRealData",sRealData);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveTrCondition(QString sScrNo, QString strCodeList, QString strConditionName, int nIndex, int nNext){
    //qDebug()<<"void QKoa::onReceiveTrCondition(QString sScrNo, QString strCodeList, QString strConditionName, int nIndex, int nNext)";
    //qDebug()<<"sScrNo"<<sScrNo<<"strCodeList"<<strCodeList<<"strConditionName"<<strConditionName<<"nIndex"<<nIndex<<"nNext"<<nNext;
    QJsonObject obj;
    obj.insert("event","onReceiveTrCondition");
    obj.insert("sScrNo",sScrNo);
    obj.insert("strCodeList",strCodeList);
    obj.insert("strConditionName",strConditionName);
    obj.insert("nIndex",nIndex);
    obj.insert("nNext",nNext);
    emit(onMessageReceived(obj));
}
void QKoa::onReceiveTrData(QString sScrNo, QString sRQName, QString sTrCode, QString sRecordName, QString sPrevNext, int nDataLength, QString sErrorCode, QString sMessage, QString sSplmMsg){
    //qDebug()<<"void QKoa::onReceiveTrData(QString sScrNo, QString sRQName, QString sTrCode, QString sRecordName, QString sPrevNext, int nDataLength, QString sErrorCode, QString sMessage, QString sSplmMsg)";
    //qDebug()<<"sScrNo"<<sScrNo<<"sRQName"<<sRQName<<"sTrCode"<<sTrCode<<"sRecordName"<<sRecordName<<"sPrevNext"<<sPrevNext<<"nDataLength"<<nDataLength<<"sErrorCode"<<sErrorCode<<"sMessage"<<sMessage<<"sSplmMsg"<<sSplmMsg<<"\n";
    QJsonObject obj;
    obj.insert("event","onReceiveTrData");
    obj.insert("sScrNo",sScrNo);
    obj.insert("sRQName",sRQName);
    obj.insert("sTrCode",sTrCode);
    obj.insert("sRecordName",sRecordName);
    obj.insert("sPrevNext",sPrevNext);
    obj.insert("nDataLength",nDataLength);
    obj.insert("sErrorCode",sErrorCode);
    obj.insert("sMessage",sMessage);
    obj.insert("sSplmMsg",sSplmMsg);
    emit(onMessageReceived(obj));
}
void QKoa::onException(int code, QString source, QString disc, QString help){
    qDebug()<<"void QKoa::onException(int code, QString source, QString disc, QString help)";
    qDebug()<<"code"<<code<<"source"<<source<<"disc"<<disc<<"help"<<help;
    QJsonObject obj;
    obj.insert("event","onException");
    obj.insert("code",code);
    obj.insert("source",source);
    obj.insert("help",help);
    emit(onMessageReceived(obj));
}
void QKoa::onPropertyChanged(QString name){
    qDebug()<<"void QKoa::onPropertyChanged(QString name)";
    qDebug()<<"name"<<name;
    QJsonObject obj;
    obj.insert("event","onPropertyChanged");
    obj.insert("name",name);
    emit(onMessageReceived(obj));
}
void QKoa::onSignal(QString name, int argc, void *argv){
    //qDebug()<<"void QKoa::onSignal(QString name, int argc, void *argv)";
    //qDebug()<<"name"<<name<<"argc"<<argc<<"argv"<<argv;
    QJsonObject obj;
    obj.insert("event","onSignal");
    obj.insert("name",name);
    obj.insert("argc",argc);
    //emit(onMessageReceived(obj));
}
