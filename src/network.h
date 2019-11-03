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

#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
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
    ~Network();

    void suggest(const QString &word);
    void translateWord(const QString &word);
    void translateSentence(const QString &word);

signals:
    void suggestResult(const QStringList &result);
    void translateWordResult(const Model::WordResult &wordResult);
    void translateSentenceResult(const Model::SentenceResult &sentenceResult);
    void networkError();
    void parseError();

private slots:
    void handleSuggestResult(QNetworkReply *reply);
    void handleTranslateWordResult(QNetworkReply *reply);
    void handleTranslateSentenceResult(QNetworkReply *reply);

private:
    QNetworkAccessManager *suggestManager;
    QNetworkReply *suggestReply;
    QNetworkAccessManager *translateWordManager;
    QNetworkReply *translateWordReply;
    QNetworkAccessManager *translateSentenceManager;
    QNetworkReply *translateSentenceReply;
};
