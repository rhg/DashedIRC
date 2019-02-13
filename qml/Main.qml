import VPlayApps 1.0
import QtQuick 2.0
import "pages"

App {
    // You get free licenseKeys from https://v-play.net/licenseKey
    // With a licenseKey you can:
    //  * Publish your games & apps for the app stores
    //  * Remove the V-Play Splash Screen or set a custom one (available with the Pro Licenses)
    //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
    //licenseKey: "<generate one from https://v-play.net/licenseKey>"
    onInitTheme: {
        if (typeof androidAccent !== 'undefined') {
            Theme.colors.tintColor = androidAccent
        }
    }

    Component.onCompleted: updateServers()
    property var buffers: ({})
    property var servers
    function updateServers() {
        servers = settings.getValue('servers') || []
    }
    function addBuffer(name, model) {
        buffers[name] = model
        buffersChanged()
    }
    function getBufferNames() {
        var ret = []
        for (var k in buffers) {
            ret.push(k)
        }
        console.log(ret)
        return ret
    }
    NavigationStack {
        Start {}
    }
}
