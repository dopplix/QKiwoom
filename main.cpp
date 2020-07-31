#include "mainwidget.h"
#include <QApplication>
#include <QJsonArray>
#include "utils/qjsonutils.h"
#include "dispatcher.h"
#include "global.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWidget w;
    dispatcher->connectWidget(&w);
    w.show();
    return a.exec();
}
