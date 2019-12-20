#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include "MiniPID.h"

class AutopilotMapLayer;
class HorizontalSituationIndicator;
class Autopilot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal planeX READ planeX NOTIFY planeXChanged)
    Q_PROPERTY(qreal planeY READ planeY NOTIFY planeYChanged)
    Q_PROPERTY(qreal planeHeading READ planeHeading NOTIFY planeHeadingChanged)
    Q_PROPERTY(qreal planeSpeed READ planeSpeed NOTIFY planeSpeedChanged)
    Q_PROPERTY(qreal pitch READ pitch NOTIFY pitchChanged)
    Q_PROPERTY(qreal roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(qreal heading READ heading NOTIFY headingChanged)
    Q_PROPERTY(qreal altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(qreal verticalSpeed READ verticalSpeed NOTIFY verticalSpeedChanged)
public:
    explicit Autopilot(QObject *parent = 0);

    void connectToUI();

    qreal planeX() const;
    qreal planeY() const;
    qreal planeHeading() const;
    qreal planeSpeed() const;
    qreal pitch() const;
    qreal roll() const;
    qreal heading() const;
    qreal altitude() const;
    qreal verticalSpeed() const;

signals:
   void planeXChanged();
   void planeYChanged();
   void planeHeadingChanged();
   void planeSpeedChanged();
   void pitchChanged();
   void rollChanged();
   void headingChanged();
   void altitudeChanged();
   void verticalSpeedChanged();

public slots:
    void speedButtonClicked(bool enabled);
    void targetSpeedChanged(qreal speed);
    void headingButtonClicked(bool enabled);
    void setTargetHeading(qreal heading);
    void altitudeButtonClicked(bool enabled);
    void verticalSpeedButtonClicked(bool enabled);
    void setTargetAltitude(qreal altitude);
    void setTargetVerticalSpeed(qreal speed);
    void ap1ButtonClicked(bool enabled);
    void athrConfig(bool enabled);
    void gpsButtonClicked(bool enabled);
    void ilsButtonClicked(bool enabled);

    void setMapLayer(QObject *mapLayer);
    void setHSI(QObject *hsi);

    void situationLoad(const QString &situation);

private slots:
    void simulationStep();

private:
    qreal m_planeX; // in miles
    qreal m_planeY;
    qreal m_planeHeading; // in degrees
    qreal m_planeSpeed;
    qreal m_pitch;
    qreal m_roll;
    qreal m_altitude;
    qreal m_verticalSpeed;

    bool m_athrEnabled;
    bool m_speedEnabled;
    MiniPID m_speedPID;
    bool m_athrStopping;
    qreal m_oldTargetSpeed;

    bool m_headingEnabled;
    MiniPID m_headingPID;
    bool m_headingStopping;
    qreal m_oldTargetHeading;

    AutopilotMapLayer *m_mapLayer;
    HorizontalSituationIndicator *m_hsi;
    bool m_gpsEnabled;
    MiniPID m_pathAttractorPID;
    bool m_gpsCaptured;

    bool m_ilsEnabled;

    bool m_ap1Enabled;
    bool m_altitudeEnabled;
    bool m_altitudeStopping;
    bool m_verticalSpeedEnabled;
    qreal m_targetAltitude;
    qreal m_targetVerticalSpeed;
    MiniPID m_altitudePID;
};

#endif // AUTOPILOT_H
