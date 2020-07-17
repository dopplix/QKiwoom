#include "qhttputil.h"

QHttpUtil::QHttpUtil(){

}
QJsonObject QHttpUtil::urlToObj(QString urlStr){
    QJsonObject paramObj;
    if(urlStr.split("?").length()>1){
        QString paramStr = urlStr.split("?").at(1);
        QStringList paramStrList = paramStr.split("&");
        for(QString param : paramStrList){
            QString key = param.split("=").at(0);
            QString value = param.split("=").at(1);
            paramObj.insert(key,value);
        }
    }
    return paramObj;
}
