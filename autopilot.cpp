#include "autopilot.h"
#include <cmath>
#include <QTimer>
#include <QDebug>

#include "autopilotmaplayer.h"
#include "horizontalsituationindicator.h"

Autopilot::Autopilot(QObject *parent) : QObject(parent),
  m_planeX(4.9), m_planeY(2.5), m_planeHeading(-2), m_planeSpeed(0.0),
  m_athrEnabled(false), m_speedEnabled(false), m_athrStopping(false),
  m_speedPID(0.3, 0, 30), m_headingPID(3, 0, 6), m_headingEnabled(false), m_headingStopping(false),
  m_gpsEnabled(false), m_pathAttractorPID(300, 0, 90000), m_mapLayer(0), m_gpsCaptured(false),
  m_ilsEnabled(false), m_ap1Enabled(false), m_altitudeEnabled(false), m_verticalSpeedEnabled(false),
  m_altitude(4500), m_verticalSpeed(0), m_targetVerticalSpeed(1000), m_altitudeStopping(false),
  m_altitudePID(0.3, 0, 30)
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000 / 60);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, &Autopilot::simulationStep);
    timer->start();

    const qreal accelerationMax = 0.05;
    m_speedPID.getOutput(0, 0);
    m_speedPID.setOutputRampRate(accelerationMax / 180);
    m_speedPID.setOutputLimits(-accelerationMax, accelerationMax);

    m_headingPID.getOutput(0, 0);
    m_headingPID.setOutputRampRate(25.0 / 10 / 60);
    m_headingPID.setOutputLimits(-25, 25);

    m_pathAttractorPID.getOutput(0, 0);
    m_pathAttractorPID.setOutputRampRate(50.0 / 10 / 60);
    m_pathAttractorPID.setOutputLimits(-15, 15);
    //m_pathAttractorPID.setOutputFilter(0.1);

    m_altitudePID.getOutput(0, 0);
    m_altitudePID.setOutputRampRate(10.0 / 10 / 60);
    m_altitudePID.setOutputLimits(-5, 5);
}

qreal Autopilot::planeX() const
{
    return m_planeX;
}

qreal Autopilot::planeY() const
{
    return m_planeY;
}

qreal Autopilot::planeHeading() const
{
    return m_planeHeading;
}

qreal Autopilot::planeSpeed() const
{
    return m_planeSpeed;
}

qreal Autopilot::pitch() const
{
    return m_pitch;
}

qreal Autopilot::roll() const
{
    return m_roll;
}

qreal Autopilot::heading() const
{
    return m_planeHeading;
}

qreal Autopilot::altitude() const
{
    return m_altitude;
}

qreal Autopilot::verticalSpeed() const
{
    return m_verticalSpeed;
}

void Autopilot::speedButtonClicked(bool enabled)
{
    m_speedEnabled = enabled;
    if (!enabled && std::fabs(m_planeSpeed - m_speedPID.getSetpoint()) > 1) {
        m_oldTargetSpeed = m_speedPID.getSetpoint();
        m_speedPID.setSetpoint(m_planeSpeed);
        m_athrStopping = true;
    }
}

void Autopilot::targetSpeedChanged(qreal speed)
{
    m_speedPID.setSetpoint(speed);
}

void Autopilot::headingButtonClicked(bool enabled)
{
    m_headingEnabled = enabled;
    if (!enabled && std::fabs(m_planeHeading - m_headingPID.getSetpoint()) > 1) {
        m_oldTargetHeading = m_headingPID.getSetpoint();
        if (m_roll > 0)
            m_headingPID.setSetpoint(m_planeHeading - 15);
        else
            m_headingPID.setSetpoint(m_planeHeading + 15);
        m_headingStopping = true;
    }
}

void Autopilot::setTargetHeading(qreal heading)
{
    //qDebug() << "tgt heading:" << heading;
    m_headingPID.setSetpoint(heading);
}

void Autopilot::altitudeButtonClicked(bool enabled)
{
    m_altitudeEnabled = enabled;
}

void Autopilot::verticalSpeedButtonClicked(bool enabled)
{
    m_verticalSpeedEnabled = enabled;
    if (enabled) {
        qreal maxPitch = m_targetVerticalSpeed / 1000 * 5;
        m_altitudePID.setOutputLimits(-maxPitch, maxPitch);
    } else {
        m_altitudePID.setOutputLimits(-5, 5);
    }
}

void Autopilot::setTargetAltitude(qreal altitude)
{
    m_targetAltitude = altitude;
}

void Autopilot::setTargetVerticalSpeed(qreal speed)
{
    m_targetVerticalSpeed = speed;
    if (m_verticalSpeedEnabled) {
        qreal maxPitch = m_targetVerticalSpeed / 1000 * 5;
        m_altitudePID.setOutputLimits(-maxPitch, maxPitch);
    }
}

