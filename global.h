#ifndef GLOBAL_H
#define GLOBAL_H

#include "dispatcher.h"
#include "actionTypes.h"
#include "utils/qjsonutils.h"
#include "utils/qkoa.h"
#include "utils/qtimeutil.h"

extern Dispatcher* dispatcher;
extern QJsonArray trDocArr;
extern QJsonArray fncDocArr;
extern QJsonObject krMapObj;

#endif // GLOBAL_H
