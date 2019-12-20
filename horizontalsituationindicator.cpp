#include "horizontalsituationindicator.h"
#include <QPainter>

HorizontalSituationIndicator::HorizontalSituationIndicator(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_heading(350),
    m_magneticHeading(355),
    m_glidePathShift(0.5),
    m_glidePath(315),
    m_ilsHeading(305)
{
}

void HorizontalSituationIndicator::paint(QPainter *p)
{
    qreal w2 = boundingRect().width() / 2;
    qreal h2 = boundingRect().height() / 2;
    p->translate(w2, h2);
    p->setRenderHint(QPainter::Antialiasing, true);
    p->setRenderHint(QPainter::TextAntialiasing, true);

    QPen tickmarkPen(Qt::white, 2);
    QPen boldTickmarkPen(Qt::white, 3);
    //! [circles]
    p->setPen(tickmarkPen);
    qreal radius = w2 - 25;
    p->drawEllipse(QRectF(-radius, -radius, radius * 2, radius * 2));
    qreal innerRadius = radius / 2;
    p->setPen(QPen(Qt::white, 1, Qt::DashLine));
    p->drawEllipse(QRectF(-innerRadius, -innerRadius, innerRadius * 2, innerRadius * 2));
    //! [circles]
    //! [marks]
    QFont font("Ubuntu", 12);
    font.setBold(true);
    p->setFont(font);
    int textHeight = p->fontMetrics().tightBoundingRect("0").height();
    p->save();
    p->rotate(-m_heading);
    for (int a = 0; a <= 360; a += 5) {
        if (a % 30 == 0) {
            p->setPen(boldTickmarkPen);
            p->drawLine(QPointF(0, radius + 1), QPointF(0, radius + 5));
            int aa = a;
            if (aa == 360)
                aa = 0;
            aa /= 10;
            QString label = QString("%1").arg(aa);
            p->drawText(QPointF(- p->fontMetrics().width(label) / 2, -radius - textHeight), label);
        } else {
            p->setPen(tickmarkPen);
            p->drawLine(0, radius + 1, 0, radius + 3);
        }
        p->rotate(5);
    }
    p->restore();
    //! [marks]
    //! [plane]
    p->setPen(QPen(Qt::yellow, 2));
    p->drawLine(0, -20, 0, 20);
    p->drawLine(-13, -5, 13, -5);
    p->drawLine(-5, 15, 5, 15);
    p->setPen(QPen(Qt::yellow, 5));
    p->drawLine(QPointF(0, -radius + 7), QPointF(0, -radius - 3));
    //! [plane]
    //! [magnetic heading]
    p->save();
    p->rotate(m_magneticHeading - m_heading);
    p->setPen(QPen(Qt::cyan, 2));
    p->drawLine(0, -radius, -6, -radius - 8);
    p->drawLine(0, -radius, 6, -radius - 8);
    p->restore();
    //! [magnetic heading]
    //! [glide path]
    p->save();
    p->rotate(m_glidePath - m_heading);
    p->setPen(QColor("#00a2e8"));
    p->drawLine(0, radius, 0, innerRadius);
    p->drawLine(0, -innerRadius, 0, -radius);
    p->translate(0, -radius * 0.85);
    p->drawLine(0, 0, 7, 10);
    p->drawLine(0, 0, -7, 10);
    p->drawLine(-7, 10, 7, 10);
    p->translate(0, radius * 0.85);
    qreal glidePathShift = m_glidePathShift > 1.0 ? 1.0 : m_glidePathShift;
    p->drawLine(QPointF(glidePathShift * innerRadius, -innerRadius),
                QPointF(glidePathShift * innerRadius, innerRadius));
    p->restore();
    //! [glide path]
    //! [ils heading]
    p->save();
    p->rotate(m_ilsHeading);
    p->setPen(Qt::white);
    p->drawLine(0, radius, 0, -radius);
    p->translate(0, -radius * 0.85);
    p->drawLine(0, 0, 7, 10);
    p->drawLine(0, 0, -7, 10);
    p->drawLine(-7, 10, 7, 10);
    p->translate(0, 2 * radius * 0.85);
    p->drawLine(0, 0, 7, 10);
    p->drawLine(0, 0, -7, 10);
    p->drawLine(-7, 10, 7, 10);
    p->restore();
    //! [ils heading]
}

double HorizontalSituationIndicator::heading() const
{
    return m_heading;
}

void HorizontalSituationIndicator::setHeading(double heading)
{
    m_heading = heading;
    update();
    emit headingChanged();
}

double HorizontalSituationIndicator::magneticHeading() const
{
    return m_magneticHeading;
}

void HorizontalSituationIndicator::setMagneticHeading(double magneticHeading)
{
    m_magneticHeading = magneticHeading;
}

double HorizontalSituationIndicator::glidePathShift() const
{
    return m_glidePathShift;
}

void HorizontalSituationIndicator::setGlidePathShift(double glidePathShift)
{
    m_glidePathShift = glidePathShift;
}

double HorizontalSituationIndicator::glidePath() const
{
    return m_glidePath;
}

void HorizontalSituationIndicator::setGlidePath(double glidePath)
{
    m_glidePath = glidePath;
}

double HorizontalSituationIndicator::ilsHeading() const
{
    return m_ilsHeading;
}

void HorizontalSituationIndicator::setIlsHeading(double ilsHeading)
{
    m_ilsHeading = ilsHeading;
}
