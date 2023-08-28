#pragma once

#include <QObject>

namespace Model
{
    struct WordResult;
    struct SentenceResult;
} // namespace Model

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(QObject *parent = nullptr);
    ~Core();

    void start();

signals:
    void done();

private:
    void query(const QString &str);

private slots:
    void handleNetworkWord(const Model::WordResult &wordResult);
    void handleNetworkSentence(const Model::SentenceResult &sentenceResult);
    void handleNetworkError();
    void handleNetworkParseError();
};
