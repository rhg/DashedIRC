#pragma once

#include <QObject>

#include <IrcConnection>
#include <IrcBuffer>
#include <IrcBufferModel>

#include "rep_connection_source.h"
#include "../messages.h"

class ConnectionManager : public ConnectionManagerSource {
    Q_OBJECT
    void handleMessage(int id, IrcMessage* message) {
        auto msg = Messages::parseMessage(message);
        if (msg != nullptr)
            emit messageReceived(id, *msg);
    }
    void handleBuffer(IrcBufferModel* model, IrcBuffer* buffer) {
        connect(buffer, &IrcBuffer::messageReceived,
                [this,model,buffer](IrcMessage* message){ handleMessage(model->indexOf(buffer), message); });
        qDebug() << "Connected " << buffer->title();
    }
public slots:
    void fromOpts(QVariantMap opts) override {
        auto conn = new IrcConnection(this);
        for (auto kv: opts.toStdMap()) {
            conn->setProperty(kv.first.toUtf8(), kv.second);
        }
        auto model = new IrcBufferModel(conn);
        model->setConnection(conn);
        auto core = new IrcBuffer(conn);
        connect(model, &IrcBufferModel::added,
                [this,model](IrcBuffer* buffer){ handleBuffer(model, buffer); });
        model->add(core);
        connect(model, &IrcBufferModel::messageIgnored,
                core, &IrcBuffer::receiveMessage);
        conn->open();
        qDebug() << "Formed connection";
    }
};
