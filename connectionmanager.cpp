#include "ConnectionManager"
#include "messages.h"

#include <QVariantMap>
#include <QDebug>
#include <QUuid>

#include <IrcConnection>
#include <IrcBuffer>
#include <IrcBufferModel>
#include <IrcMessage>
#include <IrcCommand>

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent)
{
    mParser.setTriggers(QStringList() << "/");
    mParser.setChannels(QStringList());
    mParser.addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
}

void ConnectionManager::sendCommand(QString text) {
    auto cmd = mParser.parse(text);
    if (cmd == nullptr) {
        if (text.startsWith("//")) {
            cmd = IrcCommand::createMessage(mParser.target(), text);
        } else if (text.startsWith("/")) {
            emit invalidCommand();
            return;
        } else {
            cmd = IrcCommand::createMessage(mParser.target(), text);
        }
    }
    mBuffers[mCurrentBuffer]->sendCommand(cmd);
}

void ConnectionManager::setCurrentBuffer(const QUuid bId, QString name) {
    mParser.setTarget(name);
    mCurrentBuffer = bId;
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
        mBuffers.insert(bId, buffer);
        if (buffer->isChannel())
            mParser.setChannels(mParser.channels() << buffer->title());
        connect(buffer, &IrcBuffer::messageReceived,
                [this,buffer,bId](IrcMessage* message) {
            auto msg = Messages::parseMessage(message);
            if (msg != nullptr)
                emit messageReceived(bId, buffer->title(), *msg);
        });
        connect(buffer, &IrcBuffer::titleChanged,
                [this,bId](QString title) {
            emit nameChanged(bId, title);
        });
    });
    connect(conn, &IrcConnection::displayNameChanged,
            core, &IrcBuffer::setName);
    connect(conn, &IrcConnection::connected,
            [conn]() {
        conn->sendCommand(IrcCommand::createCapability("REQ", "server-time"));
    });
    qDebug() << "Made connections " << id;
    core->setName(conn->displayName());
    model->add(core);
    conn->open();
    qDebug() << "Opened connection " << id;
    mConnections.insert(id, conn);
}
