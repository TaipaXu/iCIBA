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

#include <string>
#include "../models/wordresult.h"
#include "../models/sentenceresult.h"

class BasePrint
{
public:
    enum class Type
    {
        Word,
        Sentence
    };

public:
    explicit BasePrint(const Model::WordResult &wordResult);
    explicit BasePrint(const Model::SentenceResult &sentenceResult);
    ~BasePrint() = default;

    std::string getStr();

protected:
    static std::string getNetworkErrorStr();
    static std::string getNetworkParseErrorStr();

    Type type;
    Model::WordResult wordResult;
    Model::SentenceResult sentenceResult;

private:
    virtual std::string getWordStr() = 0;
    virtual std::string getSentenceStr() = 0;
    virtual std::string getPronunciation() = 0;
    virtual std::string getMeanings() = 0;
    virtual std::string getExchange() = 0;
};
