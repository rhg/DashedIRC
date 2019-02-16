import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Dialogs 1.2 as D

Page {
    title: qsTr('Servers')
    rightBarItem: IconButtonBarItem {
        icon: IconType.plus
        onClicked: navigationStack.push(Qt.resolvedUrl('Server.qml'))
    }
    function deleteServer(i, name) {
        servers.splice(i, 1)
        settings.setValue('servers', servers)
        settings.clearValue("server." + name)
        serversChanged()
    }
    function shareServer(name) {
        var s = JSON.stringify({name: name, server: settings.getValue("server." + name)})
        if (Qt.platform.os === 'android') {
            nativeUtils.share(s, "")
        } else {
            fD.s = s
            fD.open()
        }
    }
    D.FileDialog {
        id: fD
        folder: shortcuts.documents
        selectExisting: false
        nameFilters: qsTr('Dashed IRC Server (*.dircs)')
        onRejected: fD.close()
        onAccepted: {
            fileUtils.writeFile(fileUrl, s)
            fD.close()
        }
        property var s
    }

    AppListView {
        anchors.fill: parent
        model: servers
        delegate: SwipeOptionsContainer {
            id: container
            SimpleRow {
                id: row
                text: item
                onSelected: navigationStack.push(Qt.resolvedUrl('Server.qml'), {
                                                     name: item
                                                 })
            }
            leftOption: SwipeButton {
                height: row.height
                icon: IconType.share
                text: qsTr('Share')
                onClicked: {
                    container.hideOptions()
                    shareServer(row.item)
                }
            }
            rightOption: SwipeButton {
                height: row.height
                icon: IconType.trash
                text: qsTr('Delete')
                onClicked: deleteServer(index, row.item)
            }
        }
    }
    FloatingActionButton {
        id: fab
        icon: IconType.bolt
        onClicked: navigationStack.push(Qt.resolvedUrl('Import.qml'))
    }
}
