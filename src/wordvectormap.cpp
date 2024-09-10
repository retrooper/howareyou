#include "nlp/wordvectormap.h"

std::vector<double> wordvectormap::random_vector() {
    std::vector<double> random(vecSize);

    for (int i = 0; i < vecSize; i++) {
        //Random value between 0 and 1
        double value = (double) std::rand() / RAND_MAX;
        random.push_back(value);
    }
    return random;
}

wordvectormap::wordvectormap(int vecSize) {
    this->vecSize = vecSize;
}

std::vector<double> wordvectormap::get_vector(const std::string &word) {
    if (auto vec = wordVectorMap.find(word); vec != wordVectorMap.end()) {
        return vec->second;
    }
    else {
        wordVectorMap[word] = random_vector();
    }
}



