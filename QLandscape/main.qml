import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import FileIO 1.0
import DrawMap 1.0

ApplicationWindow {
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

    FileIO {
        id: fileIO
        onError: console.log(msg)
    }

    DrawMap {
        id: drawMap
        onError: console.log(msg)
    }

    function openFile(fileUrl) {
        return fileIO.openFile(fileUrl)
    }

    function saveFile(fileUrl, text) {
        return fileIO.saveFile(fileUrl, text)
    }

    function redrawMap() {
        //TODO: draw different size maps
        var imageBase64 = drawMap.generateMapFromFile(mapFileText.text)
        mapImage.source = "data:image/png;base64," + imageBase64
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open")
                onTriggered: fileDialog.open()
            }
            MenuItem { text: qsTr("Close") }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a file")
        folder: shortcuts.home
        selectMultiple: false
        onAccepted: {
            var path = this.fileUrl.toString()
            path = path.replace(/^(file:\/{2})/,"")
            mapFileText.text = path
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    Rectangle {
        height: parent.height
        width: 560
        anchors.left: parent.left
        anchors.top: parent.top

        TextField {
            id: mapFileText
            anchors {
                left: parent.left
                leftMargin: 10
                top: parent.top
                topMargin: 5
            }
            width: 200
            onTextChanged: {
                redrawMap()
            }
        }

        Button {
            anchors {
                left: mapFileText.right
                top: parent.top
                topMargin: 5
            }
            text: "..."
            width: 30
            onClicked: {
                fileDialog.open()
            }
        }

        Image {
            id: mapImage
            width: 512
            height: 512
            anchors {
                left: parent.left
                leftMargin: 20
                top: parent.top
                topMargin: 40
            }
            source: ""
        }
    }
    //TODO: call extern application to generate map, other design
}
