#ifndef QKOA_H
#define QKOA_H

#include <QObject>
#include <QAxWidget>
#include <QDebug>
#include <QJsonObject>

class QKoa : public QObject{

    Q_OBJECT

public:
    explicit QKoa(QObject *parent = nullptr);
    int commConnect();
    QString commGetData(QString sJongmokCode, QString sRealType, QString sFieldName, int nIndex, QString sInnerFieldName);
    int commInvestRqData(QString sMarketGb, QString sRQName, QString sScreenNo);
    int commKwRqData(QString sArrCode, int bNext, int nCodeCount, int nTypeFlag, QString sRQName, QString sScreenNo);
    int commRqData(QString sRQName, QString sTrCode, int nPrevNext, QString sScreenNo);
    void commTerminate();
    void disconnectRealData(QString sScnNo);
    QString getAPIModulePath();
    QString getActPriceList();
    QString getBranchCodeName();
    QString getChejanData(int nFid);
    QString getCodeListByMarket(QString sMarket);
    QString getCommData(QString strTrCode, QString strRecordName, int nIndex, QString strItemName);
    QVariant getCommDataEx(QString strTrCode, QString strRecordName);
    QString getCommRealData(QString sTrCode, int nFid);
    int getConditionLoad();
    QString getConditionNameList();
    int getConnectState();
    int getDataCount(QString strRecordName);
    QString getFutureCodeByIndex(int nIndex);
    QString getFutureList();
    QString getLoginInfo(QString sTag);
    int getMarketType(QString sTrCode);
    QString getMasterCodeName(QString sTrCode);
    QString getMasterConstruction(QString sTrCode);
    QString getMasterLastPrice(QString sTrCode);
    int getMasterListedStockCnt(QString sTrCode);
    QString getMasterListedStockDate(QString sTrCode);
    QString getMasterStockState(QString sTrCode);
    QString getMonthList();
    QString getOptionATM();
    QString getOptionCode(QString strActPrice, int nCp, QString strMonth);
    QString getOptionCodeByActPrice(QString sTrCode, int nCp, int nTick);
    QString getOptionCodeByMonth(QString sTrCode, int nCp, QString strMonth);
    QString getOutputValue(QString strRecordName, int nRepeatIdx, int nItemIdx);
    int getRepeatCnt(QString sTrCode, QString sRecordName);
    QString getSActPriceList(QString strBaseAssetGb);
    QString getSFOBasisAssetList();
    QString getSFutureCodeByIndex(QString strBaseAssetCode, int nIndex);
    QString getSFutureList(QString strBaseAssetCode);
    QString getSMonthList(QString strBaseAssetGb);
    QString getSOptionATM(QString strBaseAssetGb);
    QString getSOptionCode(QString strBaseAssetGb, QString strActPrice, int nCp, QString strMonth);
    QString getSOptionCodeByActPrice(QString strBaseAssetGb, QString sTrCode, int nCp, int nTick);
    QString getSOptionCodeByMonth(QString strBaseAssetGb, QString sTrCode, int nCp, QString strMonth);
    QString getThemeGroupCode(QString strThemeCode);
    QString getThemeGroupList(int nType);
    QString koa_Functions(QString sFunctionName, QString sParam);
    int sendCondition(QString strScrNo, QString strConditionName, int nIndex, int nSearch);
    void sendConditionStop(QString strScrNo, QString strConditionName, int nIndex);
    int sendOrder(QString sRQName, QString sScreenNo, QString sAccNo, int nOrderType, QString sCode, int nQty, int nPrice, QString sHogaGb, QString sOrgOrderNo);
    int sendOrderCredit(QString sRQName, QString sScreenNo, QString sAccNo, int nOrderType, QString sCode, int nQty, int nPrice, QString sHogaGb, QString sCreditGb, QString sLoanDate, QString sOrgOrderNo);
    int sendOrderFO(QString sRQName, QString sScreenNo, QString sAccNo, QString sCode, int lOrdKind, QString sSlbyTp, QString sOrdTp, int lQty, QString sPrice, QString sOrgOrdNo);
    int setInfoData(QString sInfoData);
    void setInputValue(QString sID, QString sValue);
    int setOutputFID(QString sID);
    int setRealReg(QString strScreenNo, QString strCodeList, QString strFidList, QString strOptType);
    void setRealRemove(QString strScrNo, QString strDelCode);

    QAxWidget* ax = new QAxWidget("KHOPENAPI.KHOpenAPICtrl.1");

public slots:
    void onEventConnect(int nErrCode);
    void onReceiveChejanData(QString sGubun, int nItemCnt, QString sFIdList);
    void onReceiveConditionVer(int lRet, QString sMsg);
    void onReceiveInvestRealData(QString sRealKey);
    void onReceiveMsg(QString sScrNo, QString sRQName, QString sTrCode, QString sMsg);
    void onReceiveRealCondition(QString sTrCode, QString strType, QString strConditionName, QString strConditionIndex);
    void onReceiveRealData(QString sRealKey, QString sRealType, QString sRealData);
    void onReceiveTrCondition(QString sScrNo, QString strCodeList, QString strConditionName, int nIndex, int nNext);
    void onReceiveTrData(QString sScrNo, QString sRQName, QString sTrCode, QString sRecordName, QString sPrevNext, int nDataLength, QString sErrorCode, QString sMessage, QString sSplmMsg);
    void onException(int code, QString source, QString disc, QString help);
    void onPropertyChanged(QString name);
    void onSignal(QString name, int argc, void* argv);

signals:
    void onMessageReceived(QJsonObject obj);
};

#endif // QKOA_H
