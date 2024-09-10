#include "tokenizer.h"
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> tokenizer::tokenize(const std::string &text, const char delimiter) {
    std::string temp;
    std::stringstream ss(text);
    std::vector<std::string> tokens;
    while (std::getline(ss, temp, delimiter)) {
        tokens.push_back(temp);
    }

    return tokens;
}
