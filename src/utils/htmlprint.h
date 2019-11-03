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
#include <map>

class HtmlPrint final : public BasePrint
{
public:
    enum class Color
    {
        Normal,
        Black,
        Red,
        Green,
        Blue,
        White
    };

    explicit HtmlPrint(const Model::WordResult &wordResult);
    explicit HtmlPrint(const Model::SentenceResult &sentenceResult);
    ~HtmlPrint() = default;

    static std::string getNetworkErrorStr();
    static std::string getNetworkParseErrorStr();

private:
    static QString color(const Color &color);
    static QString getFgColorAttribute(const Color &color);
    static QString getBgColorAttribute(const Color &color);
    std::string getWordStr() override;
    std::string getSentenceStr() override;
    std::string getPronunciation() override;
    std::string getMeanings() override;
    std::string getExchange() override;
    static std::string getErrorStr(const std::string &str);
    static std::string getErrorStr(const QString &str);

    static QString getElement(const QString &element, const QString &content, const std::map<const QString, const QString> &attributes = {});

    static const std::map<Color, std::string> colors;
    static const std::string INDENT;
};
