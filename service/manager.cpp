#include "manager.h"

#include <IrcConnection>
#include <IrcBufferModel>
#include <IrcBuffer>

Buffer* Manager::fromOpts(QVariantMap opts) {
    auto conn = new IrcConnection(this);
    for (auto kv: opts.toStdMap()) {
        conn->setProperty(kv.first.toUtf8(), kv.second);
    }
    auto bufferModel = new IrcBufferModel(conn);
    bufferModel->setConnection(conn);
    auto buffer = new IrcBuffer(conn);
    bufferModel->add(buffer);

    return new Buffer(buffer, buffer);
}
