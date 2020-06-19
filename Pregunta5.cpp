#include <iostream>
#include <vector>
#define INF 100000000

std::vector<int> getBlocks(std::vector<int> array);
std::vector<int> getVectorFromString(std::string);
double DPMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching);
void DPMinMatchingRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                      std::vector<std::pair<int,int>>** matching, double** memo);

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
    double** memo;
    std::vector<std::pair<int,int>>** matching;
    memo = new double*[m];
    matching = new std::vector<std::pair<int,int>>*[m];
    for (int i = 0; i < m; ++i) {
        memo[i] = new double[n];
        matching[i] = new std::vector<std::pair<int, int>>[n];
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            DPMinMatchingRec(blocksA, blocksB, i, j, matching, memo);
    }
    *Matching = matching[m-1][n-1];
    return memo[m-1][n-1];
}

void DPMinMatchingRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                      std::vector<std::pair<int,int>>** matching, double** memo) {
    if (i == 0) {
        double peso = blocksA[0];
        int den = 0;
        for (int jj = 0; jj <= j; ++jj) {
            den += blocksB[jj];
            matching[0][j].emplace_back(1, jj + 1);
        }
        peso /= den;
        memo[0][j] = peso;
    } else if (j == 0) {
        double peso = 0;
        for (int ii = 0; ii <= i; ++ii) {
            peso += blocksA[ii];
            matching[i][0].emplace_back(ii + 1, 1);
        }
        peso /= blocksB[0];
        memo[i][0] = peso;
    } else {
        memo[i][j] = INF;
        for (int x = 0; x < i; ++x) { //Agrupar
            double agrupar = memo[x][j-1];
            std::vector<std::pair<int, int>> newMatching(matching[x][j-1]);
            double num = 0;
            for (int n = x + 1; n <= i; ++n) {
                num += blocksA[n];
                newMatching.emplace_back(n + 1, j + 1);
            }
            agrupar += num / blocksB[j];
            if (agrupar < memo[i][j]) {
                memo[i][j] = agrupar;
                matching[i][j] = newMatching;
            }
        }
        for (int y = 0; y < j; ++y) { //Dividir
            double dividir = memo[i-1][y];
            std::vector<std::pair<int, int>> newMatching(matching[i-1][y]);
            double den = 0;
            for (int n = y + 1; n <= j; ++n) {
                den += blocksB[n];
                newMatching.emplace_back(i + 1, n + 1);
            }
            dividir += blocksA[i] / den;
            if (dividir < memo[i][j]) {
                memo[i][j] = dividir;
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
