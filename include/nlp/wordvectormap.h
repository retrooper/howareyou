#pragma onceonce
#include <unordered_map>
class wordvectormap {
private:
    int vecSize;
    std::unordered_map<std::string, std::vector<double>> wordVectorMap;

    std::vector<double> random_vector();
public:
    wordvectormap(int vecSize);

    std::vector<double> get_vector(const std::string &word);
};