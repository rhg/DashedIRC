#include <QAndroidService>
#include <QDebug>
#include <QRemoteObjectHost>

#include "connection.h"

int service(int argc, char** argv) {
    QAndroidService service(argc, argv);
    qDebug() << "Running Service";

    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:switch")));
    ConnectionManager manager;
    srcNode.enableRemoting(&manager);

    return service.exec();
}
