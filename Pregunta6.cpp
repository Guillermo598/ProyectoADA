#include <iostream>
#include <vector>

std::vector<int> getBlocks(std::vector<int> array);
std::vector<int> getVectorFromString(std::string);
double GreedyMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching);
double GreedyMinTransform(std::vector<std::string> matrixA, std::vector<std::string> matrixB,
                          std::vector<std::vector<std::pair<int,int>>>* transform);

int main() {
    int q;
    std::cin >> q;
    std::vector<std::string> matrixA(q), matrixB(q);
    std::string s;
    std::cout << "A:\n";
    for (int i = 0; i < q; ++i) {
        std::cin >> s;
        matrixA[i] = s;
    }
    std::cout << "B:\n";
    for (int i = 0; i < q; ++i) {
        std::cin >> s;
        matrixB[i] = s;
    }
    std::vector<std::vector<std::pair<int,int>>> transform(q);
    double peso = GreedyMinTransform(matrixA, matrixB, &transform);
    for (const auto& matching : transform) {
        for (auto pair : matching)
            std::cout << "(" << pair.first << "," << pair.second << ")";
        std::cout << "\n";
    }
    std::cout << "Peso: " << peso << std::endl;
}

double GreedyMinTransform(std::vector<std::string> matrixA, std::vector<std::string> matrixB,
                          std::vector<std::vector<std::pair<int,int>>>* transform) {
    int q = matrixA.size();
    double peso = 0;
    for (int i = 0; i < q; ++i) {
        std::vector<int> A = getVectorFromString(matrixA[i]);
        std::vector<int> B = getVectorFromString(matrixB[i]);
        std::vector<std::pair<int,int>> matching;
        peso += GreedyMinMatching(A, B, &matching);
        (*transform)[i] = matching;
    }
    return peso;
}

double GreedyMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching) {
    std::vector<int> blocksA = getBlocks(std::move(A));
    std::vector<int> blocksB = getBlocks(std::move(B));
    int m = blocksA.size();
    int n = blocksB.size();
    double peso = 0;
    if (m > n) {    // Agrupacion
        float ratio = (float)m / (float)n;
        float max_i = ratio;
        int j = 1;
        int num = 0;
        for (int i = 1; i <= m; ++i) {
            if ((float)i <= max_i) {
                matching->emplace_back(i, j);
                num += blocksA[i-1];
            }
            else {
                peso += (double)num / blocksB[j-1];
                num = blocksA[i-1];
                max_i += ratio;
                j++;
                matching->emplace_back(i, j);
            }
        }
        peso += (double)num / blocksB[j-1];
    } else {        // Division
        float ratio = (float)n / (float)m;
        float max_j = ratio;
        int i = 1;
        int den = 0;
        for (int j = 1; j <= n; ++j) {
            if ((float)j <= max_j) {
                matching->emplace_back(i, j);
                den += blocksB[j-1];
            }
            else {
                peso += (double)blocksA[i-1] / den;
                den = blocksB[j-1];
                max_j += ratio;
                i++;
                matching->emplace_back(i, j);
            }
        }
        peso += (double)blocksA[i-1] / den;
    }
    return peso;
}

std::vector<int> getBlocks(std::vector<int> array) {
    std::vector<int> blocks;
    auto arrayIt = array.begin();
    while (arrayIt != array.end()) {
        while (*arrayIt == 0 && arrayIt != array.end())
            arrayIt++;
        int size = 0;
        if (*arrayIt == 1) {
            while (*arrayIt == 1) {
                size++;
                arrayIt++;
            }
            blocks.emplace_back(size);
        }
    }
    return blocks;
}

std::vector<int> getVectorFromString(std::string s) {
    std::vector<int> v(s.size());
    for (int i = 0; i < s.size(); ++i)
        v[i] = s[i] - '0';
    return v;
}
