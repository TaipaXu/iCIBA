#include "./core.hpp"
#include <sstream>
#include <iostream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "./network.hpp"
#include "utils/terminalprint.hpp"
#include "models/wordresult.hpp"
#include "models/sentenceresult.hpp"

Core::Core(QObject *parent)
    : QObject{parent}
{
}

Core::~Core()
{
}

void Core::start()
{
    const QCommandLineOption queryOption{{"q", "query"}, "Translate the word or sentence.", "word or sentence"};
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(queryOption);

    parser.process(*qApp);

    if (parser.isSet(queryOption))
    {
        const QStringList arguments = parser.positionalArguments();
        const QString command = parser.value(queryOption);
        query(command + " " + arguments.join(" "));
    }
    else
    {
        const QStringList arguments = parser.positionalArguments();
        if (arguments.isEmpty())
        {
            parser.showHelp();
        }
        else
        {
            query(arguments.join(" "));
        }
    }
}

void Core::query(const QString &str)
{

    Network *network = new Network(this);
    connect(network, &Network::translateWordResult, this, &Core::handleNetworkWord);
    connect(network, &Network::translateSentenceResult, this, &Core::handleNetworkSentence);
    connect(network, &Network::networkError, this, &Core::handleNetworkError);
    connect(network, &Network::parseError, this, &Core::handleNetworkParseError);
    network->translate(str);
}

void Core::handleNetworkWord(const Model::WordResult &wordResult)
{
    TerminalPrint print{wordResult};
    std::cout << print.getStr();
    emit done();
}

void Core::handleNetworkSentence(const Model::SentenceResult &sentenceResult)
{
    TerminalPrint print{sentenceResult};
    std::cout << print.getStr();
    emit done();
}

void Core::handleNetworkError()
{
    std::cout << TerminalPrint::getNetworkErrorStr();
    emit done();
}

void Core::handleNetworkParseError()
{
    std::cout << TerminalPrint::getNetworkParseErrorStr();
    emit done();
}
