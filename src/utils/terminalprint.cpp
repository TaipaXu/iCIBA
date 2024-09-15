#include "./terminalprint.hpp"
#include <ranges>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sstream>
#ifdef ENABLE_FORMAT_SUPPORT
#include <format>
#else
#include <sstream>
#endif
#include <boost/algorithm/string/join.hpp>

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

#ifdef ENABLE_FORMAT_SUPPORT
std::string TerminalPrint::getWordStr() const
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
#else
std::string TerminalPrint::getWordStr() const
{
    std::ostringstream result;
    result << color(wordResult.word, FgColor::Green) << BREAK_LINE;
    const std::string pronunciation = getPronunciation();
    if (!pronunciation.empty())
    {
        result << pronunciation << BREAK_LINE;
    }
    const std::string meanings = getMeanings();
    if (!meanings.empty())
    {
        result << meanings << BREAK_LINE;
    }
    return result.str();
}
#endif

#ifdef ENABLE_FORMAT_SUPPORT
std::string TerminalPrint::getSentenceStr() const
{
    const std::string result = std::format("{}{}{}{}",
                                           sentenceResult.sentence,
                                           BREAK_LINE,
                                           color(sentenceResult.translation, TerminalPrint::FgColor::Green),
                                           BREAK_LINE);
    return result;
}
#else
std::string TerminalPrint::getSentenceStr() const
{
    std::ostringstream result;
    result << sentenceResult.sentence
           << BREAK_LINE
           << color(sentenceResult.translation, TerminalPrint::FgColor::Green)
           << BREAK_LINE;
    return result.str();
}
#endif

#ifdef ENABLE_FORMAT_SUPPORT
std::string TerminalPrint::getPronunciation() const
{
    std::string result = "";
    if (!wordResult.pronunciation.en.empty())
    {
        result += std::format("en[{}]", color(wordResult.pronunciation.en, FgColor::Green));
    }
    if (!wordResult.pronunciation.en.empty() && !wordResult.pronunciation.am.empty())
    {
        result += INDENT;
    }
    if (!wordResult.pronunciation.am.empty())
    {
        result += std::format("{us}[{}]", color(wordResult.pronunciation.am, FgColor::Green));
    }
    return result;
}
#else
std::string TerminalPrint::getPronunciation() const
{
    std::ostringstream result;
    if (!wordResult.pronunciation.en.empty())
    {
        result << "en[" << color(wordResult.pronunciation.en, FgColor::Green) << "]";
    }
    if (!wordResult.pronunciation.en.empty() && !wordResult.pronunciation.am.empty())
    {
        result << INDENT;
    }
    if (!wordResult.pronunciation.am.empty())
    {
        result << "us[" << color(wordResult.pronunciation.am, FgColor::Green) << "]";
    }
    return result.str();
}
#endif

#ifdef ENABLE_FORMAT_SUPPORT
std::string TerminalPrint::getMeanings() const
{
    std::vector<std::string> list;
    std::transform(wordResult.meanings.begin(), wordResult.meanings.end(), std::back_inserter(list),
                   [this](const Model::WordResult::Meaning &meaning)
                   {
                       std::string meansJoined = boost::algorithm::join(meaning.means, "; ");
                       if (meaning.part.empty())
                       {
                           return meansJoined;
                       }
                       else [[likely]]
                       {
                           return std::format("{} {}", color(meaning.part, FgColor::Green), meansJoined);
                       }
                   });
    return boost::algorithm::join(list, BREAK_LINE);
}
#else
std::string TerminalPrint::getMeanings() const
{
    std::ostringstream result;
    std::vector<std::string> list;
    std::transform(wordResult.meanings.begin(), wordResult.meanings.end(), std::back_inserter(list),
                   [this](const Model::WordResult::Meaning &meaning)
                   {
                       std::string meansJoined = boost::algorithm::join(meaning.means, "; ");
                       if (meaning.part.empty())
                       {
                           return meansJoined;
                       }
                       else [[likely]]
                       {
                           return color(meaning.part, FgColor::Green) + " " + meansJoined;
                       }
                   });
    return boost::algorithm::join(list, BREAK_LINE);
}
#endif

#ifdef ENABLE_FORMAT_SUPPORT
std::string TerminalPrint::color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor)
{
    return std::format("{}{}{}{}", fgColors.at(fgColor), bgColors.at(bgColor), str, COLOR_CLOSE);
}
#else
std::string TerminalPrint::color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor)
{
    std::ostringstream result;
    result << fgColors.at(fgColor) << bgColors.at(bgColor) << str << COLOR_CLOSE;
    return result.str();
}
#endif

#ifdef ENABLE_FORMAT_SUPPORT
std::string TerminalPrint::getErrorStr(const std::string &str)
{
    return std::format("{}{}", color(str, TerminalPrint::FgColor::Red), BREAK_LINE);
}
#else
std::string TerminalPrint::getErrorStr(const std::string &str)
{
    std::ostringstream result;
    result << color(str, TerminalPrint::FgColor::Red) << BREAK_LINE;
    return result.str();
}
#endif
