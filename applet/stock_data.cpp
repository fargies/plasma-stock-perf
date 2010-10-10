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
** stock_data.cpp
**
**        Created on: Sep 29, 2010
**            Author: fargie_s
**
*/

#include "stock_data.h"

StockData::StockData(const QString &symbol, const unsigned int count, const double price) :
  m_symbol(symbol), m_count(count), m_price(price), m_price_paid(price)
{
}

double StockData::getVariation() const {
  if (m_price_paid == 0)
    return 0;
  else
    return 100 * (m_price - m_price_paid) / m_price_paid;
}
#include <iostream>
QList< StockData > &operator << (QList< StockData > &stock, const QVariantList &variant) {
  QString sym;
  unsigned int count;
  double price;

  for (QVariantList::const_iterator it = variant.begin(); it != variant.end(); ++it) {
    if (!it->canConvert(QVariant::String))
      break;
    sym = it->toString();

    if (++it == variant.end() || !it->canConvert(QVariant::UInt))
      break;

    count = it->toUInt();

    if (++it == variant.end() || !it->canConvert(QVariant::Double))
      break;

    price = it->toDouble();

    stock.push_back(StockData(sym, count, price));
  }
  return stock;
}

const StockDataList &operator >> (const StockDataList &stock, QVariantList &variant) {
  for (StockDataList::const_iterator it = stock.begin(); it != stock.end(); ++it) {
    variant << QVariant(it->m_symbol) << QVariant(it->m_count) << QVariant(it->m_price_paid);
  }
  return stock;
}
