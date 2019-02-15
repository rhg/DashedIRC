#include "ConnectionManager"
#include "messages.h"

#include <QVariantMap>
#include <QDebug>
#include <QUuid>

#include <IrcConnection>
#include <IrcBuffer>
#include <IrcBufferModel>
#include <IrcMessage>

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent)
{

}

void ConnectionManager::fromOpts(QVariantMap opts) {
    auto id = QUuid::createUuid();
    auto conn = new IrcConnection(this);
    qDebug() << "Started Connection " << id;
    for (auto kv: opts.toStdMap()) {
        conn->setProperty(kv.first.toUtf8(), kv.second);
    }
    qDebug() << "Set options " << id;
    auto core = new IrcBuffer(conn);
    auto model = new IrcBufferModel(conn);
    qDebug() << "Created model and buffer " << id;
    model->setConnection(conn);
    qDebug() << "Set connection " << id;
    connect(model, &IrcBufferModel::messageIgnored,
            core, &IrcBuffer::receiveMessage);
    connect(model, &IrcBufferModel::added,
            [this](IrcBuffer* buffer) {
        auto bId = QUuid::createUuid();
        connect(buffer, &IrcBuffer::messageReceived,
                [this,buffer,bId](IrcMessage* message) {
            auto msg = Messages::parseMessage(message);
            if (msg != nullptr)
                emit messageReceived(bId, buffer->title(), *msg);
        });
    });
    qDebug() << "Made connections " << id;
    model->add(core);
    conn->open();
    qDebug() << "Opened connection " << id;
}
