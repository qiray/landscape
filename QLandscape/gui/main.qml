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
    id: mainWindow
    property string settingsPath: "settings.cfg"
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
        onFinish: redrawMap()
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open map")
                onTriggered: fileDialog.open()
            }
            MenuItem {
                text: qsTr("Close app")
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
            text: "1.map"
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
            id: algorithmSelector

            currentIndex: 0
            model: ListModel {
                id: cbItems
                ListElement { text: "Diamond square"; value: "0"; name: 'diamond_square' }
                ListElement { text: "Hill algorithm"; value: "1"; name: 'hill_algorithm' }
                ListElement { text: "Perlin noise"; value: "2"; name: 'perlin_noise' }
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
            validator: RegExpValidator{regExp: /\d+/}
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
                text: "0.5"
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 35
                text: qsTr("Init height")
            }
            RightTextField {
                id: initHeightText
                text: "5"
                anchors.topMargin: 30
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 65
                text: qsTr("Islands")
            }
            RightTextField {
                id: islandsText
                text: "1"
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
                text: "0.25"
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 35
                text: qsTr("Persistence")
            }
            RightTextField {
                id: persistenceText
                text: "0.7"
                anchors.topMargin: 30
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }

            LeftText {
                anchors.topMargin: 65
                text: qsTr("Frequency")
            }
            RightTextField {
                id: frequencyText
                text: "0.01"
                anchors.topMargin: 60
                validator: RegExpValidator{regExp: /(\d*[.])?\d+/}
            }
        }

        Rectangle {
            id: riversRect
            y: 190
            x: 0
            height: 30
            width: parent.width
            color: parent.color

            LeftText {
                anchors.topMargin: 5
                text: qsTr("Rivers number")
            }
            RightTextField {
                id: riversText
                text: "10"
                validator: RegExpValidator{regExp: /\d+/}
            }
        }

        Button {
            anchors.top: parent.top
            anchors.topMargin: 220
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: parent.width - 10
            height: 30
            text: "Generate map"
            onClicked: {
                exec.runBinary('landscape', makeArguments());
            }
        }
    }

    onClosing: {
        saveSettings()
    }
    Component.onCompleted: {
        loadSettings()
    }

    function makeArguments() {
        var result = ['--size', '512', '--noise', '--output', mapFileText.text, '--algorithm', cbItems.get(algorithmSelector.currentIndex).name]
        if (seedText.text != '') {
            result.push('--seed', seedText.text)
        } else {
            result.push('--seed', + new Date())
        }
        result = result.concat(['--height', initHeightText.text, '--roughness', roughnessText.text, '--islands', islandsText.text])
        result = result.concat(['--amplitude', amplitudeText.text, '--persistence', persistenceText.text, '--frequency', frequencyText.text])
        result = result.concat(['--rivers_number',riversText.text])
        return result
    }

    function saveSettings() {
        var result = {
            x: mainWindow.x,
            y: mainWindow.y,
            output: mapFileText.text,
            algorithm: algorithmSelector.currentIndex,
            seed: seedText.text,
            height: initHeightText.text,
            roughness: roughnessText.text,
            islands: islandsText.text,
            amplitude: amplitudeText.text,
            persistence: persistenceText.text,
            frequency: frequencyText.text,
            rivers_number: riversText.text,
        }
        saveFile(mainWindow.settingsPath, JSON.stringify(result))
    }

    function loadSettings() {
        try {
            var result = JSON.parse(openFile(mainWindow.settingsPath))
            mainWindow.setX(result.x)
            mainWindow.setY(result.y)
            mapFileText.text = result.output
            algorithmSelector.currentIndex = result.algorithm
            seedText.text = result.seed
            initHeightText.text = result.height
            roughnessText.text = result.roughness
            islandsText.text = result.islands
            amplitudeText.text = result.amplitude
            persistenceText.text = result.persistence
            frequencyText.text = result.frequency
            riversText.text = result.rivers_number
        } catch (e) {
            console.log(e);
        }
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

}
