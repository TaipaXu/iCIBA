#include "./baseprint.hpp"

BasePrint::BasePrint(const Model::WordResult &wordResult)
    : type{Type::Word},
      wordResult{wordResult}
{
}

BasePrint::BasePrint(const Model::SentenceResult &sentenceResult)
    : type{Type::Sentence},
      sentenceResult{sentenceResult}
{
}

std::string BasePrint::getStr() const
{
    if (type == Type::Word)
    {
        return getWordStr();
    }
    else
    {
        return getSentenceStr();
    }
}
