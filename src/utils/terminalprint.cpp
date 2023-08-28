#include "./terminalprint.hpp"
#include <QObject>
#include <sstream>

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
    std::ostringstream stringStream;
    stringStream << color(wordResult.word, FgColor::Green) << BREAK_LINE;
    const std::string pronunciation = getPronunciation();
    if (pronunciation.length() > 0)
    {
        stringStream << pronunciation << BREAK_LINE;
    }
    const std::string meanings = getMeanings();
    if (meanings.length() > 0)
    {
        stringStream << meanings << BREAK_LINE;
    }
    return stringStream.str();
}

std::string TerminalPrint::getSentenceStr()
{
    std::ostringstream stringStream;
    stringStream << sentenceResult.sentence.toStdString()
                 << BREAK_LINE
                 << color(sentenceResult.translation, TerminalPrint::FgColor::Green)
                 << BREAK_LINE;
    return stringStream.str();
}

std::string TerminalPrint::getPronunciation()
{
    std::ostringstream stringStream;
    if (!wordResult.pronunciation.en.isEmpty())
    {
        stringStream << QObject::tr("en").toStdString() + "[" + color(wordResult.pronunciation.en.toStdString(), FgColor::Green) + "]";
    }
    if (!wordResult.pronunciation.en.isEmpty() && !wordResult.pronunciation.am.isEmpty())
    {
        stringStream << INDENT;
    }
    if (!wordResult.pronunciation.am.isEmpty())
    {
        stringStream << QObject::tr("us").toStdString() + "[" + color(wordResult.pronunciation.am.toStdString(), FgColor::Green) + "]";
    }
    return stringStream.str();
}

std::string TerminalPrint::getMeanings()
{
    QStringList list;
    std::transform(wordResult.meanings.begin(), wordResult.meanings.end(), std::back_inserter(list), [](const Model::WordResult::Meaning &meaning) {
        return QString{"%1 %2"}
            .arg(color(meaning.part, FgColor::Green).data())
            .arg(meaning.means.join("; "));
    });
    QString meanings{list.join(BREAK_LINE.data())};

    return meanings.toStdString();
}

std::string TerminalPrint::color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor)
{
    std::string result{""};
    result += fgColors.at(fgColor);
    result += bgColors.at(bgColor);
    result += str;
    result += COLOR_CLOSE;
    return result;
}

std::string TerminalPrint::color(const QString &str, const FgColor &fgColor, const BgColor &bgColor)
{
    return TerminalPrint::color(str.toStdString(), fgColor, bgColor);
}

std::string TerminalPrint::getErrorStr(const std::string &str)
{
    std::ostringstream stringStream;
    stringStream << color(str, TerminalPrint::FgColor::Red)
                 << BREAK_LINE;
    return stringStream.str();
}

std::string TerminalPrint::getErrorStr(const QString &str)
{
    return getErrorStr(str.toStdString());
}
