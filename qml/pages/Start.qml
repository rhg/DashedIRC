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
            title: qsTr("About")
            showItem: showItemNever
            onClicked: about.open()
        }
    }
    ListModel {
        id: entries
        Component.onCompleted: {
            append({time: new Date(), nick: "Dashed IRC", content: qsTr("Welcome to Dashed IRC")})
            addBuffer('Core', entries)
        }
    }
    ColumnLayout {
        anchors.fill: parent
        transform: Translate {
            x: drawer.x + drawer.width
        }
        AppListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: entries
            delegate: SimpleRow {
                text: nick
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
                        text: nick.substring(0, 2).toLocaleLowerCase()
                        anchors.centerIn: parent
                    }
                }
            }
        }
        AppTextField {
            id: commandField
            Layout.fillWidth: true
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
            model: getBufferNames()
            delegate: SimpleRow {
                text: qsTr(item)
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
}
