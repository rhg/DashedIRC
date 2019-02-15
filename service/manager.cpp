#include "manager.h"

#include <QUuid>

ConnectionManagerAndroid::ConnectionManagerAndroid(QObject* parent) : ConnectionManagerAndroidSource(parent) {
    connect(&mManager, &ConnectionManager::nameChanged,
            [this](const QUuid id, QString title) {
        emit nameChanged(id, title);
    });
    connect(&mManager, &ConnectionManager::messageReceived,
            [this](const QUuid id, QString title, QVariantMap msg) {
        emit messageReceived(id, title, msg);
    });
}

void ConnectionManagerAndroid::fromOpts(QVariantMap opts) {
    mManager.fromOpts(opts);
}
