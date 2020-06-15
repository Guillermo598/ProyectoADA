#include <iostream>
#include <vector>
#include <sstream>
#define INF 100000000

std::vector<int> getBlocks(std::vector<int> array);
std::vector<int> getVectorFromString(std::string);
double MemoMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching);
double MemoMinMatchingRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                          std::vector<std::pair<int,int>>** matching, double** memo);

int main() {
    std::string stringA, stringB;
    std::getline(std::cin, stringA);
    std::getline(std::cin, stringB);
    std::vector<int> A = getVectorFromString(stringA);
    std::vector<int> B = getVectorFromString(stringB);

    std::vector<std::pair<int,int>> matching;
    double peso = MemoMinMatching(A, B, &matching);
    for (auto pair : matching) {
        std::cout << "(" << pair.first << "," << pair.second << ")\n";
    }
    std::cout << "Peso: " << peso << std::endl;
}

std::vector<int> getBlocks(std::vector<int> array) {
    std::vector<int> blocks;
    auto arrayIt = array.begin();
    while (arrayIt != array.end()) {
        while (*arrayIt == 0)
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

double MemoMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* Matching) {
    std::vector<int> blocksA = getBlocks(A);
    std::vector<int> blocksB = getBlocks(B);
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
            memo[i][j] = INF;
    }
    double peso = MemoMinMatchingRec(blocksA, blocksB, m-1, n-1, matching, memo);
    *Matching = matching[m-1][n-1];
    return peso;
}

double MemoMinMatchingRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                          std::vector<std::pair<int,int>>** matching, double** memo) {
    if (memo[i][j] != INF)
        return memo[i][j];
    double peso = 0;
    if (i == 0) {
        peso = blocksA[0];
        int den = 0;
        for (int n = 0; n <= j; ++n) {
            den += blocksB[n];
            matching[i][j].emplace_back(1, n + 1);
        }
        peso /= den;
        memo[i][j] = peso;
        return peso;
    }
    if (j == 0) {
        for (int n = 0; n <= i; ++n) {
            peso += blocksA[n];
            matching[i][j].emplace_back(n + 1, 1);
        }
        peso /= blocksB[0];
        memo[i][j] = peso;
        return peso;
    }
    peso = INF;
    for (int x = 0; x <= i-1; ++x) { //Agrupar
        double agrupar = MemoMinMatchingRec(blocksA, blocksB, x,j-1, matching, memo);
        std::vector<std::pair<int,int>> newMatching (matching[x][j-1]);
        double num = 0;
        for (int n = x+1; n <= i; ++n) {
            num += blocksA[n];
            newMatching.emplace_back(n+1, j+1);
        }
        agrupar += num/blocksB[j];
        if (agrupar < peso) {
            peso = agrupar;
            matching[i][j] = newMatching;
        }
    }
    for (int y = 0; y <= j-1; ++y) { //Dividir
        double dividir = MemoMinMatchingRec(blocksA, blocksB, i-1, y, matching, memo);
        std::vector<std::pair<int,int>> newMatching (matching[i-1][y]);
        double den = 0;
        for (int n = y+1; n <= j; ++n) {
            den += blocksB[n];
            newMatching.emplace_back(i+1, n+1);
        }
        dividir += blocksA[i] / den;
        if (dividir < peso) {
            peso = dividir;
            matching[i][j] = newMatching;
        }
    }
    memo[i][j] = peso;
    return peso;
}

std::vector<int> getVectorFromString(std::string s) {
    std::vector<int> v;
    int i;
    std::stringstream stream(s);
    while (stream >> i)
        v.push_back(i);
    return v;
}
