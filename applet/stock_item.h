#pragma once

#include <QGraphicsWidget>

#include <Plasma/DataEngine>

#include "stock_data.h"

class StockItem : public QGraphicsWidget
{
  Q_OBJECT

  public:
    virtual ~StockItem() {};
    StockItem(bool even, unsigned int count, double price_paid, QGraphicsItem* parent = 0);

    virtual void paint(QPainter* painter, 
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);

  public Q_SLOTS:
    virtual void dataUpdated(const QString& source, const Plasma::DataEngine::Data& data);

  protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  private:
    const bool          m_even;
    bool                m_hover;
    StockData           m_data;
    double              m_change_percent;
    double              m_change;
    double              m_value;
};

