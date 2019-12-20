#ifndef LINEARINDICATOR_H
#define LINEARINDICATOR_H
#include <QtQuick/QQuickPaintedItem>

class ColorSegment : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color MEMBER m_color)
    Q_PROPERTY(double from MEMBER m_from)
    Q_PROPERTY(double to MEMBER m_to)
public:
    ColorSegment(QObject *parent = 0);

    QColor m_color;
    double m_from;
    double m_to;
};

//Q_DECLARE_METATYPE(ColorSegment)

class LinearIndicator : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(qreal current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(QColor tickmarkColor READ tickmarkColor WRITE setTickmarkColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(qreal tickmarksStepSize READ tickmarksStepSize WRITE setTickmarksStepSize)
    Q_PROPERTY(qreal tickmarksStepValue READ tickmarksStepValue WRITE setTickmarksStepValue)
    Q_PROPERTY(qreal tickmarksEach READ tickmarksEach WRITE setTickmarksEach)
    Q_PROPERTY(qreal wrap READ wrap WRITE setWrap)
    Q_PROPERTY(QFont font READ font WRITE setFont)
    Q_PROPERTY(QQmlListProperty<ColorSegment> segments READ segments)
    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation)
public:
    LinearIndicator(QQuickItem *parent = 0);
    void paint(QPainter *painter);

    void setTarget(qreal target);
    double target() const;

    void setCurrent(qreal current);
    double current() const;

    QColor tickmarkColor() const;
    void setTickmarkColor(const QColor &tickmarkColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    qreal tickmarksStepSize() const;
    void setTickmarksStepSize(const qreal &tickmarksStepSize);

    qreal wrap() const;
    void setWrap(qreal wrap);

    QQmlListProperty<ColorSegment> segments();

    QFont font() const;
    void setFont(const QFont &font);

    qreal tickmarksStepValue() const;
    void setTickmarksStepValue(const qreal &tickmarksStepValue);

    qreal tickmarksEach() const;
    void setTickmarksEach(const qreal &tickmarksEach);

    enum Orientation {
        HorizontalTop,
        HorizontalBottom,
        VerticalLeft,
        VerticalRight
    };
    Q_ENUMS(Orientation)

    Orientation orientation() const;
    void setOrientation(const Orientation &orientation);

signals:
    void targetChanged();
    void currentChanged();

private:
    static void append_segment(QQmlListProperty<ColorSegment> *list, ColorSegment *segment);

    void drawLabel(QPainter *p, QString label, qreal x, qint32 height, qint32 halfTextHeight);

    qreal   m_target;
    qreal   m_current;
    QColor  m_tickmarkColor;
    QColor  m_backgroundColor;
    qreal   m_tickmarksStepSize;
    qreal    m_wrap;
    quint8  m_gradientWidth;
    quint8  m_tickmarksWidth;
    QFont   m_font;
    qreal   m_tickmarksStepValue;
    qreal   m_tickmarksEach;
    QList<ColorSegment *> m_segments;
    Orientation m_orientation;
};

#endif // LINEARINDICATOR_H
