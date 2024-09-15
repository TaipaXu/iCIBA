#pragma once

#include <string>
#include <vector>

namespace Model
{
    struct WordResult
    {
        struct Meaning
        {
            std::string part;
            std::vector<std::string> means;
        };
        using Meanings = std::vector<Meaning>;

        struct Pronunciation
        {
            std::string en;
            std::string am;
            std::string other;
        };

        std::string word;
        Meanings meanings;
        Pronunciation pronunciation;
    };
} // namespace Model
