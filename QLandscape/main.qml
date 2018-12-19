import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "style"

import FileIO 1.0
import DrawMap 1.0
import ExecuteBinary 1.0

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

    ExecuteBinary {
        id: exec
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
        color: "#AAAAAA"

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
            onCurrentIndexChanged: {
                var index = cbItems.get(currentIndex).value
                try {
                    if (index < 2) {
                        diamondHillSettings.visible = true
                        perlinSettings.visible = false
                    } else {
                        diamondHillSettings.visible = false
                        perlinSettings.visible = true
                    }
                } catch (e) {
                    console.log(e);
                }
            }
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
            id: diamondHillSettings
            y: 100
            x: 0
            height: 100
            width: parent.width
            color: parent.color

            LeftText {
                anchors.topMargin: 5
                text: qsTr("Roughness")
            }
            RightTextField {
                id: roughnessText
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 35
                text: qsTr("Init height")
            }
            RightTextField {
                id: initHeightText
                anchors.topMargin: 30
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 65
                text: qsTr("Islands")
            }
            RightTextField {
                id: islandsText
                anchors.topMargin: 60
                validator: RegExpValidator{regExp: /\d+/}
            }
        }

        Rectangle {
            id: perlinSettings
            visible: false
            y: 100
            x: 0
            height: 100
            width: parent.width
            color: parent.color

            LeftText {
                anchors.topMargin: 5
                text: qsTr("Amplitude")
            }
            RightTextField {
                id: amplitudeText
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 35
                text: qsTr("Persistence")
            }
            RightTextField {
                id: persistenceText
                anchors.topMargin: 30
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 65
                text: qsTr("Frequency")
            }
            RightTextField {
                id: frequencyText
                anchors.topMargin: 60
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }
        }

        Rectangle {
            id: riversRect
            y: 200
            x: 0
            height: 100
            width: parent.width
            color: parent.color

            LeftText {
                anchors.topMargin: 5
                text: qsTr("Rivers number")
            }
            RightTextField {
                id: riversText
                validator: RegExpValidator{regExp: /\d+/}
            }

            LeftText {
                anchors.topMargin: 35
                text: qsTr("River min length")
            }
            RightTextField {
                id: riversLengthText
                anchors.topMargin: 30
                validator: RegExpValidator{regExp: /\d+/}
            }

            LeftText {
                anchors.topMargin: 65
                text: qsTr("River max width")
            }
            RightTextField {
                id: riversWidthText
                anchors.topMargin: 60
                validator: RegExpValidator{regExp: /\d+/}
            }
        }

        Button {
            anchors.top: parent.top
            anchors.topMargin: 300
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: parent.width - 10
            height: 30
            text: "Generate map"
            onClicked: {
                //TODO: call extern application to generate map
                console.log('1')
            }
        }
    }
}

//private String makeArguments() {
//    String result = " --size 512 --noise --output " + mapNameTextField.getText() + " --algorithm ";
//    result += algorithmNameByIndex(algoritmsList.getSelectedIndex());
//    String temp = seedTextField.getText();
//    result += "--seed ";
//    if (temp.equals(""))
//        result += (int)(Math.random()*2147483647) + " ";
//    else
//        result += Integer.parseInt(temp) + " ";
//    result += "--height " + Integer.parseInt(startHeightTextField.getText()) + " ";
//    result += "--roughness " + Float.parseFloat(roughnessTextField.getText()) + " ";
//    result += "--islands " + Integer.parseInt(islandsTextField.getText()) + " ";
//    result += "--amplitude " + Float.parseFloat(amplitudeTextField.getText()) + " ";
//    result += "--persistence " + Float.parseFloat(persistenceTextField.getText()) + " ";
//    result += "--frequency " + Float.parseFloat(frequencyTextField.getText()) + " ";
//    result += "--rivers_number " + Integer.parseInt(riversNumberTextField.getText()) + " ";
//    result += "--river_length " + Integer.parseInt(riverLengthTextField.getText()) + " ";
//    result += "--river_width " + Integer.parseInt(riverWidthTextField.getText()) + " ";
//    return result;
//}
