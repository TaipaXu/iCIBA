#include "./network.hpp"
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include "models/wordresult.hpp"
#include "models/sentenceresult.hpp"

static const QString TRANSLATE_URL{"http://dict-pc.iciba.com/interface/index.php"};

Network::Network(QObject *parent)
    : QObject{parent}
{
}

void Network::translate(const QString &word)
{
    QNetworkRequest request;
    QUrl url(TRANSLATE_URL);
    QUrlQuery query;
    query.addQueryItem("client", "5");
    query.addQueryItem("type", "1");
    query.addQueryItem("timestamp", "1557025419");
    query.addQueryItem("uuid", "CB5082D19C82440F836DE3AED8E5FEB5");
    query.addQueryItem("c", "word");
    query.addQueryItem("m", "index");
    query.addQueryItem("v", "2016.3.3.0333");
    query.addQueryItem("sign", "cf2decaa9965af29");
    query.addQueryItem("list", "1");
    query.addQueryItem("word", word);
    url.setQuery(query);
    request.setUrl(url);

    QNetworkAccessManager *translateManager = new QNetworkAccessManager();
    connect(translateManager, &QNetworkAccessManager::finished, this, &Network::handleTranslateResult);
    translateManager->get(request);
}

void Network::handleTranslateResult(QNetworkReply *reply)
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
                const QJsonObject messageObject = object["message"].toObject();
                const QJsonObject baseInfo = messageObject["baesInfo"].toObject();

                const int type = baseInfo["translate_type"].toInt();
                if (type == 1)
                {
                    Model::WordResult wordResult;
                    wordResult.word = baseInfo["word_name"].toString();

                    const QJsonObject jsonSymbol = baseInfo["symbols"].toArray()[0].toObject();
                    const QJsonArray jsonParts = jsonSymbol["parts"].toArray();
                    for (auto &&part : jsonParts)
                    {
                        QStringList means{};
                        for (auto &&mean : part.toObject()["means"].toArray())
                        {
                            means.push_back(mean.toString());
                        }
                        wordResult.meanings.push_back(Model::WordResult::Meaning{
                            part.toObject()["part"].toString(),
                            means,
                        });
                    }

                    wordResult.pronunciation = {
                        jsonSymbol["ph_en"].toString(),
                        jsonSymbol["ph_am"].toString(),
                        jsonSymbol["ph_other"].toString(),
                    };

                    emit translateWordResult(wordResult);
                }
                else if (type == 2)
                {
                    Model::SentenceResult sentenceResult;
                    sentenceResult.sentence = messageObject["word_name"].toString();
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
    QNetworkAccessManager *manager = qobject_cast<QNetworkAccessManager *>(sender());
    manager->deleteLater();
}
