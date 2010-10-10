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
** stock_config.h
**
**        Created on: Sep 26, 2010
**            Author: fargie_s
**
*/

#ifndef STOCK_CONFIG_H_
#define STOCK_CONFIG_H_

#include <stock_data.h>
#include <QTableWidget>
#include <QVariant>
#include <QGroupBox>
#include <QPushButton>

class QClipboard;

class StockTable : public QTableWidget
{
  Q_OBJECT
public:
  StockTable(QWidget *parent = 0);

  QVariantList data() const;
  void setData(const QVariantList &list);
  void setData(const StockDataList &list);
};

class StockConfig : public QGroupBox {
  Q_OBJECT
public:
  StockConfig(QWidget *parent = 0);

  QVariantList data() const;
  void setData(const QVariantList &list);
  void setData(const StockDataList &list);

protected:
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent * event);

protected slots:
  void addStock();
  void removeStock();
  void moveUp();
  void moveDown();
  void stockActivated(const QModelIndex&, const QModelIndex&);
  void updateClipboard();

protected:
  StockTable *m_stocks;
  QPushButton *m_add, *m_remove, *m_up, *m_down;
  QClipboard *m_clip;
};

#endif /* STOCK_CONFIG_H_ */
