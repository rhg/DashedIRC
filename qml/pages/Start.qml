import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

Page {
    title: qsTr("Core")
    leftBarItem: IconButtonBarItem {
        icon: IconType.bars
        title: qsTr("Drawer")
        rotation: (1 + drawer.x / drawer.width) * 90
        onClicked: drawer.toggle()
    }
    rightBarItem: NavigationBarRow {
        IconButtonBarItem {
            title: qsTr('Connect')
            showItem: showItemNever
            onClicked: connect.open()
        }
        IconButtonBarItem {
            title: qsTr('Settings')
            onClicked: navigationStack.push(Qt.resolvedUrl('Settings.qml'))
            showItem: showItemNever
        }
        IconButtonBarItem {
            title: qsTr("About")
            showItem: showItemNever
            onClicked: about.open()
        }
    }
    function gotMessage(bId, bufferTitle, obj) {
        if (obj.content) {
            var data = getBuffer(bId)
            if (!data) {
                var c = Qt.createComponent("../models/Buffer.qml")
                var model = c.createObject(connectionManager)
                model.append(obj)
                var o = {id: bId, title: bufferTitle, model: model}
                addBuffer(bId, o)
            } else {
                data.model.append(obj)
            }
        }
    }
    function show(id, item) {
        if (id) connectionManager.setCurrentBuffer(id, item.title)
        title = item.title || '?'
        spotlight.model = item.model
    }
    function setTitle(id, title) {
        var data = getBuffer(id)
        if (data) {
            data.title = title
        }
    }
    Connections {
        target: connectionManager
        onNameChanged: setTitle(id, title)
        onInvalidCommand: {
            var o = {time: new Date(), nick: 'Dashed IRC', content: qsTr('Invalid Command')}
            entries.append(o)
        }
    }
    ListModel {
        id: entries
        Component.onCompleted: {
            append({time: new Date(), nick: "Dashed IRC", content: qsTr("Welcome to Dashed IRC")})
            addBuffer(coreUuid, {title: qsTr('Core'), model: entries})
        }
    }
    ColumnLayout {
        anchors.fill: parent
        transform: Translate {
            x: drawer.x + drawer.width
        }
        AppListView {
            id: spotlight
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: entries
            delegate: SimpleRow {
                text: nick || "?"
                detailText: content
                textItem.x: Theme.listItem.spacing + defaultAvatar.width
                detailTextItem.x: textItem.x
                Rectangle {
                    id: defaultAvatar
                    width: dp(48)
                    height: width
                    radius: height / 2
                    color: Theme.tintColor
                    x: Theme.listItem.spacing
                    y: x
                    AppText {
                        color: Theme.navigationBar.titleColor
                        text: nick.substring(0, 2).toLocaleLowerCase() || "?"
                        anchors.centerIn: parent
                    }
                }
            }
        }
        AppTextField {
            id: commandField
            Layout.fillWidth: true
            onAccepted: {
                if (text) {
                    connectionManager.sendCommand(text)
                    text = ""
                }
            }
        }
    }
    AppDrawer {
        id: drawer
        width: parent.width * 0.66
        height: width
        Rectangle {
            color: Theme.backgroundColor
            anchors.fill: parent
        }
        AppListView {
            id: bufferList
            anchors.fill: parent
            model: getBufferData()
            delegate: SimpleRow {
                text: item.title || "?"
                onSelected: show(item.id, item)
            }
        }
    }
    Dialog {
        id: about
        title: qsTr('About')
        positiveAction: false
        negativeAction: false
        onCanceled: about.close()
        GridLayout {
            columns: 2
            anchors.centerIn: parent
            AppText {
                text: qsTr('Version:')
                Layout.alignment: Qt.AlignRight
            }
            AppText {
                text: '0.3.0-git'
            }
            AppText {
                text: qsTr('Founder:')
                Layout.alignment: Qt.AlignRight
            }
            AppText {
                text: 'Ricardo Gomez'
            }
        }
    }
    Dialog {
        id: connect
        title: qsTr('Connect')
        negativeAction: false
        positiveAction: false
        onCanceled: connect.close()
        AppListView {
            anchors.fill: parent
            model: servers
            delegate: SimpleRow {
                text: item
                onSelected: {
                    connect.close()
                    if (typeof connectionManager !== 'undefined') {
                        connectionManager.messageReceived.connect(gotMessage)
                        connectionManager.fromOpts(settings.getValue("server." + item))
                    }
                }
            }
        }
    }
}
