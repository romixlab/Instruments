#ifndef ARTIFICIALHORIZON_H
#define ARTIFICIALHORIZON_H
#include <QQuickPaintedItem>

class ArtificialHorizon : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(double roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(double pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
public:
    ArtificialHorizon(QQuickItem *parent = 0);

    void paint(QPainter *p);

    double roll() const;
    void setRoll(double roll);

    double pitch() const;
    void setPitch(double pitch);

    double pitchScale() const;
    void setPitchScale(double pitchScale);

signals:
    void rollChanged();
    void pitchChanged();

private:
    void drawHorizon(QPainter *p, qreal ovlpitch, qreal w2, qreal h2);
    void drawMarks(QPainter *p, qreal radius, qreal ovlpitch, int pitchCutOff);

    double m_roll;
    double m_pitch;
    double m_pitchScale;
    QColor m_skyColor;
    QColor m_groundColor;
};

#endif // ARTIFICIALHORIZON_H
