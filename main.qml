import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import QtGraphicalEffects 1.0
import io.smth 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 705
    title: qsTr("PlaneSIM")
    color: "darkgray"

    Component.onCompleted: showMaximized()

    Material.theme: Material.Dark
    Material.accent: Material.color(Material.Orange)

    FontLoader { id: dseg7; source: "qrc:/fonts/DSEG7-Classic/DSEG7Classic-Bold.ttf" }
    FontLoader { id: dsegdot; source: "qrc:/fonts/lcddot_tr.ttf" }

    Component {
        id: lcdNumberWithName

        Item {
            height: 40
            width: numberShadow.width
            property alias text: numberText.text
            property string shadow
            property string title
            Text {
                id: numberShadow
                anchors.centerIn: parent
                font {
                    family: dseg7.name
                    pixelSize: parent.height
                }
                color: lcd.darker()
                text: parent.shadow

                Text {
                    id: numberText
                    font: parent.font
                }
            }

            Text {
                text: parent.title
                anchors.horizontalCenter: numberShadow.horizontalCenter
                anchors.bottom: numberShadow.top; anchors.bottomMargin: 10
                font {
                    family: dsegdot.name
                    pixelSize: 32
                }
            }
        }
    }

    InstrumentBackground {
        id: controlInstrumentsBackground
        anchors.top: parent.top; anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: 1200
        height: 230

        LCDBackground {
            id: lcd
            anchors.top: parent.top; anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            width: 850
            height: 100

            color: colorOrange

            Loader {
                id: lcdSpeed
                sourceComponent: lcdNumberWithName
                anchors.left: parent.left; anchors.leftMargin: 15
                anchors.bottom: parent.bottom; anchors.bottomMargin: 15
                onLoaded: { item.shadow = "888"; item.title = "SPD" }
            }

            Rectangle {
                width: 20; height: 20
                anchors.bottom: lcdSpeed.bottom
                anchors.left: lcdSpeed.right; anchors.leftMargin: 20
                color: "black"; radius: 4
            }

            Loader {
                id: lcdHeading
                sourceComponent: lcdNumberWithName
                anchors.left: lcdSpeed.right; anchors.leftMargin: 80
                anchors.bottom: parent.bottom; anchors.bottomMargin: 15
                onLoaded: { item.shadow = "888"; item.title = "HDG" }
            }

            Rectangle {
                width: 20; height: 20
                anchors.bottom: lcdHeading.bottom
                anchors.left: lcdHeading.right; anchors.leftMargin: 20
                color: "black"; radius: 4
            }

            Loader {
                id: lcdVerticalSpeed
                sourceComponent: lcdNumberWithName
                anchors.right: parent.right; anchors.rightMargin: 15
                anchors.bottom: parent.bottom; anchors.bottomMargin: 15
                onLoaded: { item.shadow = "8888"; item.title = "V/S" }
            }

            Text {
                text: "HDG V/S"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom; anchors.bottomMargin: 25
                font {
                    family: dsegdot.name
                    pixelSize: 32
                }
            }

            Text {
                id: lvlChText
                text: "LVL/CH"
                anchors.right: lcdVerticalSpeed.left
                anchors.bottom: lcdVerticalSpeed.top; anchors.bottomMargin: 10
                font {
                    family: dsegdot.name
                    pixelSize: 32
                }
            }

            Loader {
                id: lcdAltitude
                sourceComponent: lcdNumberWithName
                anchors.right: lvlChText.left
                anchors.bottom: parent.bottom; anchors.bottomMargin: 15
                onLoaded: { item.shadow = "8888"; item.title = "ALT" }
            }

            Canvas {
                id: lvlChLines
                height: 10; width: 130
                anchors.horizontalCenter: lvlChText.horizontalCenter
                anchors.top: lvlChText.top; anchors.topMargin: 10
                onPaint: {
                    var ctx = lvlChLines.getContext("2d")
                    ctx.beginPath()
                    ctx.moveTo(0, lvlChLines.height)
                    ctx.lineTo(0, 0)
                    ctx.lineTo(20, 0)
                    ctx.lineWidth = 4
                    ctx.stroke()
                    ctx.beginPath()
                    ctx.moveTo(lvlChLines.width, lvlChLines.height)
                    ctx.lineTo(lvlChLines.width, 0)
                    ctx.lineTo(lvlChLines.width - 20, 0)
                    ctx.stroke()
                }
            }
        }

        Canvas {
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d")
                ctx.beginPath()
                ctx.strokeStyle = 'white'
                ctx.lineWidth = 4
                circleBound(ctx, spdMachButton)
                //circleBound(ctx, centerFakeButton)
                circleBound(ctx, metricAltButton)
                circleBound(ctx, speedDial)
                circleBound(ctx, headingDial)
                circleBound(ctx, altitudeDial)
                circleBound(ctx, verticalSpeedDial)
            }

            function circleBound(ctx, item) {
                var radius = item.width / 2
                ctx.beginPath()
                ctx.arc(item.x + radius, item.y + radius, radius, 0, 2 * Math.PI)
                ctx.stroke()
            }
        }

        ResettableKnob {
            id: speedDial
            anchors.top: lcd.bottom; anchors.topMargin: 10
            anchors.left: lcd.left; anchors.leftMargin: -30
            text: "SPD"
            value: -1
            onPositionChanged: {
                if (lcdSpeed.item) lcdSpeed.item.text = Math.ceil(position * 400)
                autopilot.targetSpeedChanged(position * 400)
            }
            onPowerChanged: autopilot.speedButtonClicked(power)
        }

        ResettableKnob {
            id: headingDial
            anchors.top: lcd.bottom; anchors.topMargin: 10
            anchors.left: speedDial.right; anchors.leftMargin: 30
            text: "HDG"
            value: -1
            onPositionChanged: {
                if (lcdHeading.item) lcdHeading.item.text = Math.ceil(position * 360)
                autopilot.setTargetHeading(Math.ceil(position * 360))
            }
            onPowerChanged: {
                ilsButton.pressed = false
                gpsButton.pressed = false
                autopilot.headingButtonClicked(power)
            }
        }

        ResettableKnob {
            id: altitudeDial
            anchors.top: lcd.bottom; anchors.topMargin: 10
            anchors.right: verticalSpeedDial.left; anchors.rightMargin: 60
            text: "ALT"
            onPositionChanged: {
                autopilot.setTargetAltitude(Math.ceil(position * 10000))
                if (lcdAltitude.item) lcdAltitude.item.text = Math.ceil(position * 10000)
            }
            onPowerChanged: autopilot.altitudeButtonClicked(power)
        }

        ResettableKnob {
            id: verticalSpeedDial
            anchors.top: lcd.bottom; anchors.topMargin: 10
            anchors.right: lcd.right; anchors.rightMargin: -60
            text: "V/S"
            value: 1000/1800
            onPositionChanged: {
                autopilot.setTargetVerticalSpeed(Math.ceil(position * 1800))
                if (lcdVerticalSpeed.item) lcdVerticalSpeed.item.text = Math.ceil(position * 1800)
            }
            onPowerChanged: autopilot.verticalSpeedButtonClicked(power)
        }

        Text {
            text: "PUSH\nTO\nLEVEL\nOFF"; anchors.verticalCenter: verticalSpeedDial.verticalCenter
            anchors.left: verticalSpeedDial.right; anchors.leftMargin: 15
            font { bold: true; pointSize: 10 }
            color: "white"; horizontalAlignment: Text.AlignHCenter
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: lcd.horizontalCenter
            anchors.verticalCenter: altitudeDial.verticalCenter

            MetalButton {
                id: ilsButton
                text: "ILS"
                onPressedChanged: {
                    headingDial.power = false
                    gpsButton.pressed = false
                    autopilot.ilsButtonClicked(pressed)
                }
            }

            MetalButton {
                id: gpsButton
                text: "GPS"
                onPressedChanged: {
                    headingDial.power = false
                    ilsButton.pressed = false
                    autopilot.gpsButtonClicked(pressed)
                }
            }

            MetalButton {
                id: ap1Button
                text: "AP1"
                onPressedChanged: autopilot.ap1ButtonClicked(pressed)
            }

            MetalButton {
                id: ap2Button
                text: "AP2"
                enabled: false
            }

            MetalButton {
                id: athrButton
                text: "A/THR"
                onPressedChanged: autopilot.athrConfig(pressed)
            }
        }



        Image {
            id: spdMachButton
            source: "qrc:/images/button-circle.png"
            anchors.verticalCenter: speedDial.verticalCenter; anchors.verticalCenterOffset: -15
            anchors.right: speedDial.left; anchors.rightMargin: 10
        }

        Text {
            text: "SPD\nMACH"; anchors.horizontalCenter: spdMachButton.horizontalCenter
            anchors.top: spdMachButton.bottom; anchors.topMargin: 5
            font { bold: true; pointSize: 8 }
            color: "white"; horizontalAlignment: Text.AlignHCenter
        }

