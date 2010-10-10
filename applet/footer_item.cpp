#include "footer_item.h"

#include <Plasma/Theme>

#include <QtGui>

//-----------------------------------------------------------------------------
// public:
//-----------------------------------------------------------------------------
FooterItem::FooterItem(QGraphicsItem* parent)
    :
    QGraphicsWidget(parent)
{
    QFont font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DefaultFont);
    QFontMetrics fm(font);
    
    const double height = fm.height() * 1.4;
    setMinimumHeight(height);
    setMaximumHeight(height);
}

//-----------------------------------------------------------------------------
void
FooterItem::paint(QPainter* painter, 
                 const QStyleOptionGraphicsItem* option,
                 QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QRectF frame(QPoint(0, 0), geometry().size());

    // paint the background
    QGradientStops stops;
    stops.append(QGradientStop(0.0, QColor(40, 57, 74)));
    stops.append(QGradientStop(0.5, QColor(0, 0, 0)));
    stops.append(QGradientStop(1.0, QColor(10, 16, 23)));

    QLinearGradient gradient(frame.topLeft(), frame.bottomLeft());
    gradient.setStops(stops);
   
    painter->setPen(QColor(56, 73, 93));
    painter->setBrush(QBrush(gradient));

    QPainterPath path;
    path.moveTo(40, frame.bottom());
    path.arcTo(QRectF(0, frame.bottom() - 40, 40, 40), -90, -90);
    path.lineTo(0, 0);
    path.lineTo(frame.right(), 0);
    path.lineTo(frame.right(), frame.bottom() - 20);
    path.arcTo(QRectF(frame.right() - 40, frame.bottom() - 40, 40, 40), 0, -90);
    path.closeSubpath();

    painter->drawPath(path);

    painter->setPen(Qt::white);
    painter->drawText(frame, Qt::AlignCenter, m_time);
}
//-----------------------------------------------------------------------------
// public slots:
//-----------------------------------------------------------------------------
void
FooterItem::dataUpdated(const QString& source, const Plasma::DataEngine::Data& data)
{
    Q_UNUSED(source);
    m_time = data["time"].toDateTime().toString("h:mmap, ddd MMMM d");

    update();
}

#include "footer_item.moc"

