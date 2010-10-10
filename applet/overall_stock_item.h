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
** overall_stock_item.h
**
**        Created on: Sep 29, 2010
**            Author: fargie_s
**
*/

#ifndef OVERALL_STOCK_ITEM_H_
#define OVERALL_STOCK_ITEM_H_

#pragma once

#include <QGraphicsWidget>

#include <Plasma/DataEngine>
#include <QMutex>

#include "stock_data.h"

class OverallStockItem : public QGraphicsWidget
{
  Q_OBJECT

  public:
    OverallStockItem(bool even, QGraphicsItem* parent = 0);

    virtual void paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);

    void addStock(const QString &name, const unsigned int count, const double price);

  public slots:
    void dataUpdated(const QString& source, const Plasma::DataEngine::Data& data);

  protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  private:
    const bool          m_even;
    bool                m_hover;

    double m_total_paid;
    QHash< QString, StockData > m_stocks;
    QMutex m_lock;
};



#endif /* OVERALL_STOCK_ITEM_H_ */
