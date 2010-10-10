#pragma once

#include "ui_plasma_stock_quote_config.h"

#include <Plasma/PopupApplet>
#include <Plasma/DataEngine>

#include "stock_data.h"

class FooterItem;
class HeaderItem;
class OverallStockItem;

class QGraphicsWidget;
class QGraphicsLinearLayout;

class PlasmaStockQuote : public Plasma::PopupApplet
{
    Q_OBJECT

  public:
    PlasmaStockQuote(QObject *parent, const QVariantList &args);
    virtual ~PlasmaStockQuote();

    virtual QGraphicsWidget *graphicsWidget();

  protected slots:
    void configAccepted();

  protected:
    virtual void createConfigurationInterface(KConfigDialog *parent);
    virtual void constraintsEvent(Plasma::Constraints constraints);

  private:
    void updateSources();

    typedef QMap<QString, QGraphicsWidget*> WidgetMap;

    Ui::PlasmaStockQuoteConfig m_ui;

    QGraphicsWidget*           m_graphics_widget;
    QGraphicsLinearLayout*     m_layout;

    Plasma::DataEngine*        m_engine;

    FooterItem*                m_footer;
    HeaderItem*                m_header;
    WidgetMap                  m_widgets;
    OverallStockItem*          m_overall;

    StockDataList              m_stock_list;
    double                     m_opacity;
};

K_EXPORT_PLASMA_APPLET(stockquote, PlasmaStockQuote)

