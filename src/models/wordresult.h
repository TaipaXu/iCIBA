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

#include <QStringList>
#include <list>

namespace Model
{
struct WordResult
{
    struct Exchange
    {
        QStringList pl;
        QStringList third;
        QStringList past;
        QStringList done;
        QStringList ing;
        QStringList er;
        QStringList est;
        QStringList prep;
        QStringList adv;
        QStringList verv;
        QStringList noun;
        QStringList adj;
        QStringList conn;

        QStringList &operator[](const QString &key)
        {
            if (key == "pl")
            {
                return pl;
            }
            else if (key == "third")
            {
                return third;
            }
            else if (key == "past")
            {
                return past;
            }
            else if (key == "done")
            {
                return done;
            }
            else if (key == "ing")
            {
                return ing;
            }
            else if (key == "er")
            {
                return er;
            }
            else if (key == "est")
            {
                return est;
            }
            else if (key == "prep")
            {
                return prep;
            }
            else if (key == "adv")
            {
                return adv;
            }
            else if (key == "verv")
            {
                return verv;
            }
            else if (key == "noun")
            {
                return noun;
            }
            else if (key == "adj")
            {
                return adj;
            }
            else if (key == "conn")
            {
                return conn;
            }
            else
            {
            }
        }
    };

    struct Meaning
    {
        QString part;
        QStringList means;
    };
    using Meanings = std::list<Meaning>;

    struct Pronunciation
    {
        QString en;
        QString am;
        QString other;
        QString enVoice;
        QString amVoice;
        QString ttsVoice;
    };

    QString word;
    Exchange exchange;
    Meanings meanings;
    Pronunciation pronunciation;
};
} // namespace Model
