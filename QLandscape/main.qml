import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import FileIO 1.0

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

    function openFile(fileUrl) {
        return fileIO.openFile(fileUrl)
    }

    function saveFile(fileUrl, text) {
        return fileIO.saveFile(fileUrl, text)
    }

    function getColorFromInt(c) {
        if (c < -40)
            return Qt.rgba(0, 0, 32/255.0, 1)
        if (c < -10)
            return Qt.rgba(0, 0, (64 + c/2)/255.0, 1)
        if (c < 1)
            return Qt.rgba(0, 0, 128/255.0, 1)
        if (c < 10)
            return Qt.rgba(1, 1, (180 - c*5)/255.0, 1)
        if (c < 25)
            return Qt.rgba(0, (128 - c*4)/255.0, 0, 1)
        if (c < 60)
            return Qt.rgba((139 - c*2)/255.0, (69 - c)/255.0, (20 - c/3)/255.0, 1)
        if (c < 100)
            return Qt.rgba((155 + c)/255.0, (155 + c)/255.0, (155 + c)/255.0, 1)
        return Qt.rgba(1, 1, 1, 1)
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
                mapCanvas.requestPaint()
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

        Canvas {
            id: mapCanvas
            width: 512
            height: 512
            anchors {
                left: parent.left
                leftMargin: 20
                top: parent.top
                topMargin: 40
            }
            onPaint: {
                var ctx = getContext("2d")
                var mapdata = openFile(mapFileText.text)
                var mapLines = mapdata.split('\n')
                var len = mapLines.length
                var mapSize = parseInt(mapLines[0])
                for (var i = 1; i < len; i++) {
                    var line = mapLines[i].split(' ')
                    for (var j = 0; j < mapSize; j++) {
                        var value = parseInt(line[j])
                        if (isNaN(value))
                            continue
                        ctx.fillStyle = getColorFromInt(parseInt(line[j]))
                        ctx.fillRect(i - 1, j, 1, 1)
                    }
                }
            }
        }
        //TODO: fast draw using generating image instead of drawing multiple rectangulars
        //TODO: draw different size maps
    }
    //TODO: call extern application to generate map, other design
}
