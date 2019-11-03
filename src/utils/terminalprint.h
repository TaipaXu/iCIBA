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

class TerminalPrint final : public BasePrint
{
public:
    enum class FgColor
    {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

    enum class BgColor
    {
        Normal,
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

    explicit TerminalPrint(const Model::WordResult &wordResult);
    explicit TerminalPrint(const Model::SentenceResult &sentenceResult);
    ~TerminalPrint() = default;

    static std::string getNetworkErrorStr();
    static std::string getNetworkParseErrorStr();

private:
    std::string getWordStr() override;
    std::string getSentenceStr() override;
    std::string getPronunciation() override;
    std::string getMeanings() override;
    std::string getExchange() override;
    static std::string color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor = BgColor::Normal);
    static std::string color(const QString &str, const FgColor &fgColor, const BgColor &bgColor = BgColor::Normal);
    static std::string getErrorStr(const std::string &str);
    static std::string getErrorStr(const QString &str);

    static const std::map<FgColor, std::string> fgColors;
    static const std::map<BgColor, std::string> bgColors;
    static const std::string COLOR_CLOSE;
    static const std::string BREAK_LINE;
    static const std::string INDENT;
};
