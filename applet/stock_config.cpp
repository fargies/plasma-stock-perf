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
#include <QObject>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <iostream>

#include "stock_config.h"

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
	QTableWidget(1, 3, parent)
{
	QStringList labels;
	labels << tr("Stock Code") << tr("Count") << tr("Initial Value");
	setHorizontalHeaderLabels(labels);
	horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
	verticalHeader()->hide();
	setShowGrid(false);
	setItemDelegate(new StockTableDelegate());
	setItem(0, 1, new QTableWidgetItem("1"));
	setItem(0, 2, new QTableWidgetItem("0"));

	connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(cellChangedSlot(int, int)));
}

void StockTable::cellChangedSlot(int row, int column) {
  if ((column == 0) && (row + 1 >= rowCount())) {
    QTableWidgetItem *i = item(row, 0);

    if (i && i->text() != "") {
      insertRow(row + 1);
      setItem(row + 1, 1, new QTableWidgetItem("1"));
      setItem(row + 1, 2, new QTableWidgetItem("0"));
      setCurrentCell(row + 1, 0);
    }
  }
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

void StockTable::setData(QVariantList &list) {

  int row = 0;
  for (QVariantList::const_iterator it = list.begin();
       it != list.end(); ++it, ++row) {

    setItem(row, 0, new QTableWidgetItem(it->toString()));
    setItem(row, 1, new QTableWidgetItem((++it)->toString()));
    setItem(row, 2, new QTableWidgetItem((++it)->toString()));
  }

}

#include "stock_config.moc"
