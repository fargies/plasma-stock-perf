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
** stock_config.cpp
**
**        Created on: Sep 26, 2010
**            Author: fargie_s
**
*/

#include <QStringList>
#include <QString>
#include <QObject>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDragEnterEvent>
#include <QClipboard>
#include <QApplication>

#include <iostream>
#include <stdint.h>

#include "stock_config.h"
#include "stock_parser.h"

class StockTableDelegate : public QStyledItemDelegate
{
public:
  virtual QWidget* createEditor(QWidget *parent,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
    switch (index.column()) {
      case 0:
      default:
        return QStyledItemDelegate::createEditor(parent, option, index);
      case 1: {
        QSpinBox *spin = new QSpinBox(parent);
        spin->setValue(index.data().toInt());
        spin->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        spin->setRange(1, 10000);
        return spin;
      }
      case 2: {
        QDoubleSpinBox *spin = new QDoubleSpinBox(parent);
        spin->setValue(index.data().toInt());
        spin->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        spin->setRange(0, 10000);
        return spin;
      }
    }
  }

  virtual void paint(QPainter *painter,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) const {
    switch (index.column()) {
      case 1:
      case 2:
      {
        QStyleOptionViewItem new_option = option;
        new_option.displayAlignment = Qt::AlignVCenter | Qt::AlignRight;
        QStyledItemDelegate::paint(painter, new_option, index);
        break;
      }
      default:
        QStyledItemDelegate::paint(painter, option, index);
        break;
    }
  }
};

StockTable::StockTable(QWidget *parent) :
	QTableWidget(0, 3, parent)
{
	QStringList labels;
	labels << tr("Stock Code") << tr("Count") << tr("Initial Value");
	setHorizontalHeaderLabels(labels);
	horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
	horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
	horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
	verticalHeader()->hide();
	setShowGrid(false);
	setItemDelegate(new StockTableDelegate());
}

QVariantList StockTable::data() const {
  QVariantList list;

  for (int i = 0; i < rowCount(); ++i) {
    QTableWidgetItem *it = item(i, 0);

    if (!it)
      continue;

    list.append(it->text());
    list.append(item(i, 1)->data(Qt::DisplayRole).toUInt());
    list.append(item(i, 2)->data(Qt::DisplayRole).toDouble());
  }
  return list;
}

void StockTable::setData(const QVariantList &list) {
  int row = 0;

  clearContents();

  for (QVariantList::const_iterator it = list.begin();
       it != list.end(); ++it, ++row) {

    insertRow(row);
    setItem(row, 0, new QTableWidgetItem(it->toString()));
    setItem(row, 1, new QTableWidgetItem((++it)->toString()));
    setItem(row, 2, new QTableWidgetItem((++it)->toString()));
  }
}

void StockTable::setData(const StockDataList &list) {
  int row = 0;

  clearContents();

  for (StockDataList::const_iterator it = list.begin();
       it != list.end(); ++it, ++row) {
    insertRow(row);
    setItem(row, 0, new QTableWidgetItem(it->m_symbol));
    setItem(row, 1, new QTableWidgetItem(QString::number(it->m_count)));
    setItem(row, 2, new QTableWidgetItem(QString::number(it->m_price_paid)));
  }
}

StockConfig::StockConfig(QWidget *parent) :
    QGroupBox("Stock Quotes", parent)
{
  setAcceptDrops(true);

  QHBoxLayout *mainLayout = new QHBoxLayout(this);

  m_stocks = new StockTable(this);
  mainLayout->addWidget(m_stocks);
  connect(m_stocks->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
      SLOT(stockActivated(const QModelIndex&, const QModelIndex&)));


  QVBoxLayout *btnsLayout = new QVBoxLayout();
  btnsLayout->addStretch();
  mainLayout->addLayout(btnsLayout);

  m_add = new QPushButton(QIcon::fromTheme("list-add"), "&Add", this);
  btnsLayout->addWidget(m_add);
  connect(m_add, SIGNAL(clicked()), SLOT(addStock()));

  m_remove = new QPushButton(QIcon::fromTheme("list-remove"), "&Remove", this);
  btnsLayout->addWidget(m_remove);
  connect(m_remove, SIGNAL(clicked()), SLOT(removeStock()));

  m_up = new QPushButton(QIcon::fromTheme("arrow-up"), "Move &Up", this);
  btnsLayout->addWidget(m_up);
  connect(m_up, SIGNAL(clicked()), SLOT(moveUp()));

  m_down = new QPushButton(QIcon::fromTheme("arrow-down"), "Move &Down", this);
  btnsLayout->addWidget(m_down);
  connect(m_down, SIGNAL(clicked()), SLOT(moveDown()));

  m_clip = QApplication::clipboard();
  connect(m_clip, SIGNAL(dataChanged()), this, SLOT(updateClipboard()));
}


