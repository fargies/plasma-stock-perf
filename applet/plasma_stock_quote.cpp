//==============================================================================
//
// Copyright (c) 2008 Ryan Winter. All rights reserved.
//
// This file may be used under the terms of the GNU General Public
// License version 2.0 appearing in the file LICENSE packaged with this file.
//
//==============================================================================

#include "plasma_stock_quote.h"

#include "header_item.h"
#include "stock_item.h"
#include "overall_stock_item.h"
#include "footer_item.h"
#include "config.h"

#include <QtGui>

#include <KConfigDialog>
#include <KStandardDirs>
#include <Plasma/Theme>

//-----------------------------------------------------------------------------
// public:
//-----------------------------------------------------------------------------
PlasmaStockQuote::PlasmaStockQuote(QObject *parent, const QVariantList &args)
    :
    Plasma::PopupApplet(parent, args),
    m_graphics_widget(0),
    m_layout(0),
    m_engine(0),
    m_footer(0),
    m_header(0),
    m_widgets(),
    m_overall(NULL),
    m_stock_list(),
    m_opacity(1.0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setHasConfigurationInterface(true);
    setPopupIcon("kchart");
}

//-----------------------------------------------------------------------------
PlasmaStockQuote::~PlasmaStockQuote()
{
}

//-----------------------------------------------------------------------------
QGraphicsWidget *
PlasmaStockQuote::graphicsWidget()
{
    if (m_graphics_widget) return m_graphics_widget;


    m_graphics_widget = new QGraphicsWidget(this);
    m_graphics_widget->setSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::Preferred);

    // read in the stock list
    KConfigGroup configGroup = config(PROJECT_NAME);
    m_stock_list << configGroup.readEntry("stocks",  QVariantList());
    m_opacity    = configGroup.readEntry("opacity", 1.0);

    m_engine = dataEngine("stockquote");
    if (!m_engine->isValid())
    {
        setFailedToLaunch(true, "Unable to load data engine");
        return m_graphics_widget;
    }

    // setup the ui
    m_layout = new QGraphicsLinearLayout(Qt::Vertical, m_graphics_widget);
    m_layout->setSpacing(0);

    // add the header
    m_header = new HeaderItem(this);
    m_layout->addItem(m_header);

    // add the footer
    m_footer = new FooterItem(this);
    m_engine->connectSource("status", m_footer);
    m_layout->addItem(m_footer);

    // update the stocks
    m_graphics_widget->setOpacity(m_opacity);
    updateSources();

    return m_graphics_widget;
}
#include <iostream>
//-----------------------------------------------------------------------------
// protected slots:
//-----------------------------------------------------------------------------
void
PlasmaStockQuote::configAccepted()
{
    m_opacity    = m_ui.opacity->value() / 100.0;
    m_graphics_widget->setOpacity(m_opacity);

    QVariantList vlist = m_ui.stockConfig->data();
    m_stock_list.clear();
    m_stock_list << vlist;

    updateSources();

    KConfigGroup configGroup = config();
    configGroup.writeEntry("stocks",  vlist);
    configGroup.writeEntry("opacity", m_opacity);

    emit configNeedsSaving();
}

//-----------------------------------------------------------------------------
// protected:
//-----------------------------------------------------------------------------
void
PlasmaStockQuote::createConfigurationInterface(KConfigDialog* parent)
{
    QWidget* widget = new QWidget;
    QVariantList vlist;
    m_stock_list >> vlist;

    m_ui.setupUi(widget);

    m_ui.stockConfig->setData(vlist);
    m_ui.opacity->setValue(m_opacity * 100);

    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(widget, parent->windowTitle(), icon());

    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

//-----------------------------------------------------------------------------
void
PlasmaStockQuote::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::FormFactorConstraint)
    {
        setBackgroundHints(Plasma::Applet::NoBackground);
    }
}

//-----------------------------------------------------------------------------
// private:
//-----------------------------------------------------------------------------
void
PlasmaStockQuote::updateSources()
{
    prepareGeometryChange();

    // out with the old
    for (WidgetMap::iterator iter = m_widgets.begin();
         iter != m_widgets.end(); ++iter)
    {
        m_engine->disconnectSource(iter.key(), iter.value());
        m_engine->disconnectSource(iter.key(), m_overall);

        m_layout->removeItem(iter.value());

        delete iter.value();
        m_widgets.erase(iter);
    }

    if (m_overall != NULL) {
      m_layout->removeItem(m_overall);
      delete m_overall;
    }

    bool even = false;
    m_overall = new OverallStockItem((m_stock_list.size() % 2) ? !even : even, this);
    // in with the new
    for (StockDataList::const_iterator it = m_stock_list.begin();
         it != m_stock_list.end(); ++it, even = !even) {
      QString sym = it->m_symbol.toUpper();

      if (sym.isEmpty() || m_widgets.count(sym) != 0)
        continue;

      StockItem *item = new StockItem(even, it->m_count, it->m_price_paid, this);
      m_overall->addStock(sym, it->m_count, it->m_price_paid);

      m_engine->connectSource(sym, item);
      m_engine->connectSource(sym, m_overall);

      m_widgets.insert(sym, item);

      m_layout->insertItem(m_layout->count() - 1, item);
    }
    m_layout->insertItem(m_layout->count() - 1, m_overall);

    m_graphics_widget->setMinimumSize(m_layout->sizeHint(Qt::MinimumSize));
    m_graphics_widget->setPreferredSize(m_layout->sizeHint(Qt::PreferredSize));

    // if it's on the desktop, resize it
    if (layout()->itemAt(0) == m_graphics_widget)
    {
        setMinimumSize(m_layout->sizeHint(Qt::MinimumSize));
        setPreferredSize(m_layout->sizeHint(Qt::PreferredSize));
        resize(preferredSize());
    }
}

#include "plasma_stock_quote.moc"
