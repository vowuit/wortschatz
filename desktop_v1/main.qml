import QtQuick 2.0
import Wortschatz 1.0

Rectangle {
    width: 200
    height: 400
    color: "#ef6767"

    Text {
        id: frontText
        x: 56
        y: 44
        text: trainer.front
        font.pixelSize: 20
    }

    Text {
        id: backText
        x: 56
        y: 109
        text: trainer.back
        font.pixelSize: 20
    }

    Rectangle {
        id: btnFAILED
        x: 36
        y: 178
        width: 128
        height: 45

        color: mouseFailed.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor

        property color buttonColor: "lightblue"
        property color onHoverColor: "gold"
        property color borderColor: "white"

        signal buttonClick()

        MouseArea {


            id: mouseFailed
            anchors.fill: parent
            onClicked: trainer.answer(Trainer.FAIL);
            hoverEnabled: true
            onEntered: parent.border.color = parent.onHoverColor
            onExited:  parent.border.color = parent.borderColor
        }
        Text {
            id: text1
            text: qsTr("FAILED")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
            font.pixelSize: 12
        }


    }

    Rectangle {
        id: btnSUCCESS
        x: 36
        y: 234
        width: 128
        height: 45

        color: mouseSuccess.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor

        property color buttonColor: "lightblue"
        property color onHoverColor: "gold"
        property color borderColor: "white"

        signal buttonClick()

        MouseArea {
            id: mouseSuccess
            anchors.fill: parent
            onClicked: trainer.answer(Trainer.SUCCESS);
            hoverEnabled: true
            onEntered: parent.border.color = parent.onHoverColor
            onExited:  parent.border.color = parent.borderColor
        }

        Text {
            id: text2
            x: 0
            y: 0
            text: qsTr("SUCCESS")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
    }
}
