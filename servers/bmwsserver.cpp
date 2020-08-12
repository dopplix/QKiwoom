#include "bmwsserver.h"
#include "global.h"
#include <QDebug>

BmWsServer::BmWsServer(quint16 port, QObject *parent) : QObject(parent){
    wsServer = new QWebSocketServer("BmServer",QWebSocketServer::NonSecureMode,this);
    wsServer->listen(QHostAddress::Any,port);
    connect(wsServer, &QWebSocketServer::newConnection,this, &BmWsServer::onNewConnection);
}
QString BmWsServer::getIdentifier(QWebSocket *client){
    QString address = client->peerAddress().toString();
    QString port = QString::number(client->peerPort());
    return address+":"+port;
}
void BmWsServer::onNewConnection(){
    auto client = wsServer->nextPendingConnection();
    qDebug()<<getIdentifier(client)<<" Connected";
    client->setParent(this);
    connect(client, &QWebSocket::textMessageReceived, this, &BmWsServer::processMessage);
    connect(client, &QWebSocket::disconnected, this, &BmWsServer::socketDisconnected);
    wsClients.append(client);
}
void BmWsServer::processMessage(QString message){
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug()<<getIdentifier(client)<<message;
    emit(messageReceived(message));
}
void BmWsServer::socketDisconnected(){
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug()<<getIdentifier(client)<<" Disconnected";
    if(client){
        wsClients.removeAll(client);
        client->deleteLater();
    }
}
void BmWsServer::sendMessageToAllClient(QString message){
    qDebug()<<"void BmWsServer::sendMessageToAllClient(QString message)";
    for(int i=0;i<wsClients.size();i++){
        QWebSocket* client = wsClients.at(i);
        client->sendTextMessage(message);
    }
}
