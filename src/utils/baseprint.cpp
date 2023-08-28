#include "./baseprint.hpp"
#include <QObject>

BasePrint::BasePrint(const Model::WordResult &wordResult)
    : wordResult{wordResult},
      type{Type::Word}
{
}

BasePrint::BasePrint(const Model::SentenceResult &sentenceResult)
    : sentenceResult{sentenceResult},
      type{Type::Sentence}
{
}

std::string BasePrint::getStr()
{
    if (type == Type::Word)
    {
        return getWordStr();
    }
    else if (type == Type::Sentence)
    {
        return getSentenceStr();
    }
}

std::string BasePrint::getNetworkErrorStr()
{
    return QObject::tr("Network error").toStdString();
}

std::string BasePrint::getNetworkParseErrorStr()
{
    return QObject::tr("Network parsing error").toStdString();
}
