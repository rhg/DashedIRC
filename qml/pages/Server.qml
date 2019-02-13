import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

Page {
    title: nameField.text ? nameField.text : qsTr('Add Server')
    property string name
    readonly property var server: settings.getValue('server.' + nameField.text) || {}
    function saveAndPop() {
        var o = {}
        o.host = hostField.text
        o.port = portField.port
        if (passwordField.text) o.password = passwordField.text
        o.secure = sslBox.checked
        o.userName = unField.text
        o.nickName = nnField.text
        o.realName = rnField.text
        settings.setValue("server." + nameField.text, o)
        var l = settings.getValue('servers') || []
        if (!l.find(function(el) { return el === nameField.text })) {
            l.push(nameField.text)
            settings.setValue('servers', l)
        }
        updateServers()
        navigationStack.pop()
    }
    GridLayout {
        anchors.fill: parent
        columns: 2
        AppText {
            text: qsTr('Name:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: nameField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            visible: !name
            text: name
            Layout.preferredWidth: dp(200)
        }
        AppText {
            text: qsTr('Host:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: hostField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            text: server.host || ""
            Layout.preferredWidth: dp(200)
        }
        AppText {
            text: qsTr('SSL:')
            Layout.alignment: Qt.AlignRight
        }
        AppCheckBox {
            id: sslBox
            checked: server.secure || false
        }
        AppText {
            text: qsTr('Port:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: portField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            text: server.port || (sslBox.checked ? '6697' : '6667')
            Layout.preferredWidth: dp(200)
            validator: IntValidator {bottom: 1; top: 65535}
            property int port: parseInt(text)
        }
        AppText {
            text: qsTr('Password:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: passwordField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            text: server.password || ""
            echoMode: TextInput.Password
            Layout.preferredWidth: dp(200)
        }
        AppText {
            text: qsTr('User Name:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: unField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            text: server.userName || "dirc"
            Layout.preferredWidth: dp(200)
        }
        AppText {
            text: qsTr('Nick Name:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: nnField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            text: server.nickName || ""
            Layout.preferredWidth: dp(200)
        }
        AppText {
            text: qsTr('Real Name:')
            Layout.alignment: Qt.AlignRight
        }
        AppTextField {
            id: rnField
            borderColor: Theme.tintColor
            borderWidth: dp(2)
            text: server.realName || "Dashed IRC 0.3.0-git"
            Layout.preferredWidth: dp(200)
        }
        AppButton {
            icon: IconType.save
            text: qsTr('Save')
            enabled: nameField.text && hostField.text && portField.port && nnField.text
            onClicked: saveAndPop()
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
        }
    }
}