void Autopilot::ap1ButtonClicked(bool enabled)
{
    m_ap1Enabled = enabled;
    if (!enabled) {
        m_altitudePID.setSetpoint(m_altitude);
        m_altitudeStopping = true;

        m_headingPID.setSetpoint(m_planeHeading);
        m_headingStopping = true;
    }
}

void Autopilot::athrConfig(bool enabled)
{
    m_athrEnabled = enabled;
    if (!enabled && std::fabs(m_planeSpeed - m_speedPID.getSetpoint()) > 1) {
        m_oldTargetSpeed = m_speedPID.getSetpoint();
        m_speedPID.setSetpoint(m_planeSpeed);
        m_athrStopping = true;
    }
}

void Autopilot::gpsButtonClicked(bool enabled)
{
    m_gpsEnabled = enabled;
    if (enabled) {

    } else {
        m_altitudeEnabled = false;
        m_altitudeStopping = true;
    }
}

void Autopilot::ilsButtonClicked(bool enabled)
{
    m_ilsEnabled = enabled;
    if (enabled) {
        m_altitudeEnabled = true;
        m_hsi->setGlidePath(0);
    } else {
        m_altitudeStopping = true;
        m_targetAltitude = m_altitude;
    }
}

void Autopilot::setMapLayer(QObject *mapLayer)
{
    m_mapLayer = qobject_cast<AutopilotMapLayer *>(mapLayer);
}

void Autopilot::setHSI(QObject *hsi)
{
    m_hsi = qobject_cast<HorizontalSituationIndicator *>(hsi);
}

void Autopilot::situationLoad(const QString &situation)
{
    if (situation == QStringLiteral("S1")) {
        m_planeX = 4.9; m_planeY = 2; m_planeHeading = -0.5;
        m_altitude = 2800; m_planeSpeed = 130;
    } else if (situation == QStringLiteral("S2")) {
        m_planeX = 1.5; m_planeY = 1.7; m_planeHeading = 90;
        m_altitude = 7000; m_planeSpeed = 150;
    } else if (situation == QStringLiteral("S3")) {
        m_planeX = 1.5; m_planeY = 1.5; m_planeHeading = 95;
        m_altitude = 7000; m_planeSpeed = 150;
    } else if (situation == QStringLiteral("S4")) {
        m_planeX = 3.5; m_planeY = 1.5; m_planeHeading = 45;
        m_altitude = 5000; m_planeSpeed = 150;
    }

    m_speedPID.setSetpoint(m_planeSpeed);
    emit planeXChanged();
    emit planeYChanged();
    emit headingChanged();
    emit altitudeChanged();
    emit planeSpeedChanged();
}

