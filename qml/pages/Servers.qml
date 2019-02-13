import VPlayApps 1.0
import QtQuick 2.0

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
    AppListView {
        anchors.fill: parent
        model: servers
        delegate: SwipeOptionsContainer {
            SimpleRow {
                id: row
                text: item
                onSelected: navigationStack.push(Qt.resolvedUrl('Server.qml'), {
                                                     name: item
                                                 })
            }
            rightOption: SwipeButton {
                height: row.height
                icon: IconType.trash
                text: qsTr('Delete')
                onClicked: deleteServer(index, row.item)
            }
        }
    }

}
