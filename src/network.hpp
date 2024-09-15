#pragma once

#include <string>
#include <map>
#include <variant>
#include "models/wordresult.hpp"
#include "models/sentenceresult.hpp"

class Network
{
public:
    Network() = default;
    ~Network() = default;

    std::variant<Model::WordResult, Model::SentenceResult> translate(const std::string &text);

private:
    std::string makeQueryString(const std::map<std::string, std::string> &params) const;
};