void Autopilot::simulationStep()
{
    const int stepsPerSecond = 60;
    if ((m_athrEnabled && m_speedEnabled) || m_athrStopping) {
        qreal acceleration = m_speedPID.getOutput(m_planeSpeed);
        m_planeSpeed += acceleration;
        emit planeSpeedChanged();

        if (m_athrStopping && (std::fabs(m_planeSpeed - m_speedPID.getSetpoint()) < 0.1)) {
            m_athrStopping = false;
            m_speedPID.setSetpoint(m_oldTargetSpeed);
        }
    }

    if (m_ap1Enabled && (m_headingEnabled || m_headingStopping)) {
        m_roll = m_headingPID.getOutput(m_planeHeading);
        m_planeHeading += m_roll / 25 / 60 * 1.8;
        m_pitch = std::fabs(m_roll) / 25 * 2.5;
        emit rollChanged();
        emit pitchChanged();
        emit headingChanged();

        //qDebug() << "tgt:" << m_headingPID.getSetpoint() << "act:" << m_planeHeading << "roll" << m_roll;

        if (m_headingStopping && (std::fabs(m_roll) < 0.1)) {
            m_headingStopping = false;
            m_headingPID.setSetpoint(m_oldTargetHeading);
        }
    }

    if (m_ap1Enabled && m_gpsEnabled) {
        qreal minDistance = 1e9;
        QPointF closestPoint;
        qreal positionOnPath;
        for (qreal x = 0; x <= 1; x += 0.005) {
            QPointF pointOnPath = m_mapLayer->m_gpsPath.pointAtPercent(x);
            qreal distance = std::pow(pointOnPath.x() / 100 - m_planeX, 2) +
                    std::pow(pointOnPath.y() / 100 - m_planeY, 2);
            if (distance < minDistance) {
                minDistance = distance;
                closestPoint = pointOnPath;
                positionOnPath = x;
            }
        }

        if (positionOnPath > 0.01 && positionOnPath <= 0.98) {
            m_mapLayer->m_x = closestPoint;
            m_mapLayer->update();

            QPointF nextToClosestPoint = m_mapLayer->m_gpsPath.pointAtPercent(positionOnPath + 0.005);
            QPointF dp = nextToClosestPoint - closestPoint;
            QPointF dpp = QPointF(m_planeX, m_planeY) - closestPoint / 100;
            qreal curvature = std::atan2(dp.y(), dp.x()) / 2 / M_PI * 360;
            qreal curvaturePlane = std::atan2(dpp.y(), dpp.x()) / 2 / M_PI * 360;
            qreal da = curvature - curvaturePlane;

            qreal dx = std::sqrt(std::pow(m_planeX - closestPoint.x() / 100, 2) +
                                 std::pow(m_planeY - closestPoint.y() / 100, 2));

            dx *= (std::fabs(da - 90) < 20) ? 1 : -1;
            qreal pathAttract = m_pathAttractorPID.getOutput(dx / 3, 0);

            //qDebug() << "dx" << dx << "\tattr" << pathAttract;// << "\tcurv" << curvature << "\tpl_curv" << curvaturePlane;

            m_hsi->setGlidePathShift(-dx);
            m_hsi->setGlidePath(90 - curvature);

            m_roll = m_headingPID.getOutput(m_planeHeading, -curvature + 90 + pathAttract);

            m_planeHeading += m_roll / 25 / 60 * 1.8;
            emit rollChanged();
            emit headingChanged();

            if (positionOnPath >= 0 && positionOnPath <= 0.37) {
                m_targetAltitude = 7000 - positionOnPath / 0.37 * 3000;
            } else if (positionOnPath > 0.37 && positionOnPath <= 0.53) {
                m_targetAltitude = 4000 - ((positionOnPath - 0.37) / 0.16) * 1000;
            } else if (positionOnPath > 0.53 && positionOnPath <= 0.72) {
                m_targetAltitude = 3000 - ((positionOnPath - 0.53) / 0.19) * 500;
            } else {
                m_targetAltitude = 2500 - ((positionOnPath - 0.72) / 0.28) * 2500;
            }
            m_altitudeEnabled = true;

            if (positionOnPath > 0.95) {
                m_gpsCaptured = false;
                m_gpsEnabled = false;
                m_altitudeStopping = true;
                m_targetAltitude = 0;
                m_speedPID.setSetpoint(0);
                m_athrStopping = true;
                qDebug() << "GPS Release";
            }
        }
    }

    if (m_ap1Enabled && m_ilsEnabled) {
        qreal dx = m_planeX - 5.0;
        m_roll = m_pathAttractorPID.getOutput(dx, 0);
        m_planeHeading += m_roll / 25 / 60 * 1.8;
        emit rollChanged();
        emit headingChanged();

       // qDebug() << dx << m_roll;

        m_hsi->setGlidePathShift(-dx);

        m_targetAltitude = (5 - m_planeY) / 1.5 * 2500;
        if (m_targetAltitude < 0)
            m_targetAltitude = 0;
        if (m_targetAltitude > 2500)
            m_targetAltitude = 2500;
        if (m_altitude <= 50) {
            m_speedPID.setSetpoint(0);
            m_athrStopping = true;
        }
    }

    if (m_ap1Enabled && (m_altitudeEnabled || m_altitudeStopping)) {
        m_pitch = m_altitudePID.getOutput(m_altitude, m_targetAltitude);
        m_verticalSpeed = 300 * m_pitch * m_planeSpeed / 3600 / stepsPerSecond;
        m_altitude += m_verticalSpeed;

        if (m_altitude < 0)
            m_altitude = 0;

        if (m_altitudeStopping && std::fabs(m_altitude - m_targetAltitude) < 5) {
            m_altitudeStopping = false;
            m_pitch = 0;
            m_verticalSpeed = 0;
        }
        emit pitchChanged();
        emit altitudeChanged();
        emit verticalSpeedChanged();
    }

    qreal ilsHeading = std::atan2(5.0 - m_planeY, 5.0 - m_planeX) / 2 / M_PI * 360;
    ilsHeading = -ilsHeading + 90;
    m_hsi->setIlsHeading(ilsHeading);

    qreal dspeed = m_planeSpeed / 3600 / stepsPerSecond;
    qreal dspeedx = std::fabs(dspeed) * std::cos((m_planeHeading + 90) * 2 * M_PI / 360.0);
    qreal dspeedy = std::fabs(dspeed) * std::cos(m_planeHeading * 2 * M_PI / 360.0);
    m_planeX -= dspeedx;
    m_planeY += dspeedy;
    emit planeXChanged();
    emit planeYChanged();
}
