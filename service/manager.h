#pragma once

#include "connection.h"
#include "rep_connection_source.h"

class Manager : public ManagerSource
{
    Q_OBJECT
public slots:
    Buffer* fromOpts(QVariantMap opts) override;
};
