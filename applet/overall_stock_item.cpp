/*
** Copyright (C) 2010 Fargier Sylvain <fargier.sylvain@free.fr>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** overall_stock_item.cpp
**
**        Created on: Sep 29, 2010
**            Author: fargie_s
**
*/

#include "overall_stock_item.h"

#include <QMutexLocker>
#include <Plasma/Theme>

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
OverallStockItem::OverallStockItem(bool even, QGraphicsItem* parent)
    :
    QGraphicsWidget(parent),
    m_even(even),
    m_hover(false),
    m_total_paid(0.0)
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
OverallStockItem::paint(QPainter* painter,
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

    frame.adjust(10, 1, -10, -1);

    // paint the text
    QString string;
    QTextStream stream(&string);

    stream.setRealNumberPrecision(2);
    stream.setRealNumberNotation(QTextStream::FixedNotation);
    QFont font(painter->font());

    // symbol
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(frame, Qt::AlignLeft  | Qt::AlignBottom, "Overall");

    m_lock.lock();
    // change
    double total = 0;
    for (QHash< QString, StockData >::const_iterator it = m_stocks.begin();
         it != m_stocks.end(); ++it) {
      total += it->m_price;
    }
    m_lock.unlock();

    stream << total;
    painter->drawText(frame, Qt::AlignRight | Qt::AlignTop, string);

    // change
    stream.setNumberFlags(QTextStream::ForceSign);
    string.clear();
    double var = total - m_total_paid;
    stream << var;
    painter->setPen(var < 0 ? Qt::red : Qt::green);
    painter->drawText(frame, Qt::AlignRight | Qt::AlignBottom, string);
}

void OverallStockItem::addStock(const QString &name, const unsigned int count, const double price) {
  QMutexLocker locker(&m_lock);
  QHash< QString, StockData >::iterator it = m_stocks.find(name);

  if (it != m_stocks.end())
    m_total_paid -= it->m_price_paid;
  m_total_paid += price * count;

  m_stocks.insert(name, StockData(name, count, price * count));
}

//-----------------------------------------------------------------------------
// public slots:
//-----------------------------------------------------------------------------
void
OverallStockItem::dataUpdated(const QString&                  source,
                       const Plasma::DataEngine::Data& data)
{
    Q_UNUSED(source);
    QMutexLocker locker(&m_lock);
    QHash< QString, StockData >::iterator it = m_stocks.find(data["symbol"].toString());

    if (it == m_stocks.end())
      return;

    it->m_price = it->m_count * data["price"].toDouble();
    update();
}

//-----------------------------------------------------------------------------
void
OverallStockItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_hover = true;
    update();
}

//-----------------------------------------------------------------------------
void
OverallStockItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_hover = false;
    update();
}

#include "overall_stock_item.moc"
