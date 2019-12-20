#ifndef HORIZONTALSITUATIONINDICATOR_H
#define HORIZONTALSITUATIONINDICATOR_H
#include <QQuickPaintedItem>

class HorizontalSituationIndicator : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal heading READ heading WRITE setHeading)
    Q_PROPERTY(qreal glidePathShift READ glidePathShift WRITE setGlidePathShift)
    Q_PROPERTY(qreal gligePath READ glidePath WRITE setGlidePath)
    Q_PROPERTY(qreal ilsHeading READ ilsHeading WRITE setIlsHeading)
public:
    HorizontalSituationIndicator(QQuickItem *parent = 0);
    void paint(QPainter *p);

    double heading() const;
    void setHeading(double heading);

    double magneticHeading() const;
    void setMagneticHeading(double magneticHeading);

    double glidePathShift() const;
    void setGlidePathShift(double glidePathShift);

    double glidePath() const;
    void setGlidePath(double glidePath);

    double ilsHeading() const;
    void setIlsHeading(double ilsHeading);

signals:
    void headingChanged();

private:
    double m_heading;
    double m_magneticHeading;
    double m_glidePathShift;
    double m_glidePath;
    double m_ilsHeading;
};

#endif // HORIZONTALSITUATIONINDICATOR_H
