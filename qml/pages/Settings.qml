import VPlayApps 1.0
import QtQuick 2.0

ListPage {
    title: qsTr('Settings')
    model: [{type: 'IRC', icon: IconType.bolt, text: qsTr('Servers'), page: 'Servers.qml'}]
    section.property: "type"
    delegate: SimpleRow {
        onSelected: navigationStack.push(Qt.resolvedUrl(item.page))
    }
}
