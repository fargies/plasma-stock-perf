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
** stock_parser_bousorama.h
**
**        Created on: Oct 9, 2010
**            Author: fargie_s
**
*/

#ifndef STOCK_PARSER_BOUSORAMA_H_
#define STOCK_PARSER_BOUSORAMA_H_

#include <QString>
#include <stdint.h>
#include "stock_parser.h"

class StockParserBoursorama : public StockParser
{
public:
  virtual bool parse(const QString &str, StockDataList &list);

  class Resolver : public StockParser::Resolver
  {
  public:
    virtual StockParser *createParser(const QString &data);
  };

};

#endif /* STOCK_PARSER_BOUSORAMA_H_ */