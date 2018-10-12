import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0

Window {
    property int windowWidth: 800
    property int windowHeight: 600
    visible: true
    width: windowWidth
    minimumWidth: windowWidth
    maximumWidth: windowWidth
    height: windowHeight
    minimumHeight: windowHeight
    maximumHeight: windowHeight
    title: qsTr("Landscape GUI")

    Rectangle {
        height: parent.height
        width: 560
        anchors.left: parent.left
        anchors.top: parent.top
        color: "#EEEEEE"

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: shortcuts.home
            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                Qt.quit()
            }
            onRejected: {
                console.log("Canceled")
                Qt.quit()
            }
//            Component.onCompleted: visible = true
        }

        //TODO: text field + button to open dialog
        //TODO: draw map with http://doc.qt.io/qt-5/qml-qtquick-context2d.html
    }
    //TODO: call extern application to generate map, other design
}
