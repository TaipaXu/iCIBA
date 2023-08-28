#pragma once

#include <list>
#include <QStringList>

namespace Model
{
    struct WordResult
    {
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
        };

        QString word;
        Meanings meanings;
        Pronunciation pronunciation;
    };
} // namespace Model
