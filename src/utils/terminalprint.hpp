#include "baseprint.hpp"
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

private:
    std::string getWordStr() const override;
    std::string getSentenceStr() const override;
    std::string getPronunciation() const override;
    std::string getMeanings() const override;
    static std::string color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor = BgColor::Normal);
    static std::string getErrorStr(const std::string &str);

    static const std::map<FgColor, std::string> fgColors;
    static const std::map<BgColor, std::string> bgColors;
    static const std::string COLOR_CLOSE;
    static const std::string BREAK_LINE;
    static const std::string INDENT;
};
