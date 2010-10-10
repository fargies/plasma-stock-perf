#pragma once

#include <QGraphicsWidget>

#include <Plasma/DataEngine>

class StockItem : public QGraphicsWidget
{
  Q_OBJECT

  public:
    StockItem(bool even, QGraphicsItem* parent = 0);

    virtual void paint(QPainter* painter, 
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);

  public slots:
    void dataUpdated(const QString& source, const Plasma::DataEngine::Data& data);

  protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  private:
    const bool m_even;
    bool       m_hover;
    QString    m_symbol;
    QString    m_name;
    double     m_price;
    double     m_change;
    double     m_percent_change;
};

