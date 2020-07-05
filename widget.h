#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAxWidget>
#include <QAxBase>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QFile>
#include "qkoa.h"
#include "qjsontreewidget.h"
#include "qjsonobjectmutex.h"

class Widget : public QWidget{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QJsonTreeWidget* functionTree = new QJsonTreeWidget();
    QListWidget* functionList = new QListWidget;
    QTableWidget* functionTable = new QTableWidget;
    QJsonTreeWidget* trTree = new QJsonTreeWidget();
    QListWidget* trList = new QListWidget;
    QTableWidget* trTable = new QTableWidget;
    QJsonTreeWidget* storeTree = new QJsonTreeWidget();
    QKoa* koa = new QKoa;
    QJsonArray fncDocArr;
    QJsonArray trDocArr;
    QJsonObjectMutex* store = new QJsonObjectMutex;

public slots:
    void test();
};
#endif // WIDGET_H
