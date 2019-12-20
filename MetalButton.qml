import QtQuick 2.0
import QtGraphicalEffects 1.0

Image {
    id: root
    source: "qrc:/images/button-rect.png"
    width: 72; height: 72
    property alias text: label.text
    property bool pressed: false

    Text {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        y: 12
        font {
            bold: true
            pointSize: 10
        }
        color: "#ccc"
    }

    Column {
        id: light
        visible: false
        width: 50
        spacing: 7
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom; anchors.bottomMargin: 8

        Repeater {
            model: 3

            Rectangle {
                height: 1; width: light.width
                color: "#bce82e"; opacity: 0.7
            }
        }
    }

    Glow {
        id: lightGlow
        anchors.fill: light
        radius: 3; samples: 8
        color: "#bce82e"
        source: light
        opacity: 0.0
        Behavior on opacity { NumberAnimation { duration: 300; } }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            lightGlow.opacity = lightGlow.opacity === 0.0 ? 1.0 : 0.0
            root.pressed = !root.pressed
        }
    }
}
