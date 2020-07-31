#include "global.h"

Dispatcher* dispatcher = new Dispatcher;
QJsonArray trDocArr = QJsonUtils::readJsonArrFromFile(":/doc/json/tr.json");
QJsonArray fncDocArr = QJsonUtils::readJsonArrFromFile(":/doc/json/functions.json");
QJsonObject krMap = QJsonUtils::readJsonObjFromFile(":/doc/json/krmap.json");
