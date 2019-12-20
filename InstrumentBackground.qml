import QtQuick 2.0
//import "canvashelpers.js" as CH

BorderImage {
    id: root
    // Image borrowed Prosim MCP
    source: "data:image/png;base64,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"
    //implicitWidth: 200; implicitHeight: 200
    border.left: 25; border.top: 25
    border.right: 25; border.bottom: 25
}

//Canvas {
//    id: root
//    property int borderWidth: 10
//    property int borderRadius: 30
//    implicitWidth: 200; implicitHeight: 200
//    onPaint: {
//        var ctx = getContext("2d")

//        var cornerGrd = ctx.createLinearGradient(0, 0, width, height)
//        cornerGrd.addColorStop(0, '#535353')
//        cornerGrd.addColorStop(1, '#979797')
//        ctx.strokeStyle = cornerGrd
//        ctx.lineWidth = 10
//        CH.roundRect(ctx, 5, 5, width - borderWidth, height - borderWidth, 55)

//        var bgGrd = ctx.createLinearGradient(10, 10, width - 20, height - 20)
//        bgGrd.addColorStop(0, '#2c2c2e')
//        bgGrd.addColorStop(1, '#454547')
//        ctx.fillStyle = bgGrd
//        CH.roundRect(ctx, 10, 10, width - 2 * borderWidth, height - 2 * borderWidth, 49, true, false)

//        ctx.strokeStyle = '#a4a4a6'
//        ctx.lineWidth = 1
//        CH.roundRect(ctx, 15, 15, width - 30, height - 30, 45)

//        ctx.lineWidth = 1
//        screw(ctx, 40, 40, 10); screw(ctx, 40, height - 40, 10)
//        screw(ctx, width - 40, 40, 10); screw(ctx, width - 40, height - 40, 10)
//    }

//    function screw(ctx, x, y, r) {
//        ctx.beginPath()
//        ctx.arc(x, y, r, 0, 2 * Math.PI)
//        ctx.strokeStyle = '#222222'
//        ctx.fillStyle = '#4d4d4d'
//        ctx.fill(); ctx.stroke();

//        ctx.save()
//        ctx.translate(x, y)
//        ctx.rotate(Math.random() * 90 * Math.PI / 180)
//        ctx.beginPath()
//        ctx.moveTo(0, - r); ctx.lineTo(0, r);
//        ctx.strokeStyle = '#111111'
//        ctx.stroke()
//        ctx.restore()
//    }
//}
