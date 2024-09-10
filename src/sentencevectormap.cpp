#include "nlp/sentencevectormap.h"

std::vector<double> sentencevectormap::get_vector(std::vector<std::string> tokens) {
    std::vector<double> avg(vecSize);

    for (std::string token : tokens) {
        std::vector<double> wordVec = wordMap.get_vector(token);
        //Add sum, N is the amount of tokens
        for (int i = 0; i < vecSize; i++) {
            avg[i] += wordVec[i];
        }
    }

    //Divide by N, leaving us with the average
    for (int i = 0; i < vecSize; i++) {
        avg[i] /= tokens.size();
    }
    return avg;
}

sentencevectormap::sentencevectormap(int vecSize) {
    this->vecSize = vecSize;
    this->wordMap = wordvectormap(vecSize);
}
