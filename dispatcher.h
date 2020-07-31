#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include "qflux/dispatchercore.h"

class Dispatcher : public DispatcherCore{

public:
    Dispatcher();

public slots:
    void dispatch(QString actionType, QJsonObject payload) override;

};

#endif // DISPATCHER_H
