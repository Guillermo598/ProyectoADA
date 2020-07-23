#include <iostream>
#include <vector>
#define INF 100000000

std::vector<int> getBlocks(std::vector<int> array);
std::vector<int> getVectorFromString(std::string);
double DPMinMatchingMean(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching);
void DPMinMatchingMeanRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                      std::vector<std::pair<int,int>>** matching, double** peso, double mu);
double DPMinTransformMean(std::vector<std::string> matrixA, std::vector<std::string> matrixB,
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
    double peso = DPMinTransformMean(matrixA, matrixB, &transform);
    for (const auto& matching : transform) {
        for (auto pair : matching)
            std::cout << "(" << pair.first << "," << pair.second << ")";
        std::cout << "\n";
    }
    std::cout << "Peso: " << peso << std::endl;
}

double DPMinTransformMean(std::vector<std::string> matrixA, std::vector<std::string> matrixB,
                      std::vector<std::vector<std::pair<int,int>>>* transform) {
    int q = matrixA.size();
    double peso = 0;
    for (int i = 0; i < q; ++i) {
        std::vector<int> A = getVectorFromString(matrixA[i]);
        std::vector<int> B = getVectorFromString(matrixB[i]);
        std::vector<std::pair<int,int>> matching;
        peso += DPMinMatchingMean(A, B, &matching);
        (*transform)[i] = matching;
    }
    return peso;
}

double DPMinMatchingMean(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* Matching) {
    std::vector<int> blocksA = getBlocks(std::move(A));
    std::vector<int> blocksB = getBlocks(std::move(B));
    int m = blocksA.size();
    int n = blocksB.size();
    int muNum = 0;
    for (int a : blocksA)
        muNum += a;
    int muDen = 0;
    for (int b : blocksB)
        muDen += b;
    double mu = (double)muNum/muDen;
    auto peso = new double*[m];
    auto matching = new std::vector<std::pair<int,int>>*[m];
    for (int i = 0; i < m; ++i) {
        peso[i] = new double[n];
        matching[i] = new std::vector<std::pair<int, int>>[n];
        for (int j = 0; j < n; ++j)
            DPMinMatchingMeanRec(blocksA, blocksB, i, j, matching, peso, mu);
    }
    *Matching = matching[m-1][n-1];
    return peso[m-1][n-1];
}

void DPMinMatchingMeanRec(std::vector<int> blocksA, std::vector<int> blocksB, int i, int j,
                          std::vector<std::pair<int,int>>** matching, double** peso, double mu) {
    if (i == 0) {
        double w = blocksA[0];
        int den = 0;
        for (int jj = 0; jj <= j; ++jj) {
            den += blocksB[jj];
            matching[0][j].emplace_back(1, jj + 1);
        }
        w /= den;
        w = w - mu;
        if (w < 0)
            w *= -1;
        peso[0][j] = w;
    } else if (j == 0) {
        double w = 0;
        for (int ii = 0; ii <= i; ++ii) {
            w += blocksA[ii];
            matching[i][0].emplace_back(ii + 1, 1);
        }
        w /= blocksB[0];
        w = w - mu;
        if (w < 0)
            w *= -1;
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
            w = w - mu;
            if (w < 0)
                w *= -1;
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
            w = w - mu;
            if (w < 0)
                w *= -1;
            if (w < peso[i][j]) {
                peso[i][j] = w;
                matching[i][j] = newMatching;
            }
        }
    }
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
