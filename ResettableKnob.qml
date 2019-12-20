import QtQuick 2.0
import QtQuick.Templates 2.0 as T

Item {
    id: root

    property bool power: false
    property alias text: label.text
    property alias position: dial.position
    property alias value: dial.value

    width: 96; height: 96
    T.Dial {
        id: dial
        enabled: true

        anchors.fill: parent

        from: -1; to: 1
        stepSize: 1

        background: Image {
            source: "qrc:/images/knob.png"
            width: parent.width; height: parent.height
            antialiasing: true
            rotation: parent.angle
        }

        Rectangle {
            anchors.centerIn: parent
            width: 70; height: 70; radius: 35
            border.color: "gray"; border.width: 2
            color: "transparent"
            opacity: root.power ? 0.0 : 1.0

            Behavior on opacity { NumberAnimation { duration: 300 } }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 66; height: 66; radius: 33
        color: "#222"

        Text {
            id: label
            anchors.centerIn: parent
            color: "white"
            font.pointSize: 12
        }

        MouseArea {
            anchors.fill: parent; anchors.margins: 10
            onClicked: root.power = !root.power
        }
    }
}
