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
            MenuItem {
                text: qsTr("Close")
                onTriggered: Qt.quit()
            }
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
        id: mapRect
        height: parent.height
        width: 522
        anchors.left: parent.left
        anchors.top: parent.top

        TextField {
            id: mapFileText
            anchors {
                left: parent.left
                leftMargin: 5
                top: parent.top
                topMargin: 5
            }
            width: 200
            text: ""
            onTextChanged: {
                redrawMap()
            }
            Component.onCompleted: this.text = "../output/1.map" //run on load
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
                leftMargin: 5
                top: parent.top
                topMargin: 40
            }
            source: ""

            //TODO: save map as image
        }
    }
    Rectangle {
        height: parent.height
        anchors.left: mapRect.right
        anchors.right: parent.right
        anchors.top: parent.top
        color: "#CCCCCC"

        Text {
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 20
            text: qsTr("Algorithm:")
        }

        ComboBox {
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5

            currentIndex: 0
            model: ListModel {
                id: cbItems
                ListElement { text: "Diamond square"; value: "0" }
                ListElement { text: "Hill algorithm"; value: "1" }
                ListElement { text: "Perlin noise"; value: "2" }
            }
            width: 150
            height: 20
            onCurrentIndexChanged: console.debug(cbItems.get(currentIndex).text + ", " + cbItems.get(currentIndex).value)
        }

        Text {
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 20
            text: qsTr("Seed (empty for random):")
        }

        TextField {
            id: seedText
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: parent.width - 10
        }

        Rectangle {
            id: riversRect

        }

    }
    //TODO: call extern application to generate map, other design
}
