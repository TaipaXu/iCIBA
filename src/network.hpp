#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QNetworkReply;
QT_END_NAMESPACE

namespace Model
{
    struct WordResult;
    struct SentenceResult;
} // namespace Model

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(QObject *parent = nullptr);
    ~Network() = default;

    void translate(const QString &word);

signals:
    void translateWordResult(const Model::WordResult &wordResult);
    void translateSentenceResult(const Model::SentenceResult &sentenceResult);
    void networkError();
    void parseError();

private slots:
    void handleTranslateResult(QNetworkReply *reply);
};
