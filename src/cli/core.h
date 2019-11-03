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

class Settings;
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

private slots:
    void handleNetworkWord(const Model::WordResult &wordResult);
    void handleNetworkSentence(const Model::SentenceResult &sentenceResult);
    void handleNetworkError();
    void handleNetworkParseError();

private:
    void installTranslator();
    void query(const QString &str);

    Settings *settings;
};
