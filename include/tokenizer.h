#pragma once

#include <vector>
#include <string>
class tokenizer {
public:
    tokenizer() = default;
    std::vector<std::string> tokenize(const std::string &text, const char delimiter);
};