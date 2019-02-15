#include <QAndroidService>
#include <QDebug>
#include <QRemoteObjectHost>

#include "manager.h"

int service(int argc, char** argv) {
    QAndroidService service(argc, argv);
    qDebug() << "Running Service";

    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:switch")));
    ConnectionManagerAndroid manager;
    srcNode.enableRemoting(&manager);

    return service.exec();
}
