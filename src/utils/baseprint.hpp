#include <string>
#include "models/wordresult.hpp"
#include "models/sentenceresult.hpp"

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
};
