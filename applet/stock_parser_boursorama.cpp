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
** stock_parser_bousorama.cpp
**
**        Created on: Oct 9, 2010
**            Author: fargie_s
**
*/

#include <QXmlQuery>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QLocale>
#include <tidy.h>
#include <buffio.h>
#include <iostream>
#include <stdint.h>

#include "stock_parser_boursorama.h"

bool tidyfy(const QString &str, QString &html_str) {
  TidyDoc tdoc = tidyCreate();
  TidyBuffer output;
  TidyBuffer errbuf;
  int rc = -1;

  tidyBufInit(&output);
  tidyBufInit(&errbuf);
  if (tidyOptSetBool(tdoc, TidyXhtmlOut, yes))
    rc = tidySetErrorBuffer(tdoc, &errbuf);
  if (rc >= 0)
    rc = tidySetInCharEncoding(tdoc, "utf8");
  if (rc >= 0)
    rc = tidySetOutCharEncoding(tdoc, "utf8");
  if ( rc >= 0 )
    rc = tidyParseString(tdoc, str.toUtf8().constData());
  if ( rc >= 0 )
    rc = tidyCleanAndRepair(tdoc);
  if ( rc >= 0 )
    rc = tidyRunDiagnostics(tdoc);
  if ( rc > 1 )
    rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
  if ( rc >= 0 )
    rc = tidySaveBuffer(tdoc, &output);

  if (rc >= 0) {
    if (rc > 0) {
      std::cerr << "An error occured while running tidy: " << (const char *) errbuf.bp << std::endl;
    }
    html_str = QString::fromUtf8((const char *) output.bp);
  }
  else {
    std::cerr << "A severe error occurred in tidy: " << rc << std::endl;
  }

  tidyBufFree(&output);
  tidyBufFree(&errbuf);
  tidyRelease(tdoc);
  return rc >= 0;
}

static uint8_t getHeaders(const QString &str,
                          int8_t &label_index,
                          int8_t &count_index,
                          int8_t &price_index) {
  QXmlQuery query;
  QStringList output;

  query.setFocus(str);
  query.setQuery("//*:th/*:a[starts-with(@href, 'http://www.boursorama.com/portefeuille/portefeuille.phtml')]/string()");

  if (!query.isValid() || !query.evaluateTo(&output)) {
    std::cerr << "Failed to run XPath query" << std::endl;
    return 0;
  }

  int8_t curr_index = 0;
  QString quantity = QString::fromUtf8("Qt\xc3\xa9");

  for (QStringList::iterator it = output.begin();
         it != output.end(); ++it, ++curr_index) {
      QString header = it->trimmed();

      if ((label_index < 0) && (header == "Libelle"))
        label_index = curr_index;
      else if ((count_index < 0) && (header == quantity))
        count_index = curr_index;
      else if ((price_index < 0) && (header == "Prix revient"))
        price_index = curr_index;
  }

  return output.size();
}

static bool getStockSymbols(const QString &str, const int8_t label_index, QStringList &symbols) {
  QXmlQuery query;
  static const QRegExp regExpSym("symbole=1rP([^&]*)");

  query.setFocus(str);

  query.setQuery("//*:tr/*:td[" + QString::number((int) (label_index + 2)) + "]/*/*:a/@href/string()");
  if (!query.isValid() || !query.evaluateTo(&symbols)) {
    std::cerr << "Failed to run XPath query " << query.isValid() << std::endl;
    return false;
  }

  for (QStringList::iterator it = symbols.begin();
       it != symbols.end(); ++it) {
    if (regExpSym.indexIn(*it) != -1) {
      *it = regExpSym.cap(1);
    }
  }
  return true;
}

static bool getStockData(const QString &str, const int8_t index, QStringList &data) {
  QXmlQuery query;

  query.setFocus(str);

  query.setQuery("//*:tr/*:td[" + QString::number((int) (index)) + "]/string()");
  if (!query.isValid() || !query.evaluateTo(&data)) {
    std::cerr << "Failed to run XPath query " << query.isValid() << std::endl;
    return false;
  }

  return true;
}

bool StockParserBoursorama::parse(const QString &str, StockDataList &list) {
  QString html_str;
  int8_t label_index = -1, count_index = -1, price_index = -1;
  uint8_t nb_col;
  QStringList stockSymbols, stockCount, stockPrices;

  if (!tidyfy(str, html_str)) {
    std::cerr << "Something went wrong with tidy" << std::endl;
    return false;
  }

  nb_col = getHeaders(html_str, label_index, count_index, price_index);

  if ((label_index < 0) || (count_index < 0) || (price_index < 0) || (nb_col == 0)) {
    std::cerr << "A column is missing label: " << (int) label_index
        << " count: " << (int) count_index << " price: "
        << (int) price_index << std::endl;
    return false;
  }

  if (!getStockSymbols(html_str, label_index, stockSymbols)) {
    std::cerr << "Failed to get symbols" << std::endl;
    return false;
  }

  if (!getStockData(html_str, count_index + 2, stockCount)) {
    std::cerr << "Failed to get stock count" << std::endl;
    return false;
  }
  QStringList::iterator countIt;
  for (countIt = stockCount.begin(); countIt != stockCount.end(); ++countIt) {
    int pos = countIt->indexOf(',');
    if (pos > 0)
      countIt->resize(pos);
  }

  if (!getStockData(html_str, price_index + 2, stockPrices)) {
    std::cerr << "Failed to get stock prices" << std::endl;
    return false;
  }

  QStringList::const_iterator symIt, priceIt;
  QLocale loc(QLocale::French);
  for (symIt = stockSymbols.begin(), countIt = stockCount.begin(),
       priceIt = stockPrices.begin();
       (symIt != stockSymbols.end()) && (countIt != stockCount.end()) &&
       (priceIt != stockPrices.end()); symIt++, countIt++, priceIt++) {
    list.push_back(StockData(*symIt + ".PA", loc.toUInt(*countIt), loc.toDouble(*priceIt)));
  }

  return true;
}

StockParser *StockParserBoursorama::Resolver::createParser(const QString &data) {
  if (data.contains("boursorama"))
    return new StockParserBoursorama();
  else
    return NULL;
}
