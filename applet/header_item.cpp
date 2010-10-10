#include "header_item.h"

#include <Plasma/Theme>

#include <QtGui>

namespace
{
    const QString title = "Stock Performance";
}

//-----------------------------------------------------------------------------
// public:
//-----------------------------------------------------------------------------
HeaderItem::HeaderItem(QGraphicsItem* parent)
    :
    QGraphicsWidget(parent)
{
    QFont font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DefaultFont);
    font.setBold(true);
    font.setPointSize(static_cast<int>(font.pointSize() * 1.4));
    QFontMetrics fm(font);

    const QSize size = fm.size(Qt::TextSingleLine, title);
    setMinimumWidth(size.width() * 1.4);
    setMinimumHeight(size.height() * 1.4);
    setMaximumHeight(size.height() * 1.4);
}

//-----------------------------------------------------------------------------
void
HeaderItem::paint(QPainter* painter, 
                 const QStyleOptionGraphicsItem* option,
                 QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(static_cast<int>(font.pointSize() * 1.4));
    painter->setFont(font);

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
    path.moveTo(0, 20);
    path.arcTo(0, 0, 40, 40, 180, -90);
    path.lineTo(frame.right() - 20, 0);
    path.arcTo(frame.right() - 40, 0, 40, 40, 90, -90);
    path.lineTo(frame.right(), frame.bottom());
    path.lineTo(0, frame.bottom());
    path.lineTo(0, 20);
    path.closeSubpath();

    painter->drawPath(path);

    painter->setPen(Qt::white);
    painter->drawText(frame, Qt::AlignCenter, title);
}
