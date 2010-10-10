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
** stock_data.h
**
**        Created on: Sep 29, 2010
**            Author: fargie_s
**
*/

#ifndef STOCK_DATA_H_
#define STOCK_DATA_H_

#include <QList>
#include <QVariantList>

class StockData {
public:
  StockData(const QString &symbol, const unsigned int count = 0, const double price = 0.0);

  /**
   * @return stock gain/loss in percent
   */
  double getVariation() const;

  QString     m_symbol;
  QString           m_name;
  unsigned int      m_count;
  double            m_price;
  double            m_price_paid;
};

typedef QList< StockData > StockDataList;

/**
 * convert a QVariantList in a StockData list
 */
StockDataList &operator << (StockDataList &, const QVariantList &);

/**
 * convert a StockData list in a QVariantList
 */
const StockDataList &operator >> (const StockDataList &, QVariantList &);

#endif /* STOCK_DATA_H_ */
