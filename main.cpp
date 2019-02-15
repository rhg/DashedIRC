#include <QApplication>
#include <QDebug>
#include <QUuid>
#include <VPApplication>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QAndroidService>
#include <QRemoteObjectNode>
#include "rep_connection_replica.h"
#else
#include "ConnectionManager"
#endif

#include <QQmlContext>
#include <QQmlApplicationEngine>

// uncomment this line to add the Live Client Module and use live reloading with your custom C++ code
#include <VPLiveClient>

int service(int argc, char** argv);

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    if (argc == 2) {
        return service(argc, argv);
    }
#endif
    QApplication app(argc, argv);
    VPApplication vplay;

    // Use platform-specific fonts instead of V-Play's default font
    vplay.setPreservePlatformFonts(true);

    QQmlApplicationEngine engine;
    vplay.initialize(&engine);
#ifdef Q_OS_ANDROID
    auto accent = QAndroidJniObject::callStaticObjectMethod("com/rhg135/dashedirc/Colors",
                                                            "getAccent",
                                                            "(Landroid/content/Context;)Ljava/lang/String;",
                                                            QtAndroid::androidActivity().object());
    engine.rootContext()->setContextProperty(QLatin1Literal("androidAccent"), accent.toString());

    QAndroidJniObject::callStaticMethod<void>("com/rhg135/dashedirc/Service",
                                              "startService",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidActivity().object());
    QRemoteObjectNode repNode;
    repNode.connectToNode(QUrl(QStringLiteral("local:switch")));
    QScopedPointer<ConnectionManagerReplica> ptr;
    ptr.reset(repNode.acquire<ConnectionManagerReplica>());
    engine.rootContext()->setContextProperty(QLatin1Literal("connectionManager"),
                                             ptr.data());
#else
    ConnectionManager manager;
    engine.rootContext()->setContextProperty(QLatin1Literal("connectionManager"),
                                             &manager);
#endif
    auto coreId = QUuid::createUuid();
    engine.rootContext()->setContextProperty(QLatin1Literal("coreUuid"),
                                             coreId);
    // use this during development
    // for PUBLISHING, use the entry point below
    // vplay.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    // use this instead of the above call to avoid deployment of the qml files and compile them into the binary with qt's resource system qrc
    // this is the preferred deployment option for publishing games to the app stores, because then your qml files and js files are protected
    // to avoid deployment of your qml files and images, also comment the DEPLOYMENTFOLDERS command in the .pro file
    // also see the .pro file for more details
    // vplay.setMainQmlFileName(QStringLiteral("qrc:/qml/Main.qml"));

    // engine.load(QUrl(vplay.mainQmlFileName()));

    // to start your project as Live Client, comment (remove) the lines "vplay.setMainQmlFileName ..." & "engine.load ...",
    // and uncomment the line below
    VPlayLiveClient client (&engine);

    return app.exec();
}
