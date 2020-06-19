#include <iostream>
#include <vector>
#define INF 100000000

std::vector<int> getBlocks(std::vector<int> array);
std::vector<int> getVectorFromString(std::string);
double DPMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching);
void DPMinMatchingRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                      std::vector<std::pair<int,int>>** matching, double** peso);

int main() {
    std::string stringA, stringB;
    std::cin >> stringA;
    std::cin >> stringB;
    std::vector<int> A = getVectorFromString(stringA);
    std::vector<int> B = getVectorFromString(stringB);

    std::vector<std::pair<int,int>> matching;
    double peso = DPMinMatching(A, B, &matching);
    for (auto pair : matching) {
        std::cout << "(" << pair.first << "," << pair.second << ")\n";
    }
    std::cout << "Peso: " << peso << std::endl;
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

double DPMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* Matching) {
    std::vector<int> blocksA = getBlocks(std::move(A));
    std::vector<int> blocksB = getBlocks(std::move(B));
    int m = blocksA.size();
    int n = blocksB.size();
    auto peso = new double*[m];
    auto matching = new std::vector<std::pair<int,int>>*[m];
    for (int i = 0; i < m; ++i) {
        peso[i] = new double[n];
        matching[i] = new std::vector<std::pair<int, int>>[n];
        for (int j = 0; j < n; ++j)
            DPMinMatchingRec(blocksA, blocksB, i, j, matching, peso);
    }
    *Matching = matching[m-1][n-1];
    return peso[m-1][n-1];
}

void DPMinMatchingRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                      std::vector<std::pair<int,int>>** matching, double** peso) {
    if (i == 0) {
        double w = blocksA[0];
        int den = 0;
        for (int jj = 0; jj <= j; ++jj) {
            den += blocksB[jj];
            matching[0][j].emplace_back(1, jj + 1);
        }
        w /= den;
        peso[0][j] = w;
    } else if (j == 0) {
        double w = 0;
        for (int ii = 0; ii <= i; ++ii) {
            w += blocksA[ii];
            matching[i][0].emplace_back(ii + 1, 1);
        }
        w /= blocksB[0];
        peso[i][0] = w;
    } else {
        peso[i][j] = INF;
        for (int x = 0; x < i; ++x) { //Agrupar
            double w = peso[x][j-1];
            std::vector<std::pair<int, int>> newMatching(matching[x][j-1]);
            double num = 0;
            for (int n = x + 1; n <= i; ++n) {
                num += blocksA[n];
                newMatching.emplace_back(n + 1, j + 1);
            }
            w += num / blocksB[j];
            if (w < peso[i][j]) {
                peso[i][j] = w;
                matching[i][j] = newMatching;
            }
        }
        for (int y = 0; y < j; ++y) { //Dividir
            double w = peso[i-1][y];
            std::vector<std::pair<int, int>> newMatching(matching[i-1][y]);
            double den = 0;
            for (int n = y + 1; n <= j; ++n) {
                den += blocksB[n];
                newMatching.emplace_back(i + 1, n + 1);
            }
            w += blocksA[i] / den;
            if (w < peso[i][j]) {
                peso[i][j] = w;
                matching[i][j] = newMatching;
            }
        }
    }
}

std::vector<int> getVectorFromString(std::string s) {
    std::vector<int> v(s.size());
    for (int i = 0; i < s.size(); ++i)
        v[i] = s[i] - '0';
    return v;
}
