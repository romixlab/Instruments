#include "artificialhorizon.h"
#include <QPainter>

#include <QDebug>

#include <cmath>

ArtificialHorizon::ArtificialHorizon(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_roll(0), m_pitch(0), m_pitchScale(25.0),
    m_skyColor("#2977ef"), m_groundColor("#884600")
{
}

void ArtificialHorizon::paint(QPainter *p)
{
    qreal width = boundingRect().width();
    qreal height = boundingRect().height();
    qreal w2 = width / 2;
    qreal h2 = height / 2;
    const int pitchCutOff = 35;
    p->translate(w2, h2);
    p->translate(0, 10);
    qreal ovlpitch = m_pitch; // TODO wrap360

    //! [horizon]
    qreal radius = h2 - 10;
    drawHorizon(p, ovlpitch, w2, radius);
    p->setPen(QPen(Qt::white, 2));
    qreal alpha = std::asin((radius - pitchCutOff) / radius);
    p->drawLine(QPointF(-radius * std::cos(alpha) + 5, radius - pitchCutOff),
                QPointF(radius * std::cos(alpha) - 5, radius - pitchCutOff));
    p->drawLine(QPointF(-radius * std::cos(alpha) + 5, -radius + pitchCutOff),
                QPointF(radius * std::cos(alpha) - 5, -radius + pitchCutOff));
    //! [horizon]
    //! [marks]
    p->save();
    p->setClipRect(-radius, -radius + pitchCutOff, radius * 2, (radius - pitchCutOff) * 2);
    drawMarks(p, radius, ovlpitch, pitchCutOff);
    p->restore();
    //! [marks]
    //! [zero marks]
    auto zeroMark = [p, w2](){
        p->drawLine(QPointF(w2 - 10, 0), QPointF(w2 - 50, 0));
        p->drawLine(QPointF(w2 - 50, 0), QPointF(w2- 50, 12));
    };
    p->setPen(QPen(Qt::yellow, 8));
    zeroMark();
    p->setPen(QPen(Qt::black, 4));
    zeroMark();
    p->scale(-1, 1);
    p->setPen(QPen(Qt::yellow, 8));
    zeroMark();
    p->setPen(QPen(Qt::black, 4));
    zeroMark();
    p->scale(-1, 1);

    p->fillRect(-5, -5, 10, 10, Qt::yellow);
    p->fillRect(-3, -3, 6, 6, Qt::black);
    //! [zero marks]
    //! [roll marks]
    p->setPen(QPen(Qt::yellow, 2));
    p->drawLine(QPointF(0, -radius), QPointF(13,  -radius + 26));
    p->drawLine(QPointF(0, -radius), QPointF(-13, -radius + 26));
    p->drawLine(QPointF(-13, -radius + 26), QPointF(13, -radius + 26));
    p->drawLine(QPointF(-8, -radius + 16), QPointF(8, -radius + 16));

    p->rotate(m_roll);
    p->drawLine(QPointF(0, -radius), QPointF(-5, -radius - 10));
    p->drawLine(QPointF(0, -radius), QPointF(5, -radius - 10));
    p->drawLine(QPointF(-5, -radius - 10), QPointF(5, -radius - 10));

    p->setPen(QPen(Qt::white, 2));
    p->drawArc(QRectF(-radius, -radius, radius * 2, radius * 2), 45 * 16, 90 * 16);
    p->rotate(-45);
    for (int i = -45; i <= 45; i += 5, p->rotate(5)) {
        if (!((i % 45 == 0) || (i % 10 == 0)))
            continue;
        if (i % 40 == 0)
            continue;
        p->drawLine(QPointF(0, -radius), QPointF(0, -radius - 10));
    }
    //! [roll marks]
}

void ArtificialHorizon::drawHorizon(QPainter *p, qreal ovlpitch, qreal w2, qreal h2)
{
    qreal scpitch = ovlpitch;
    if (scpitch > (180 - m_pitchScale)) scpitch -= 180;
    if (scpitch < -(180 - m_pitchScale)) scpitch += 180;
    qreal h = -scpitch / m_pitchScale * h2;

    p->rotate(-m_roll);
    p->setPen(Qt::NoPen);
    p->setRenderHint(QPainter::Antialiasing, true);

    if ((scpitch > m_pitchScale) || (scpitch < -m_pitchScale)) {
        p->setBrush(ovlpitch > m_pitchScale ? m_skyColor : m_groundColor);
        QPainterPath path;
        path.arcTo(QRectF(-h2, -h2, h2 * 2, h2 * 2), 0, 16 * 360);
        p->drawPath(path);
    } else {
        qreal alpha = std::asin(h / h2);

        p->setBrush((ovlpitch > 150 || ovlpitch < -150) ? m_groundColor : m_skyColor);
        QPainterPath path;
        path.moveTo(0, -h);
        path.arcTo(QRectF(-h2, -h2, h2 * 2, h2 * 2),
                   alpha / M_PI * 180, 180 - 2 * alpha / M_PI * 180);
        p->drawPath(path);

        p->setBrush((ovlpitch > 150 || ovlpitch < -150) ? m_skyColor : m_groundColor);
        path = QPainterPath();
        path.moveTo(0, -h);
        path.arcTo(QRectF(-h2, -h2, h2 * 2, h2 * 2),
                   alpha / M_PI * 180, -180 - 2 * alpha / M_PI * 180);
        p->drawPath(path);

        p->setPen(QPen(Qt::white, 2));
        p->drawLine(QPointF(-h2 * std::cos(alpha) + 1, -h), QPointF(h2 * std::cos(alpha) - 1, -h));
    }
    p->rotate(m_roll);
}

void ArtificialHorizon::drawMarks(QPainter *p, qreal radius, qreal ovlpitch, int pitchCutOff)
{
    qreal pxPerDegree = radius / m_pitchScale;
    qreal dp = ovlpitch + m_pitchScale - pitchCutOff / pxPerDegree;
    qreal pitchMax = std::ceil(dp / 2.5) * 2.5;
    qreal ymax = ovlpitch * pxPerDegree - pitchMax * pxPerDegree;

    auto markWidth = [](qreal pitch) -> qreal{
        if (fmod(pitch, 10) == 0.0)
            return 28;
        if (fmod(pitch, 5.0) == 0.0)
            return 14;
        return 8; // 2.5
    };

    int textHeight2 = p->fontMetrics().tightBoundingRect("1").height() / 2;
    for (qreal y = ymax, pitch = pitchMax; y <= radius - pitchCutOff + pxPerDegree * 2.5; y += 2.5 * pxPerDegree, pitch -= 2.5) {
        if (pitch == 180 || pitch == 0 || pitch == -180)
            continue;

        qreal width = markWidth(pitch);
        p->drawLine(QPointF(-width, y), QPointF(width, y));

        qreal pp = std::fabs(pitch);
        if (pp > 180)
            pp -= 360;
        if (fmod(pp, 10) == 0) {
            QString label = QString("%1").arg(pp);
            int textWidth = p->fontMetrics().width(label);
            p->drawText(QPointF(-width - textWidth - 5, y + textHeight2), label);
            p->drawText(QPointF(width + 5, y + textHeight2), label);
        }
    }
}

double ArtificialHorizon::roll() const
{
    return m_roll;
}

void ArtificialHorizon::setRoll(double roll)
{
    m_roll = roll;
    update();
    emit rollChanged();
}

double ArtificialHorizon::pitch() const
{
    return m_pitch;
}

void ArtificialHorizon::setPitch(double pitch)
{
    m_pitch = pitch;
    update();
    emit pitchChanged();
}

double ArtificialHorizon::pitchScale() const
{
    return m_pitchScale;
}

void ArtificialHorizon::setPitchScale(double pitchScale)
{
    m_pitchScale = pitchScale;
    update();
}

