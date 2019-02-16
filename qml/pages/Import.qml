import Felgo 3.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

Page {
    title: qsTr('Import')
    function save() {
        var o = JSON.parse(ati.text)
        settings.setValue("server." + o.name, o.server)
        addServer(o.name)
        navigationStack.pop()
    }
    ColumnLayout {
        anchors.fill: parent
        AppTextInput {
            id: ati
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        AppButton {
            icon: IconType.check
            text: qsTr('Save')
            enabled: ati.text
            Layout.alignment: Qt.AlignRight
            onClicked: save()
        }
    }
}
