import QtQuick 2.0

Canvas {
    id: root
    implicitWidth: 6; implicitHeight: 11

    onPaint: {
        var ctx = getContext("2d")
        ctx.strokeStyle = 'green'
        ctx.lineWidth = 2
        ctx.beginPath()
        ctx.moveTo(6, 0); ctx.lineTo(0, 5);
        ctx.moveTo(6, 6); ctx.lineTo(0, 11);
        ctx.stroke()
    }
}