//        Image {
//            id: centerFakeButton
//            source: "qrc:/images/button-circle.png"
//            anchors.horizontalCenter: lcd.horizontalCenter
//            anchors.top: lcd.bottom; anchors.topMargin: 10
//        }

        Image {
            id: metricAltButton
            source: "qrc:/images/button-circle.png"
            anchors.verticalCenter: altitudeDial.verticalCenter; anchors.verticalCenterOffset: -10
            anchors.left: altitudeDial.right; anchors.leftMargin: 15
        }

        Text {
            text: "METRIC\nALT"; anchors.horizontalCenter: metricAltButton.horizontalCenter
            anchors.top: metricAltButton.bottom; anchors.topMargin: 5
            font { bold: true; pointSize: 8 }
            color: "white"; horizontalAlignment: Text.AlignHCenter
        }

        Dial {
            anchors.left: parent.left; anchors.leftMargin: 100
            anchors.top: parent.top; anchors.topMargin: 20
            width: 50; height: 50
            from: 0; to: 1; value: 0.20
            onPositionChanged: mapInstrumentBackground.anchors.horizontalCenterOffset = (position - 0.5) * 500
            Component.onCompleted: onPositionChanged()
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left; anchors.leftMargin: 20
            width: 50
            spacing: 3

            Button { text: "S1"; onClicked: autopilot.situationLoad(text) }
            Button { text: "S2"; onClicked: autopilot.situationLoad(text) }
            Button { text: "S3"; onClicked: autopilot.situationLoad(text) }
            Button { text: "S4"; onClicked: autopilot.situationLoad(text) }
        }
    } // Control InstrumentBackground

    InstrumentBackground {
        id: ahInstrumentBackground
        anchors.top: controlInstrumentsBackground.bottom; anchors.topMargin: 5
        anchors.right: mapInstrumentBackground.left; anchors.rightMargin: 5
        width: 460; height: 460

        Rectangle {
            anchors.fill: parent
            anchors.margins: 20
            radius: 10; color: "#111"
        }

        ArtificialHorizon {
            id: artificialHorizon
            width: 200; height: 280
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -10

            roll: autopilot.roll
            pitch: autopilot.pitch
        }

        Text {
            id: altitudeActualLabel
            anchors.horizontalCenter: artificialHorizon.horizontalCenter
            anchors.bottom: artificialHorizon.bottom; anchors.bottomMargin: 10
            text: Math.ceil(altitudeLinearIndicator.current)
            font { bold: true; pointSize: 14 }
            color: "#ebb751"
        }

        TickMark {
            anchors.left: artificialHorizon.right; anchors.leftMargin: 5
            anchors.top: artificialHorizon.top; anchors.topMargin: 90
        }

        TickMark {
            anchors.right: artificialHorizon.left; anchors.rightMargin: 5
            anchors.top: artificialHorizon.top; anchors.topMargin: 90
            transform: [ Scale { origin.x: 3; xScale: -1 } ]
        }

        LinearIndicator {
            id: headingLinearIndicator
            width: artificialHorizon.width; height: 40
            anchors.top: artificialHorizon.bottom; anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter

            current: autopilot.heading

            wrap: 360

            font {
                family: "Helvetica"
                pixelSize: 16
            }
        }

        LinearIndicator {
            id: speedLinearIndicator
            width: 60; height: artificialHorizon.height
            anchors.right: artificialHorizon.left; anchors.rightMargin: 30
            anchors.verticalCenter: artificialHorizon.verticalCenter

            current: autopilot.planeSpeed

            orientation: LinearIndicator.VerticalRight
            tickmarksStepSize: 40
            tickmarksStepValue: 10
            tickmarksEach: 20

            segments: [
                ColorSegment { from: 0; to: 100; color: "red" },
                ColorSegment { from: 100; to: 115; color: "yellow" },
                ColorSegment { from: 320; to: 400; color: "red" }
            ]

            font {
                family: "Helvetica"
                pixelSize: 16
            }
        }

        Text {
            id: speedTargetLabel
            anchors.horizontalCenter: speedLinearIndicator.horizontalCenter
            anchors.bottom: speedLinearIndicator.top; anchors.bottomMargin: 5
            text: Math.ceil(speedDial.position * 400)
            font { bold: true; pointSize: 14 }
            color: "white"
        }

        LinearIndicator {
            id: altitudeLinearIndicator
            width: 60; height: artificialHorizon.height
            anchors.left: artificialHorizon.right; anchors.leftMargin: 30
            anchors.verticalCenter: artificialHorizon.verticalCenter

            orientation: LinearIndicator.VerticalLeft
            tickmarksStepSize: 40
            tickmarksStepValue: 100
            tickmarksEach: 500

            current: autopilot.altitude

            font {
                family: "Helvetica"
                pixelSize: 16
            }
        }

        Canvas {
            property real diff: autopilot.verticalSpeed / 1.5
            property real fullScale: 1.0
            anchors.right: parent.right; anchors.rightMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            width: 10; height: 250
            onDiffChanged: requestPaint()
            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                ctx.save()
                ctx.translate(0, height / 2)
                ctx.strokeStyle = 'white'
                ctx.lineWidth = 2
                var hc = height / 2 - 2
                var yy = [0, hc / 3, hc * 2 / 3, hc * 5 / 6, hc]
                yy.forEach(function(item, i, yy) {
                    ctx.beginPath(); ctx.moveTo(0, item); ctx.lineTo(5, item); ctx.stroke();
                    ctx.beginPath(); ctx.moveTo(0, -item); ctx.lineTo(5, -item); ctx.stroke();
                });
                ctx.restore()

                ctx.strokeStyle = 'green'
                ctx.lineWidth = 4
                var diffBoxed = diff
                if (diffBoxed > fullScale)  diffBoxed =  fullScale
                if (diffBoxed < -fullScale) diffBoxed = -fullScale;
                ctx.save()
                ctx.translate(width / 2, height / 2 - (diffBoxed / fullScale) * hc)
                ctx.beginPath()
                ctx.moveTo(-width / 2, 0); ctx.lineTo(width / 2, 0)
                ctx.stroke()
                ctx.restore()
            }
        }

        Text {
            id: altitudeTargetLabel
            anchors.horizontalCenter: altitudeLinearIndicator.horizontalCenter
            anchors.bottom: altitudeLinearIndicator.top; anchors.bottomMargin: 5
            text: Math.ceil(altitudeDial.position * 10000)
            font { bold: true; pointSize: 14 }
            color: "white"
        }

        Text {
            id: aThrLabel
            anchors.left: parent.left; anchors.top: parent.top
            anchors.margins: 30
            text: "A/THR"; color: "#00d0d0"; font { pixelSize: 18 }
            visible: athrButton.pressed
        }

        Text {
            id: spdLabel
            anchors.top: aThrLabel.bottom
            anchors.horizontalCenter: aThrLabel.horizontalCenter
            text: "SPD"; color: "#00d0d0"; font { pixelSize: 18 }
            visible: athrButton.pressed && (Math.abs(speedLinearIndicator.current - speedDial.position * 400) < 1)
        }

        Text {
            id: hdgLabel
            anchors.top: aThrLabel.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: "HDG"; color: "#00d0d0"; font { pixelSize: 18 }
        }

        Text {
            id: apLabel
            anchors.top: parent.top; anchors.topMargin: 30
            anchors.right: parent.right; anchors.rightMargin: 80
            text: "A/P"; color: "#00d0d0"; font { pixelSize: 18 }
        }

        Text {
            id: altLabel
            anchors.top: apLabel.bottom
            anchors.right: apLabel.left
            text: "ALT"; color: "#00d0d0"; font { pixelSize: 18 }
        }

        Text {
            id: vsLabel
            anchors.top: apLabel.bottom
            anchors.left: apLabel.right
            text: "V/S"; color: "#00d0d0"; font { pixelSize: 18 }
        }
    } // InstrumentBackground ArtificialHorizon

    InstrumentBackground {
        id: mapInstrumentBackground
        anchors.top: controlInstrumentsBackground.bottom; anchors.topMargin: 5
        anchors.horizontalCenter: controlInstrumentsBackground.horizontalCenter
        width: 460; height: 460

        Rectangle {
            id: mapDarkRect
            anchors.fill: parent
            anchors.margins: 20
            radius: 10
            color: "#111"
            clip: true

            Flickable {
                anchors.fill: parent
                contentWidth: 1000
                contentHeight: 1000
                contentX: 250
                contentY: 250

                Rectangle {
                    id: landingStrip
                    color: "yellow"
                    width: 20; height: 200
                    x: 490; y: 300

                    Rectangle {
                        width: 14; height: 14
                        color: "black"; radius: 7
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                    }

                    Rectangle {
                        width: 10; height: 10
                        color: "red"; radius: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5
                    }
                }

                Rectangle {
                    width: 2; height: 2; color: "red"
                    x: 100; y: 900
                }

                Canvas {
                    anchors.top: landingStrip.bottom; anchors.left: landingStrip.left
                    anchors.topMargin: 5
                    width: 20; height: 150
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.strokeStyle = 'red'
                        ctx.lineWidth = 2
                        for (var y = 0; y <= height; y += 20) {
                            ctx.beginPath()
                            ctx.moveTo(10, y)
                            ctx.lineTo(10, y + 10)
                            ctx.stroke()
                        }
                    }
                }



                Item {
                    width: 1000; height: 1000
                    Image {
                        id: planeImage
                        source: "qrc:/images/plane.png"
                        width: 50; height: 50
                        visible: false; smooth: true
                        x: autopilot.planeX * 100 - 25; y: (10 - autopilot.planeY) * 100 - 20
                    }

                    ColorOverlay {
                        anchors.fill: planeImage
                        source: planeImage
                        color: "#666"
                        opacity: 0.75
                        transform: [
                            Rotation {
                                axis {x: 0; y: 0; z: 1}
                                origin.x: 25; origin.y: 20
                                angle: autopilot.heading
                            }
                        ]
                    }
                }

                AutopilotMapLayer {
                    anchors.fill: parent
                    Component.onCompleted: autopilot.setMapLayer(this)
                }
            }

            Text {
                color: "white"; text: "--- nm"
                anchors.left: parent.left; anchors.top: parent.top
                anchors.margins: 30
                font { pixelSize: 18; }
            }

            Canvas {
                anchors.top: parent.top; anchors.right: parent.right
                anchors.topMargin: 15; anchors.rightMargin: 10
                width: 40; height: 90
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.fillStyle = '#00a2e8'
                    ctx.beginPath()
                    ctx.moveTo(12, 0)
                    ctx.lineTo(23, 40)
                    ctx.lineTo(18, 40)
                    ctx.lineTo(18, 71)
                    ctx.lineTo(5, 71)
                    ctx.lineTo(5, 40)
                    ctx.lineTo(0, 40)
                    ctx.fill()
                    ctx.font = '14px sans-serif'
                    ctx.fillText("N", 6, 85)
                }
            }
        }

        InnerShadow {
            anchors.fill: mapDarkRect
            horizontalOffset: -3
            verticalOffset: -3
            radius: 8
            samples: 16
            spread: 0.2
            source: mapDarkRect
        }
    } // InstrumentBackground Map

    InstrumentBackground {
        id: hsiInstrimentBackround
        width: 460; height: 460
        anchors.top: controlInstrumentsBackground.bottom; anchors.topMargin: 5
        anchors.left: mapInstrumentBackground.right; anchors.leftMargin: 5

        Rectangle {
            id: hsiDarkRect
            anchors.fill: parent
            anchors.margins: 20
            radius: 10
            color: "#111"
        }

        HorizontalSituationIndicator {
            id: hsiIndicator
            anchors.centerIn: parent
            width: 350; height: 350
            anchors.horizontalCenterOffset: -20
            heading: autopilot.heading
            Component.onCompleted: autopilot.setHSI(this)
        }

        Text {
            text: "MAG"
            anchors.top: parent.top; anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
        }

        Text {
            text: "VOR1"
            anchors.bottom: distanceToRadioBeacon.top; anchors.left: distanceToRadioBeacon.left
            color: "white"
        }

        Text {
            id: distanceToRadioBeacon
            property real distance: 0.0
            color: "#06d404"
            text: "DME " + distance + " NM"
            anchors.left: parent.left; anchors.bottom: parent.bottom
            anchors.leftMargin: 30; anchors.bottomMargin: 60
        }

        ILSIndicator {
            opacity: ilsButton.pressed
            Behavior on opacity { NumberAnimation { duration: 300 } }
            anchors.right: parent.right; anchors.rightMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            width: 10; height: 250
            diff: autopilot.altitude - (5 - autopilot.planeY) / 1.5 * 2500
            fullScale: 1000
        }

        ILSIndicator {
            opacity: ilsButton.pressed
            Behavior on opacity { NumberAnimation { duration: 300 } }
            anchors.bottom: parent.bottom; anchors.bottomMargin: -85
            anchors.horizontalCenter: hsiIndicator.horizontalCenter
            width: 10; height: 250
            rotation: 90
            diff: -(autopilot.planeX - 5.0) * 10
        }
    }
}
