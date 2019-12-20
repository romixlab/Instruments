#ifndef AUTOPILOTMAPLAYER_H
#define AUTOPILOTMAPLAYER_H
#include <QQuickPaintedItem>
#include <QPainterPath>

class Autopilot;
class AutopilotMapLayer : public QQuickPaintedItem
{
    Q_OBJECT
public:
    AutopilotMapLayer(QQuickItem *parent = 0);

    void paint(QPainter *p);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    friend class Autopilot;

    typedef struct {
        QPointF coordinate;
        qreal altitude;
        qreal speed;
        bool visible;
    } waypoint_t;
    QList<waypoint_t> m_wayPoints;

    QPointF m_x;

    QPainterPath m_gpsPath;
    QVector<QPointF> m_gpsPathPoints;
};

#endif // AutopilotMapLayer_H
