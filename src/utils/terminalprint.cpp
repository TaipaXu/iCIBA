#include "./terminalprint.hpp"
#include <QObject>
#include <sstream>
#include <format>
#include <vector>

const std::map<TerminalPrint::FgColor, std::string> TerminalPrint::fgColors{
    {FgColor::Black, "\033[30m"},
    {FgColor::Red, "\033[31m"},
    {FgColor::Green, "\033[32m"},
    {FgColor::Yellow, "\033[33m"},
    {FgColor::Blue, "\033[34m"},
    {FgColor::Magenta, "\033[35m"},
    {FgColor::Cyan, "\033[36m"},
    {FgColor::White, "\033[37m"},
};

const std::map<TerminalPrint::BgColor, std::string> TerminalPrint::bgColors{
    {BgColor::Normal, ""},
    {BgColor::Black, "\033[40m"},
    {BgColor::Red, "\033[41m"},
    {BgColor::Green, "\033[42m"},
    {BgColor::Yellow, "\033[43m"},
    {BgColor::Blue, "\033[44m"},
    {BgColor::Magenta, "\033[45m"},
    {BgColor::Cyan, "\033[46m"},
    {BgColor::White, "\033[47m"},
};

const std::string TerminalPrint::COLOR_CLOSE{"\033[0m"};
const std::string TerminalPrint::BREAK_LINE{"\n"};
const std::string TerminalPrint::INDENT{"  "};

TerminalPrint::TerminalPrint(const Model::WordResult &wordResult)
    : BasePrint(wordResult)
{
}

TerminalPrint::TerminalPrint(const Model::SentenceResult &sentenceResult)
    : BasePrint(sentenceResult)
{
}

std::string TerminalPrint::getNetworkErrorStr()
{
    return getErrorStr(BasePrint::getNetworkErrorStr());
}

std::string TerminalPrint::getNetworkParseErrorStr()
{
    return getErrorStr(BasePrint::getNetworkParseErrorStr());
}

std::string TerminalPrint::getWordStr()
{
    std::string result = std::format("{}{}", color(wordResult.word, FgColor::Green), BREAK_LINE);
    const std::string pronunciation = getPronunciation();
    if (!pronunciation.empty())
    {
        result += std::format("{}{}", pronunciation, BREAK_LINE);
    }
    const std::string meanings = getMeanings();
    if (!meanings.empty())
    {
        result += std::format("{}{}", meanings, BREAK_LINE);
    }
    return result;
}

std::string TerminalPrint::getSentenceStr()
{
    const std::string result = std::format("{}{}{}{}",
                                           sentenceResult.sentence.toStdString(),
                                           BREAK_LINE,
                                           color(sentenceResult.translation, TerminalPrint::FgColor::Green),
                                           BREAK_LINE);
    return result;
}

std::string TerminalPrint::getPronunciation()
{
    std::string result = "";
    if (!wordResult.pronunciation.en.isEmpty())
    {
        result += std::format("{}[{}]",
                              QObject::tr("en").toStdString(),
                              color(wordResult.pronunciation.en, FgColor::Green));
    }
    if (!wordResult.pronunciation.en.isEmpty() && !wordResult.pronunciation.am.isEmpty())
    {
        result += INDENT;
    }
    if (!wordResult.pronunciation.am.isEmpty())
    {
        result += std::format("{}[{}]",
                              QObject::tr("us").toStdString(),
                              color(wordResult.pronunciation.am, FgColor::Green));
    }
    return result;
}

std::string TerminalPrint::getMeanings()
{
    std::vector<std::string> list;
    std::transform(wordResult.meanings.begin(), wordResult.meanings.end(), std::back_inserter(list),
                   [this](const Model::WordResult::Meaning &meaning) {
                       if (meaning.part.isEmpty())
                       {
                           return std::format("{}", meaning.means.join("; ").toStdString());
                       }
                       else [[likely]]
                       {
                           return std::format("{} {}", color(meaning.part, FgColor::Green), meaning.means.join("; ").toStdString());
                       }
                   });

    std::string meanings = std::accumulate(std::next(list.begin()), list.end(), list.begin() != list.end() ? list.front() : "",
                                           [this](const std::string &a, const std::string &b) {
                                               return a + BREAK_LINE + b;
                                           });

    return meanings;
}

std::string TerminalPrint::color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor)
{
    return std::format("{}{}{}{}", fgColors.at(fgColor), bgColors.at(bgColor), str, COLOR_CLOSE);
}

std::string TerminalPrint::color(const QString &str, const FgColor &fgColor, const BgColor &bgColor)
{
    return TerminalPrint::color(str.toStdString(), fgColor, bgColor);
}

std::string TerminalPrint::getErrorStr(const std::string &str)
{
    return std::format("{}{}", color(str, TerminalPrint::FgColor::Red), BREAK_LINE);
}

std::string TerminalPrint::getErrorStr(const QString &str)
{
    return getErrorStr(str.toStdString());
}
