import QtQuick 2.0

Item {
    id: root
    property color  color:          colorLightGreen
    property color  borderColor:    "black"
    property int    borderWidth:    5
    property int    borderRadius:   5

    readonly property color colorMateGreen:      "#87ad34"
    readonly property color colorLightGreen:     "#8ffe01"
    readonly property color colorOrange:         "#fb7c00"

    implicitWidth: 200; implicitHeight: 80

    Rectangle {
        anchors.fill: parent
        color: root.borderColor; radius: root.borderRadius
        Rectangle {
            anchors.fill: parent; anchors.margins: root.borderWidth
            color: root.darker()
            radius: root.borderRadius / 2
        }
        Rectangle {
            anchors.fill: parent
            anchors.margins: root.borderWidth * 1.5
            color: root.color; radius: root.borderRadius / 2
        }
    }

    function darker() {
        return Qt.darker(root.color, 1.3)
    }
}
