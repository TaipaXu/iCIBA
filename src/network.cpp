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

#include "network.h"
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include <vector>
#include "../models/wordresult.h"
#include "../models/sentenceresult.h"

static const QString SUGGEST_URL{"http://dict-mobile.iciba.com/interface/index.php"};
static const QString WORD_URL{"http://www.iciba.com/index.php"};
static const QString SENTENCE_URL{"http://fy.iciba.com/ajax.php"};

Network::Network(QObject *parent)
    : QObject{parent},
      suggestManager{nullptr},
      suggestReply{nullptr},
      translateWordManager{nullptr},
      translateWordReply{nullptr},
      translateSentenceManager{nullptr},
      translateSentenceReply{nullptr}
{
}

Network::~Network()
{
}

void Network::suggest(const QString &word)
{
    if (suggestReply)
    {
        suggestReply->abort();
        suggestReply = nullptr;
    }

    QNetworkRequest request;
    QUrl url(SUGGEST_URL);
    QUrlQuery query;
    query.addQueryItem("c", "word");
    query.addQueryItem("m", "getsuggest");
    query.addQueryItem("nums", "10");
    query.addQueryItem("client", "6");
    query.addQueryItem("is_need_mean", "1");
    query.addQueryItem("word", word);
    url.setQuery(query);
    request.setUrl(url);

    if (!suggestManager)
    {
        suggestManager = new QNetworkAccessManager{this};
        connect(suggestManager, &QNetworkAccessManager::finished, this, &Network::handleSuggestResult);
    }

    suggestReply = suggestManager->get(request);
}

void Network::translateWord(const QString &word)
{
    if (translateWordReply)
    {
        translateWordReply->abort();
        translateWordReply = nullptr;
    }

    QNetworkRequest request;
    QUrl url(WORD_URL);
    QUrlQuery query;
    query.addQueryItem("a", "getWordMean");
    query.addQueryItem("c", "search");
    query.addQueryItem("list", "1,2,3,4,5,8,9,10,12,13,14,18,21,22,3003,3005");
    query.addQueryItem("word", word);
    url.setQuery(query);
    request.setUrl(url);

    if (!translateWordManager)
    {
        translateWordManager = new QNetworkAccessManager(this);
        connect(translateWordManager, &QNetworkAccessManager::finished, this, &Network::handleTranslateWordResult);
    }

    translateWordReply = translateWordManager->get(request);
}

void Network::translateSentence(const QString &sentence)
{
    if (translateSentenceReply)
    {
        translateSentenceReply->abort();
        translateSentenceReply = nullptr;
    }

    QNetworkRequest request;
    QUrl url(SENTENCE_URL);
    QUrlQuery query;
    query.addQueryItem("a", "fy");
    query.addQueryItem("f", "auto");
    query.addQueryItem("t", "auto");
    query.addQueryItem("w", sentence);
    url.setQuery(query);
    request.setUrl(url);

    if (!translateSentenceManager)
    {
        translateSentenceManager = new QNetworkAccessManager{this};
        connect(translateSentenceManager, &QNetworkAccessManager::finished, this, &Network::handleTranslateSentenceResult);
    }

    translateSentenceReply = translateSentenceManager->get(request);
}

void Network::handleSuggestResult(QNetworkReply *reply)
{
    const QString all = reply->readAll();
    QJsonParseError jsonError;
    const QJsonDocument document = QJsonDocument::fromJson(all.toUtf8(), &jsonError);
    if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
    {
        QStringList result{};
        const QJsonObject object = document.object();
        const QJsonArray messages = object["message"].toArray();
        for (auto &&message : messages)
        {
            const QString word = message.toObject()["key"].toString();
            result.append(word);
        }
        emit suggestResult(result);
    }

    reply->deleteLater();
    suggestReply = nullptr;
}

void Network::handleTranslateWordResult(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit networkError();
    }
    else
    {
        const QString all = reply->readAll();
        QJsonParseError jsonError;
        const QJsonDocument document = QJsonDocument::fromJson(all.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            try
            {
                const QJsonObject object = document.object();
                const QJsonObject baseInfo = object["baesInfo"].toObject();

                const int type = baseInfo["translate_type"].toInt();
                if (type == 1)
                {
                    Model::WordResult wordResult;
                    wordResult.word = baseInfo["word_name"].toString();

                    QJsonObject jsonExchange = baseInfo["exchange"].toObject();
                    for (auto &&i : QStringList{"pl", "third", "past", "done", "ing", "er", "est", "prep", "adv", "verv", "noun", "adj", "conn"})
                    {
                        const QJsonArray jsonList = jsonExchange["word_" + i].toArray();
                        std::transform(jsonList.begin(), jsonList.end(), std::back_inserter(wordResult.exchange[i]), [](auto item) {
                            return item.toString();
                        });
                    }

                    const QJsonObject jsonSymbol = baseInfo["symbols"].toArray()[0].toObject();
                    const QJsonArray jsonParts = jsonSymbol["parts"].toArray();
                    for (auto &&part : jsonParts)
                    {
                        QStringList means{};
                        std::transform(part.toObject()["means"].toArray().begin(), part.toObject()["means"].toArray().end(), std::back_inserter(means), [](auto item) {
                            return item.toString();
                        });
                        wordResult.meanings.push_back(Model::WordResult::Meaning{
                            part.toObject()["part"].toString(),
                            means,
                        });
                    }

                    wordResult.pronunciation = {
                        jsonSymbol["ph_en"].toString(),
                        jsonSymbol["ph_am"].toString(),
                        jsonSymbol["ph_other"].toString(),
                        jsonSymbol["ph_en_mp3"].toString(),
                        jsonSymbol["ph_am_mp3"].toString(),
                        jsonSymbol["ph_tts_mp3"].toString(),
                    };

                    emit translateWordResult(wordResult);
                }
                else if (type == 2)
                {
                    Model::SentenceResult sentenceResult;
                    sentenceResult.sentence = baseInfo["word_name"].toString();
                    sentenceResult.translation = baseInfo["translate_result"].toString();
                    emit translateSentenceResult(sentenceResult);
                }
                else
                {
                    emit parseError();
                }
            }
            catch (...)
            {
                emit parseError();
            }
        }
        else
        {
            emit parseError();
        }
    }

    reply->deleteLater();
    translateWordReply = nullptr;
}

void Network::handleTranslateSentenceResult(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit networkError();
    }
    else
    {
        const QString all = reply->readAll();
        QJsonParseError jsonError;
        const QJsonDocument document = QJsonDocument::fromJson(all.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            try
            {
                const QJsonObject object = document.object();
                const int status = object["status"].toInt();
                const QJsonObject content = object["content"].toObject();
                if (status == 1)
                {
                    const QString wordMean = content["out"].toString();
                    Model::SentenceResult sentenceResult;
                    sentenceResult.sentence = QString{""};
                    sentenceResult.translation = wordMean;
                    emit translateSentenceResult(sentenceResult);
                }
                else if (status == 0)
                {
                    const QJsonArray wordMeans = content.value("word_mean").toArray();
                    QString result{};
                    for (auto &&wordMean : wordMeans)
                    {
                        result.append(wordMean.toString());
                        result.append("\n");
                    }

                    Model::SentenceResult sentenceResult;
                    sentenceResult.sentence = QString{""};
                    sentenceResult.translation = result;
                    emit translateSentenceResult(sentenceResult);
                }
            }
            catch (...)
            {
                emit parseError();
            }
        }
        else
        {
            emit parseError();
        }
    }

    reply->deleteLater();
    translateSentenceReply = nullptr;
}
