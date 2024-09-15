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

    std::string getStr() const;

    const Type type;
    const Model::WordResult wordResult;
    const Model::SentenceResult sentenceResult;

private:
    virtual std::string getWordStr() const = 0;
    virtual std::string getSentenceStr() const = 0;
    virtual std::string getPronunciation() const = 0;
    virtual std::string getMeanings() const = 0;
};
