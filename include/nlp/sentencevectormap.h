#pragma once

#include "wordvectormap.h"

class sentencevectormap {
private:
    int vecSize;
    wordvectormap wordMap;

public:
    sentencevectormap(int vecSize);
    std::vector<double> get_vector(std::vector<std::string> tokens);
};