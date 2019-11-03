/*
 * GNU General Public License, Version 3.0
 *
 * Copyright (c) 2019 Taipa Xu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "baseprint.h"
#include <QObject>

BasePrint::BasePrint(const Model::WordResult &wordResult)
    : wordResult{wordResult},
      type{Type::Word}
{
}

BasePrint::BasePrint(const Model::SentenceResult &sentenceResult)
    : sentenceResult{sentenceResult},
      type{Type::Sentence}
{
}

std::string BasePrint::getStr()
{
    if (type == Type::Word)
    {
        return getWordStr();
    }
    else if (type == Type::Sentence)
    {
        return getSentenceStr();
    }
}

std::string BasePrint::getNetworkErrorStr()
{
    return QObject::tr("Network error").toStdString();
}

std::string BasePrint::getNetworkParseErrorStr()
{
    return QObject::tr("Network parsing error").toStdString();
}
