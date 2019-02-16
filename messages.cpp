#include "messages.h"

#include <QDebug>

Messages::Messages(QObject *parent) : QObject(parent)
{

}

QString parseNumeric(IrcNumericMessage* msg) {
    switch (msg->code()) {
    case 1:
    case 2:
    case 3:
        return msg->parameter(1);
    default:
        qDebug() << "Unhandled numeric " << msg->code();
        return msg->parameters().join(" ");
    }
}

QVariantMap* Messages::parseMessage(IrcMessage* message) {
    QString content;
    switch (message->type()) {
    case IrcMessage::Capability: {
        auto msg = reinterpret_cast<IrcCapabilityMessage*>(message);
        content = msg->subCommand() + " " + msg->capabilities().join(' ');
        break;
    }
    case IrcMessage::Numeric: {
        auto msg = reinterpret_cast<IrcNumericMessage*>(message);
        if (!msg->isComposed()) {
            content = parseNumeric(msg);
        }
        break;
    }
    case IrcMessage::Motd: {
        auto msg = reinterpret_cast<IrcMotdMessage*>(message);
        content = msg->lines().join('\n');
        break;
    }
    case IrcMessage::Mode: {
        auto msg = reinterpret_cast<IrcModeMessage*>(message);
        content = msg->target() + " = " + msg->mode();
        break;
    }
    case IrcMessage::Topic: {
        auto msg = reinterpret_cast<IrcTopicMessage*>(message);
        content = "Topic is '" + msg->topic() + "'";
        break;
    }
    case IrcMessage::Private: {
        auto msg = reinterpret_cast<IrcPrivateMessage*>(message);
        content = msg->content();
        break;
    }
    case IrcMessage::Notice: {
        auto msg = reinterpret_cast<IrcNoticeMessage*>(message);
        content = "!" + msg->content();
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
