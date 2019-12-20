import QtQuick 2.0

Canvas {
    id: root
    property double roll:  0
    property double pitch: 0
    property double pitchScale: 30

    implicitWidth: 200; implicitHeight: 200

    function sign(x) {
        return x > 0 ? 1 : -1
    }

    onPaint: {
        var ctx = getContext("2d")
        ctx.save()

        ctx.clearRect(0, 0, width, height)

        var ovlpitch = pitch; //wrap360



        var w2 = width / 2
        var h2 = height / 2
        var scpitch = ovlpitch
        if (scpitch > 150) scpitch -= 180
        if (scpitch < -150) scpitch += 180
        var h = -scpitch / pitchScale * h2
        console.log('scpitch', scpitch)

        ctx.translate(w2, h2)
        ctx.rotate(roll * Math.PI / 180)

        if ((scpitch > pitchScale) || (scpitch < -pitchScale)) {
            ctx.beginPath()
            ctx.arc(0, 0, h2, 0, 2 * Math.PI)
//            if (ovlpitch > 150 || ovlpitch < -150)
//                ctx.fillStyle = ovlpitch > pitchScale ? '#884600' : '#2977ef'
//            else
                ctx.fillStyle = ovlpitch > pitchScale ? '#2977ef' : '#884600'
            ctx.fill()
        } else {
            var alpha = Math.asin(h / h2)

            ctx.beginPath()
            ctx.arc(0, 0, h2, -Math.PI + alpha, -alpha)
            ctx.fillStyle = (ovlpitch > 150 || ovlpitch < -150) ? '#884600' : '#2977ef'
            ctx.fill()

            ctx.beginPath()
            ctx.arc(0, 0, h2, -alpha, -Math.PI + alpha)
            ctx.fillStyle = (ovlpitch > 150 || ovlpitch < -150) ? '#2977ef' : '#884600'
            ctx.fill()

            ctx.beginPath()
            ctx.moveTo(-h2 * Math.cos(alpha), -h); ctx.lineTo(h2 * Math.cos(alpha), -h)
            ctx.strokeStyle = 'white'
            ctx.stroke()
        }

        ctx.strokeStyle = 'white'
        ctx.fillStyle = 'white'
        ctx.font = '12px ubuntu'
        drawPitchMarks(ctx, h2 / pitchScale, h2, ovlpitch)

//        if (width != height) {
//            ctx.beginPath()
//            ctx.rect(-w2, -h2, w, h)
//        }


        ctx.restore()
    }

    function drawPitchMarks(ctx, pxPerDegree, radius, ovlpitch) {
        var dp = ovlpitch + pitchScale
        var pmax = Math.ceil(dp / 2.5) * 2.5
        var ymax = ovlpitch * pxPerDegree - pmax * pxPerDegree

        console.log('dp',dp, 'pmax', pmax, 'ymax', ymax)

        for (var y = ymax, p = pmax; y < radius; y += 2.5 * pxPerDegree, p -= 2.5) {
            if (p == 180 || p == 0 || p == -180)
                continue

            ctx.beginPath()
            var w = 10
            ctx.moveTo(-w, y); ctx.lineTo(w, y)
            ctx.stroke()

            var pp = p
            if (pp > 180)
                pp -= 360
            if (pp < -180)
                pp += 360
            if (pp % 10 == 0)
                ctx.fillText(pp, 25, y + 6)
        }

//        for (var y = -pitch * pxPerDegree; y < radius; y += pxPerDegree * 2.5) {





//            p += 2.5

////            var t = width
////            if (widthPrev == 20 && width == 5) width = 10
////            if (widthPrev == 5 && width == 10) width = 5
////            if (widthPrev == 10 && width == 5) width = 20
////            if (widthPrev == 5 && width == 20) width = 5
////            widthPrev = t
//        }
    }

    onRollChanged: root.requestPaint()
    onPitchChanged: root.requestPaint()
}
