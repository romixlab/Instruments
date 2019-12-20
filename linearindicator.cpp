#include "linearindicator.h"

#include "painterhelpers.h"

#include <QPainter>
#include <cmath>

#include <QDebug>

ColorSegment::ColorSegment(QObject *parent) : QObject(parent)
{
}

LinearIndicator::LinearIndicator(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_target(std::nan("1")),
    m_current(0),
    m_tickmarkColor(Qt::white),
    m_backgroundColor(Qt::black),
    m_tickmarksStepSize(10),
    m_wrap(0),
    m_gradientWidth(4),
    m_tickmarksWidth(10),
    m_tickmarksStepValue(1.0),
    m_tickmarksEach(5.0),
    m_orientation(HorizontalTop)
{
}

void LinearIndicator::paint(QPainter *p)
{
    //p->setRenderHint(QPainter::Antialiasing, true);
    p->setRenderHint(QPainter::TextAntialiasing, true);
    p->setFont(m_font);
    p->setBrush(Qt::NoBrush);

    qint32 width = (qint32)boundingRect().width();
    qint32 height = (qint32)boundingRect().height();
    qint32 gradientWidth = m_segments.isEmpty() ? 0 : m_gradientWidth;

    if (m_orientation == HorizontalBottom) {
        p->translate(0, height);
        p->scale(1, -1);
        gradientWidth += 1;
    } else if (m_orientation == VerticalRight) {
        p->rotate(90);
        p->translate(0, -width);
        p->scale(-1, 1);
        p->translate(-height, 0);
        qint32 t = width; width = height; height = t;
        gradientWidth += gradientWidth == 0 ? 1 : 0;
    } else if (m_orientation == VerticalLeft) {
        p->rotate(-90);
        p->translate(-height, 0);
        qint32 t = width; width = height; height = t;
    }

    //! [axis]
    p->setPen(m_tickmarkColor.darker());
    p->drawLine(QPointF(0, gradientWidth), QPointF(width, gradientWidth));
    QColor zeroColor(Qt::yellow);
    zeroColor.setAlphaF(0.7);
    QPen zeroPen(zeroColor, 3);
    p->setPen(zeroPen);
    p->drawLine(width / 2, gradientWidth + 5, width / 2, height);
    //! [axis]
    //! [tickmarks, labels]
    p->setPen(m_tickmarkColor);
    qreal dummy;
    qreal vmin = m_current - (width / 2) / m_tickmarksStepSize * m_tickmarksStepValue;
    qreal vminShift = std::fmod(vmin, m_tickmarksStepValue);
    qreal xmin = (m_tickmarksStepValue - vminShift) / m_tickmarksStepValue * m_tickmarksStepSize;
    xmin -= m_tickmarksStepSize;
    qint32 halfTextHeight = p->fontMetrics().tightBoundingRect("123").height() / 2;
    for (qreal x = xmin, v = vmin - vminShift; x < width + m_tickmarksStepSize; x += m_tickmarksStepSize, v += m_tickmarksStepValue) {
        p->drawLine(QPointF(x, gradientWidth + 5), QPointF(x, gradientWidth + 5 + m_tickmarksWidth));
        if (modf(v / m_tickmarksEach, &dummy) == 0) {
            qreal wrapped = instruments::wraphalf(v, m_wrap);
            QString label = QString("%1").arg(wrapped);
            drawLabel(p, label, x, height, halfTextHeight);
        }
    }
    //! [tickmarks, labels]
    //! [segments]
    qreal vmax = vmin + width / m_tickmarksStepSize * m_tickmarksStepValue;
    QLinearGradient colorGradient(0, 0, 0, m_gradientWidth);
    colorGradient.setColorAt(0, Qt::transparent);
    foreach (ColorSegment *segment, m_segments) {
        qreal smin = segment->m_from;
        qreal smax = segment->m_to;
        if ( (smin <= vmin && smax <= vmin) || (smin >= vmax && smax >= vmax) )
            continue;
        if (smin < vmin) smin = vmin;
        if (smax > vmax) smax = vmax;

        colorGradient.setColorAt(1, segment->m_color);
        p->fillRect(QRectF(QPointF((smin - vmin) / m_tickmarksStepValue * m_tickmarksStepSize,
                           0),
                           QPointF((smax - vmin) / m_tickmarksStepValue * m_tickmarksStepSize,
                           m_gradientWidth)),
                    colorGradient);
    }
    //! [segments]
    //! [edge shadow]
    QLinearGradient gradient(0, 0, 20, 0);
    gradient.setColorAt(0, QColor(m_backgroundColor));
    gradient.setColorAt(1, Qt::transparent);
    p->fillRect(0, 0, 20, height, gradient);
    p->translate(width, 0);
    p->scale(-1, 1);
    p->fillRect(0, 0, 20, height, gradient);
    //! [edge shadow]
}

void LinearIndicator::drawLabel(QPainter *p, QString label, qreal x, qint32 height, qint32 halfTextHeight)
{
    QFontMetrics fontMetrics = p->fontMetrics();

    if (m_orientation == HorizontalTop) {
        p->drawText(QPointF(x - fontMetrics.width(label) / 2, height), label);
    } else if (m_orientation == HorizontalBottom) {
        p->scale(1, -1);
        p->drawText(QPointF(x - fontMetrics.width(label) / 2, -height + halfTextHeight), label);
        p->scale(1, -1);
    } else if (m_orientation == VerticalRight) {
        p->rotate(-90);
        p->scale(1, -1);
        p->drawText(QPointF(-boundingRect().width(), -x + halfTextHeight), label);
        p->scale(1, -1);
        p->rotate(90);
    } else if (m_orientation == VerticalLeft) {
        p->rotate(90);
        p->drawText(QPointF(boundingRect().width() - fontMetrics.width(label), -x + halfTextHeight), label);
        p->rotate(-90);
    }
}

qreal LinearIndicator::target() const
{
    return m_target;
}

void LinearIndicator::setTarget(qreal target)
{
    m_target = target;
    emit targetChanged();
    update();
}

qreal LinearIndicator::current() const
{
    return m_current;
}

void LinearIndicator::setCurrent(qreal current)
{
    m_current = current;
    emit currentChanged();
    update();
}

QColor LinearIndicator::tickmarkColor() const
{
    return m_tickmarkColor;
}

void LinearIndicator::setTickmarkColor(const QColor &tickmarkColor)
{
    m_tickmarkColor = tickmarkColor;
    update();
}

QColor LinearIndicator::backgroundColor() const
{
    return m_backgroundColor;
}

void LinearIndicator::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}

