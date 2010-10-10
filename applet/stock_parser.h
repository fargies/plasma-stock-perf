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
** stock_parser.h
**
**        Created on: Oct 8, 2010
**            Author: fargie_s
**
*/

#ifndef STOCK_PARSER_H_
#define STOCK_PARSER_H_

#include <QString>
#include "stock_data.h"

class StockParser {
public:
  virtual ~StockParser() {};
  virtual bool parse(const QString &str, StockDataList &list) = 0;

  /**
   * @brief create a StockParser instance if data might be parsed by this stockParser
   */
  class Resolver
  {
  public:
    virtual ~Resolver() {};
    virtual StockParser *createParser(const QString &data) = 0;
  };
};

class StockParserFactory {
public:
  inline static StockParserFactory &getInstance() { return m_instance; }

  StockParser *find(const QString &str);
  void addResolver(StockParser::Resolver *resolver);

  ~StockParserFactory();
protected:
  StockParserFactory();

  static StockParserFactory m_instance;
  QList< StockParser::Resolver * > m_resolvers;
};

#endif /* STOCK_PARSER_H_ */
