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

#include <QWidget>

namespace Ui
{
class TranslateWord;
}
QT_BEGIN_NAMESPACE
class QCompleter;
class QStringListModel;
class Network;
class QMediaPlayer;
QT_END_NAMESPACE
namespace Model
{
struct WordResult;
struct SentenceResult;
} // namespace Model

class TranslateWord : public QWidget
{
    Q_OBJECT

public:
    explicit TranslateWord(QWidget *parent = nullptr);
    ~TranslateWord();

private slots:
    void handleSuggestResult(const QStringList &result);
    void handleCompleterActivated(const QString &text);
    void translate();
    void handleTranslateWordResult(const Model::WordResult &wordResult);
    void handleTranslateSentenceResult(const Model::SentenceResult &sentenceResult);
    void handleNetworkError();
    void handleNetworkParseError();
    void handleAnchorClicked(const QUrl &link);

private:
    Ui::TranslateWord *ui;
    QCompleter *completer;
    QStringListModel *completeModel;
    Network *network;
    QMediaPlayer *player;
};
