QT       += core gui axcontainer sql network concurrent websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/qjsontreewidget.cpp \
    dispatcher.cpp \
    mainwidget.cpp \
    qflux/connectedwidget.cpp \
    qflux/dispatchercore.cpp \
    qflux/store/qjsonobjectmutex.cpp \
    servers/bmwsserver.cpp \
    tabs/assettab.cpp \
    tabs/conditiontab.cpp \
    tabs/fnctab.cpp \
    tabs/fnctab.cpp \
    main.cpp \
    tabs/trtab.cpp \
    utils/qhttputil.cpp \
    utils/qjsonutils.cpp \
    utils/qkoa.cpp \
    utils/qmysql.cpp \
    utils/qtimeutil.cpp \
    servers/qtvudfserver.cpp \

HEADERS += \
    actionTypes.h \
    components/qjsontreewidget.h \
    dispatcher.h \
    global.h \
    mainwidget.h \
    qflux/connectedwidget.h \
    qflux/dispatchercore.h \
    qflux/store/qjsonobjectmutex.h \
    servers/bmwsserver.h \
    tabs/assettab.h \
    tabs/conditiontab.h \
    tabs/fnctab.h \
    tabs/fnctab.h \
    tabs/trtab.h \
    utils/qhttputil.h \
    utils/qjsonutils.h \
    utils/qkoa.h \
    utils/qmysql.h \
    utils/qtimeutil.h \
    servers/qtvudfserver.h

INCLUDEPATH += $$PWD/qhttpserver/qhttpserver/src
#LIBS += -L$$PWD/qhttpserver/lib -lqhttpserverd
CONFIG += debug
LIBS += -L$$PWD/qhttpserver/lib -lqhttpserver

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    json.qrc
