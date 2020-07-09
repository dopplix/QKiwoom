#ifndef QJSONTREEWIDGET_H
#define QJSONTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class QJsonTreeWidget : public QTreeWidget {

public:
    QJsonTreeWidget();
    void appendJson(QTreeWidgetItem* item, QJsonObject& obj);
    void appendJson(QTreeWidgetItem* item, QJsonArray& arr);
    void appendJson(QJsonObject& obj);
    void appendJson(QJsonArray& arr);

private:
    QTreeWidgetItem* rootItem = QTreeWidget::invisibleRootItem();

};

#endif // QJSONTREEWIDGET_H
