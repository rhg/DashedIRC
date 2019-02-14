#include "messages.h"

#include <QDebug>

Messages::Messages(QObject *parent) : QObject(parent)
{

}

QVariantMap* Messages::parseMessage(IrcMessage* message) {
    QString content;
    switch (message->type()) {
    case IrcMessage::Capability: {
        auto msg = reinterpret_cast<IrcCapabilityMessage*>(message);
        content = msg->subCommand() + " " + msg->capabilities().join('\n');
        break;
    }
    default:
        qDebug() << "Unhandled type: " << message->type();
        return nullptr;
    }
    auto ret = new QVariantMap();
    ret->insert("time", message->timeStamp());
    ret->insert("nick", message->nick());
    ret->insert("content", content);
    return ret;
}
