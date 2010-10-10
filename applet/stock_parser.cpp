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
** stock_parser.cpp
**
**        Created on: Oct 8, 2010
**            Author: fargie_s
**
*/

#include "stock_parser.h"
#include "stock_parser_boursorama.h"

StockParserFactory StockParserFactory::m_instance;

StockParser *StockParserFactory::find(const QString &str) {
  StockParser *ret = NULL;

  for (QList< StockParser::Resolver * >::const_iterator it = m_resolvers.begin();
       it != m_resolvers.end() && ret == NULL; ++it) {
    ret = (*it)->createParser(str);
  }
  return ret;
}

void StockParserFactory::addResolver(StockParser::Resolver *resolver) {
  m_resolvers.append(resolver);
}

StockParserFactory::StockParserFactory() {
  addResolver(new StockParserBoursorama::Resolver());
}

StockParserFactory::~StockParserFactory() {
  foreach(StockParser::Resolver *res, m_resolvers) {
    delete res;
  }
}

