#include "manager.h"

ConnectionManagerAndroid::ConnectionManagerAndroid(QObject* parent) : ConnectionManagerAndroidSource(parent) {
    connect(&mManager, &ConnectionManager::nameChanged,
            [this](const QUuid id, QString title) {
        emit nameChanged(id, title);
    });
    connect(&mManager, &ConnectionManager::messageReceived,
            [this](const QUuid id, QString title, QVariantMap msg) {
        emit messageReceived(id, title, msg);
    });
    connect(&mManager, &ConnectionManager::invalidCommand,
            this, &ConnectionManagerAndroid::invalidCommand);
}

void ConnectionManagerAndroid::fromOpts(QVariantMap opts) {
    mManager.fromOpts(opts);
}

void ConnectionManagerAndroid::setCurrentBuffer(const QUuid bId) {
    mManager.setCurrentBuffer(bId);
}

void ConnectionManagerAndroid::sendCommand(const QString text) {
    mManager.sendCommand(text);
}
