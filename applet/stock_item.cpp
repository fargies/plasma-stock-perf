#include "stock_item.h"

#include <Plasma/Theme>

#include <KRun>

#include <QtGui>

namespace
{
    const QColor even_colour_0(26, 33, 42);
    const QColor even_colour_1(20, 26, 33);

    const QColor  odd_colour_0(12, 14, 18);
    const QColor  odd_colour_1(16, 20, 26);
}

//-----------------------------------------------------------------------------
// public:
//-----------------------------------------------------------------------------
StockItem::StockItem(bool even, QGraphicsItem* parent)
    :
    QGraphicsWidget(parent),
    m_even(even),
    m_hover(false),
    m_symbol(),
    m_name(),
    m_price(0.0),
    m_change(0.0),
    m_percent_change(0.0)
{
    setAcceptHoverEvents(true);

    QFont font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DefaultFont);
    QFontMetrics fm(font);

    setMinimumHeight(2 * fm.height());
    setMaximumHeight(2 * fm.height());
    setMinimumWidth(280);
}

//-----------------------------------------------------------------------------
void
StockItem::paint(QPainter* painter,
                 const QStyleOptionGraphicsItem* option,
                 QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QRectF frame(QPoint(0,0), geometry().size());

    // paint the background
    QGradientStops stops;
    QColor colour_0;
    QColor colour_1;

    if (m_hover)
    {
        colour_0 = even_colour_0.lighter(200);
        colour_1 = even_colour_1.lighter(200);
    }
    else if (m_even)
    {
        colour_0 = even_colour_0;
        colour_1 = even_colour_1;
    }
    else
    {
        colour_0 = odd_colour_0;
        colour_1 = odd_colour_1;
    }

    stops.append(QGradientStop(0.0, colour_0));
    stops.append(QGradientStop(1.0, colour_1));

    QLinearGradient gradient(frame.topLeft(), frame.bottomLeft());
    gradient.setStops(stops);

    painter->setPen(QColor(56, 73, 93));
    painter->setBrush(QBrush(gradient));
    painter->drawRect(frame);

    if (m_name.isEmpty()) return;

    frame.adjust(10, 1, -10, -1);

    // paint the text
    QString string;
    QTextStream stream(&string);

    if (m_symbol.contains("=X")) stream.setRealNumberPrecision(4);
    else                         stream.setRealNumberPrecision(2);

    stream.setRealNumberNotation(QTextStream::FixedNotation);
    QFont font(painter->font());

    // symbol
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(frame, Qt::AlignLeft  | Qt::AlignTop, m_symbol);

    // name
    font.setBold(false);
    painter->setFont(font);
    painter->drawText(frame, Qt::AlignLeft  | Qt::AlignBottom, m_name);

    // price
    stream << m_price;
    painter->drawText(frame, Qt::AlignRight | Qt::AlignTop, string);

    if (!m_symbol.contains("=X"))
    {
        // change
        stream.setRealNumberPrecision(2);
        string.clear();
        stream.setNumberFlags(QTextStream::ForceSign);
        stream << m_change << " (" << m_percent_change << "%)";
        painter->setPen(m_change < 0 ? Qt::red : Qt::green);
        painter->drawText(frame, Qt::AlignRight | Qt::AlignBottom, string);
    }
}

//-----------------------------------------------------------------------------
// public slots:
//-----------------------------------------------------------------------------
void
StockItem::dataUpdated(const QString&                  source,
                       const Plasma::DataEngine::Data& data)
{
    Q_UNUSED(source);

    m_symbol         = data["symbol"].toString();
    m_name           = data["name"].toString();
    m_price          = data["price"].toDouble();
    m_change         = data["change"].toDouble();
    m_percent_change = data["percent_change"].toDouble();

    update();
}

//-----------------------------------------------------------------------------
// protected:
//-----------------------------------------------------------------------------
void
StockItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (Qt::LeftButton == event->button())
    {
        const QString url = "http://finance.yahoo.com/q?s=" + m_symbol;
        KRun::runUrl(url, "text/html", 0);
    }
}

//-----------------------------------------------------------------------------
void
StockItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_hover = true;
    update();
}

//-----------------------------------------------------------------------------
void
StockItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_hover = false;
    update();
}

#include "stock_item.moc"
