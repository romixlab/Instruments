#include "autopilotmaplayer.h"
#include <QPainter>
#include <QDebug>

#include <cmath>

AutopilotMapLayer::AutopilotMapLayer(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);

    m_wayPoints.append({QPoint(200, 150), 2500, 100, false});
    m_wayPoints.append({QPoint(400, 200), 2500, 100, false});
    m_wayPoints.append({QPoint(470, 250), 2500, 100, false});
    m_wayPoints.append({QPoint(500, 350), 2500, 100, false});

    m_gpsPath.moveTo(200, 150);
    m_gpsPath.quadTo(QPointF(300, 150), QPointF(400, 200));
    m_gpsPath.quadTo(QPointF(500, 240), QPointF(500, 350));
    m_gpsPath.lineTo(500, 500);

//    for (qreal x = 0; x <= 1; x += 0.01)
//        m_gpsPathPoints << m_gpsPath.pointAtPercent(x);
}

void AutopilotMapLayer::paint(QPainter *p)
{
    qreal w = boundingRect().width();
    qreal h = boundingRect().height();
    p->translate(0, h);
    p->scale(1, -1);

    auto drawTriangle = [p](qreal x, qreal y, qreal width) {
        p->drawLine(QPointF(x, y + width / 2), QPointF(x + width / 2, y - width / 2));
        p->drawLine(QPointF(x + width / 2, y - width / 2), QPointF(x - width / 2, y - width / 2));
        p->drawLine(QPointF(x - width / 2, y - width / 2), QPointF(x, y + width / 2));
    };

    p->setPen(QColor("#ff00ff"));
    foreach(const waypoint_t &wp, m_wayPoints) {
//        if (!wp.visible)
//            continue;
        drawTriangle(wp.coordinate.x(), wp.coordinate.y(), 10);
    }

    p->setPen(Qt::red);
    drawTriangle(m_x.x(), m_x.y(), 4);

    p->setPen(Qt::cyan);
    p->drawPath(m_gpsPath);
}

void AutopilotMapLayer::mousePressEvent(QMouseEvent *event)
{

}
