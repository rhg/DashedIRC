#pragma once

#include "rep_manager_source.h"
#include "../ConnectionManager"

class ConnectionManagerAndroid : public ConnectionManagerAndroidSource
{
    Q_OBJECT
    ConnectionManager mManager;
public:
    explicit ConnectionManagerAndroid(QObject* parent = nullptr);
public slots:
    void fromOpts(QVariantMap opts) override;
};