qreal LinearIndicator::tickmarksStepSize() const
{
    return m_tickmarksStepSize;
}

void LinearIndicator::setTickmarksStepSize(const qreal &tickmarkStepSize)
{
    m_tickmarksStepSize = tickmarkStepSize;
    update();
}

qreal LinearIndicator::wrap() const
{
    return m_wrap;
}

void LinearIndicator::setWrap(qreal wrap)
{
    m_wrap = wrap;
    update();
}

QQmlListProperty<ColorSegment> LinearIndicator::segments()
{
    return QQmlListProperty<ColorSegment>(this, 0, &LinearIndicator::append_segment, 0, 0, 0);
}

void LinearIndicator::append_segment(QQmlListProperty<ColorSegment> *list, ColorSegment *segment)
{
    LinearIndicator *indicator = qobject_cast<LinearIndicator *>(list->object);
    if (indicator) {
        segment->setParent(indicator);
        indicator->m_segments.append(segment);
        indicator->update();
    }
}

LinearIndicator::Orientation LinearIndicator::orientation() const
{
    return m_orientation;
}

void LinearIndicator::setOrientation(const Orientation &orientation)
{
    m_orientation = orientation;
    update();
}

qreal LinearIndicator::tickmarksEach() const
{
    return m_tickmarksEach;
}

void LinearIndicator::setTickmarksEach(const qreal &tickmarksEach)
{
    m_tickmarksEach = tickmarksEach;
}

qreal LinearIndicator::tickmarksStepValue() const
{
    return m_tickmarksStepValue;
}

void LinearIndicator::setTickmarksStepValue(const qreal &tickmarksStepValue)
{
    m_tickmarksStepValue = tickmarksStepValue;
    update();
}

QFont LinearIndicator::font() const
{
    return m_font;
}

void LinearIndicator::setFont(const QFont &font)
{
    m_font = font;
    update();
}


