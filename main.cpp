#include "mainwidget.h"
#include <QApplication>
#include <QJsonArray>
#include "utils/qjsonutils.h"
#include "dispatcher.h"

Dispatcher* dispatcher;
QJsonArray trDocArr;
QJsonArray fncDocArr;
QJsonObject krMapObj;
QJsonObject fidDocObj;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    dispatcher = new Dispatcher;
    trDocArr = QJsonUtils::readJsonArrFromFile(":/doc/json/tr.json");
    fncDocArr= QJsonUtils::readJsonArrFromFile(":/doc/json/functions.json");
    krMapObj = QJsonUtils::readJsonObjFromFile(":/doc/json/krmap.json");
    fidDocObj= QJsonUtils::readJsonObjFromFile(":/doc/json/fid.json");
    MainWidget w;
    dispatcher->connectWidget(&w);
    w.show();
    return a.exec();
}
