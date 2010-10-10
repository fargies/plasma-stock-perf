#pragma once

#include <QGraphicsWidget>

#include <Plasma/DataEngine>

class HeaderItem : public QGraphicsWidget
{
  public:
    HeaderItem(QGraphicsItem* parent = 0);

    virtual void paint(QPainter* painter, 
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);
};

