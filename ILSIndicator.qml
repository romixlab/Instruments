import QtQuick 2.4

Canvas {
    property real diff: 0
    property real fullScale: 1.0

    onDiffChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.fillColor = 'black'
        ctx.fillRect(0, 0, width, height)
        ctx.save()
        ctx.translate(width / 2, height / 2)
        ctx.lineWidth = 2
        ctx.strokeStyle = 'yellow'
        ctx.beginPath()
        ctx.moveTo(-width / 2, 0); ctx.lineTo(width / 2, 0)
        ctx.stroke()

        ctx.strokeStyle = 'white'
        var hc = height / 2 - width / 2
        ctx.beginPath(); ctx.arc(0, hc / 2, width / 3, 0, 2 * Math.PI); ctx.stroke()
        ctx.beginPath(); ctx.arc(0, hc, width / 3, 0, 2 * Math.PI); ctx.stroke()
        ctx.beginPath(); ctx.arc(0, -hc / 2, width / 3, 0, 2 * Math.PI); ctx.stroke()
        ctx.beginPath(); ctx.arc(0, -hc, width / 3, 0, 2 * Math.PI); ctx.stroke()
        ctx.restore()

        ctx.strokeStyle = '#e200e8'
        ctx.lineWidth = 2
        var diffBoxed = diff
        if (diffBoxed > fullScale)  diffBoxed =  fullScale
        if (diffBoxed < -fullScale) diffBoxed = -fullScale;
        ctx.save()
        ctx.translate(width / 2, height / 2 - (diffBoxed / fullScale) * hc)
        ctx.beginPath()
        var wc = width / 2 - 1
        ctx.moveTo(-wc, 0); ctx.lineTo(0, wc);
        ctx.lineTo(wc, 0); ctx.lineTo(0, -wc);
        ctx.closePath()
        ctx.stroke();
        ctx.restore()
    }
}