QVariantList StockConfig::data() const {
  return m_stocks->data();
}

void StockConfig::setData(const QVariantList &list) {
  m_stocks->setData(list);
}

void StockConfig::setData(const StockDataList &list) {
  m_stocks->setData(list);
}

void StockConfig::addStock() {
  int row = m_stocks->currentRow();
  int col = m_stocks->currentColumn();

  if (col < 0)
    col = 0;

  m_stocks->insertRow(row + 1);
  m_stocks->setItem(row + 1, 1, new QTableWidgetItem("1"));
  m_stocks->setItem(row + 1, 2, new QTableWidgetItem("0"));
  m_stocks->setCurrentCell(row + 1, col);
}

void StockConfig::removeStock() {
  int row = m_stocks->currentRow();

  if (row >= 0) {
    m_stocks->removeRow(row);
    m_stocks->setCurrentCell(row, m_stocks->currentColumn());
  }
}

void StockConfig::moveUp() {
  int row = m_stocks->currentRow();

  if (row <= 0)
    return;

  for (uint8_t col = 0; col < 3; ++col) {
    QTableWidgetItem *item = m_stocks->takeItem(row - 1, col);
    m_stocks->setItem(row - 1, col, m_stocks->takeItem(row, col));
    m_stocks->setItem(row, col, item);
  }
  m_stocks->setCurrentCell(row - 1, m_stocks->currentColumn());
}

void StockConfig::moveDown() {
  int row = m_stocks->currentRow();

  if (row + 1 >= m_stocks->rowCount())
    return;

  for (uint8_t col = 0; col < 3; ++col) {
    QTableWidgetItem *item = m_stocks->takeItem(row + 1, col);
    m_stocks->setItem(row + 1, col, m_stocks->takeItem(row, col));
    m_stocks->setItem(row, col, item);
  }
  m_stocks->setCurrentCell(row + 1, m_stocks->currentColumn());
}

void StockConfig::stockActivated(const QModelIndex &current, const QModelIndex &) {
  if (current.row() + 1 >= m_stocks->rowCount())
    m_down->setEnabled(false);
  else
    m_down->setEnabled(true);

  if (current.row() == 0)
    m_up->setEnabled(false);
  else
    m_up->setEnabled(true);
}

void StockConfig::dragEnterEvent(QDragEnterEvent *event) {
  const QMimeData *mimeData = event->mimeData();

  if (mimeData->hasHtml())
    event->acceptProposedAction();
}

void StockConfig::dropEvent(QDropEvent * event) {
  const QMimeData *mimeData = event->mimeData();

  if (mimeData->hasHtml()) {
    QString str = mimeData->html();

    StockParser *parser = StockParserFactory::getInstance().find(str);
    if (parser) {
      StockDataList list;

      if (parser->parse(str, list))
        setData(list);
      delete parser;
    }
    else
      std::cout << "No parser found for dropped data" << std::endl;
  }
}

void StockConfig::updateClipboard() {
  const QMimeData *mimeData = m_clip->mimeData();

  if (mimeData->hasHtml()) {
    QString str = mimeData->html();

    StockParser *parser = StockParserFactory::getInstance().find(str);
    if (parser) {
      StockDataList list;

      if (parser->parse(str, list))
        setData(list);
      delete parser;
    }
    else
      std::cout << "No parser found for clipped data" << std::endl;
  }
}

#include "stock_config.moc"
