#pragma once

#include <QObject>

#include <IrcMessage>

class Messages : public QObject
{
    Q_OBJECT
public:
    explicit Messages(QObject *parent = nullptr);
    Q_INVOKABLE static QVariantMap* parseMessage(IrcMessage* msg);
};
