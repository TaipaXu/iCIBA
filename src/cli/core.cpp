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

#include "core.h"
#include <QTranslator>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <sstream>
#include <iostream>
#include "../network.h"
#include "../utils/terminalprint.h"
#include "../persistence/settings.h"
#include "../persistence/history.h"
#include "../models/wordresult.h"
#include "../models/sentenceresult.h"

Core::Core(QObject *parent)
    : QObject{parent},
      settings{Settings::getInstance()}
{
    installTranslator();
}

Core::~Core()
{
    settings->destory();
}

void Core::start()
{
    const QCommandLineOption queryOption{{"q", "query"}, "Translate the word or sentence.", "word or sentence"};
    QCommandLineOption historyOption{{"i", "history"}, "Get the history", "<number>"};
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(queryOption);
    parser.addOption(historyOption);

    parser.process(*qApp);

    if (parser.isSet(historyOption))
    {
        const QString countStr = parser.value(historyOption);
        int count = countStr.toInt();
        count = std::max(count, 0);

        History *history = new History();
        const QStringList histories = history->getHistories(count);
        const std::string result = histories.join("\n").toStdString();
        std::cout << result
                  << std::endl;
        delete history;
        exit(0);
    }
    else if (parser.isSet(queryOption))
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

void Core::installTranslator()
{
    const QString currentLanguage = settings->getLanguage();

    if (currentLanguage == "zh_CN")
    {
        QTranslator *translator = new QTranslator(this);
        translator->load(":/i18n/zh_CN");
        qApp->installTranslator(translator);
    }
}

void Core::query(const QString &str)
{

    Network *network = new Network{this};
    connect(network, &Network::translateWordResult, this, &Core::handleNetworkWord);
    connect(network, &Network::translateSentenceResult, this, &Core::handleNetworkSentence);
    connect(network, &Network::networkError, this, &Core::handleNetworkError);
    connect(network, &Network::parseError, this, &Core::handleNetworkParseError);
    network->translateWord(str);

    History *history = new History();
    history->addHistory(str);
    delete history;
}
