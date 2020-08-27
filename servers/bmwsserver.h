#ifndef BMWSSERVER_H
#define BMWSSERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class BmWsServer : public QObject{
    Q_OBJECT

public:
    explicit BmWsServer(quint16 port, QObject *parent = nullptr);

private:
    QWebSocketServer* wsServer;
    QList<QWebSocket*> wsClients;
    QMap<QString,QWebSocket*> clientMap;
    QString getIdentifier(QWebSocket* peer);

public slots:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();
    void sendMessageToAllClient(QString message);
    void sendMessageToClient(QString name, QString message);
signals:
    void messageReceived(QString message);
};

#endif // BMWSSERVER_H
