#pragma once

#include <QGraphicsWidget>

#include <Plasma/DataEngine>

class FooterItem : public QGraphicsWidget
{
  Q_OBJECT

  public:
    FooterItem(QGraphicsItem* parent = 0);

    virtual void paint(QPainter* painter, 
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);

  public slots:
    void dataUpdated(const QString& source, const Plasma::DataEngine::Data& data);

  private:
    QString m_time;
};

