#ifndef BMWSSERVER_H
#define BMWSSERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

class BmWsServer : public QObject{
    Q_OBJECT

public:
    explicit BmWsServer(quint16 port, QObject *parent = nullptr);

private:
    QWebSocketServer* wsServer;
    QList<QWebSocket*> wsClients;
    QString getIdentifier(QWebSocket* peer);

public slots:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();
    void sendMessageToAllClient(QString message);

signals:
    void messageReceived(QString message);
};

#endif // BMWSSERVER_H
