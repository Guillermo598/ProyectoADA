#include <iostream>
#include <vector>

std::vector<std::pair<int,int>> minMatching(std::vector<int> A, std::vector<int> B, int p, double* peso) {
    std::vector<int> Asizes, Bsizes;
    std::vector<std::pair<int, int>> Matching;
    auto it = A.begin();
    while (it != A.end()) {
        while (*it == 0)
            it++;
        int size = 0;
        if (*it == 1) {
            while (*it == 1) {
                size++;
                it++;
            }
            Asizes.emplace_back(size);
        }
    }
    it = B.begin();
    while (it != B.end()) {
        while (*it == 0)
            it++;
        int size = 0;
        if (*it == 1) {
            while (*it == 1) {
                size++;
                it++;
            }
            Bsizes.emplace_back(size);
        }
    }
    int n = Asizes.size(), m = Bsizes.size(), i = 0, j = 0;

    while (i + 1 < n && j + 1 < m) {
        if (Asizes[i] > Bsizes[j]) { // Dividir
            int Asize = Asizes[i];
            while (Asize > 0) {
                Matching.emplace_back(i + 1, j + 1);
                Asize -= Bsizes[j];
                j++;
            }
            i++;
        } else {
            int Bsize = Bsizes[j];
            while (Bsize > 0) {
                Matching.emplace_back(i + 1, j + 1);
                Bsize -= Asizes[i];
                i++;
            }
            j++;
        }
    }
    if (i + 1 < n) { // Agrupar en B_final
        while (i < n) {
            Matching.emplace_back(i + 1, j + 1);
            i++;
        }
    } else { // Dividir A_final
        while (j < m) {
            Matching.emplace_back(i + 1, j + 1);
            j++;
        }
    }
    int k = 0;
    *peso = 0;
    while (k < Matching.size()) {
        int numerator, denominator;
        if (Matching[k].first == Matching[k + 1].first) { // Division
            numerator = Asizes[Matching[k].first - 1];
            denominator = Bsizes[Matching[k].second - 1];
            while (Matching[k].first == Matching[k + 1].first) {
                denominator += Bsizes[Matching[k + 1].second - 1];
                k++;
            }
        } else { // Agrupacion
            numerator = Asizes[Matching[k].first - 1];
            denominator = Bsizes[Matching[k].second - 1];
            while (Matching[k].second == Matching[k + 1].second) {
                numerator += Asizes[Matching[k + 1].first - 1];
                k++;
            }
        }
        k++;
        *peso += (double) numerator / denominator;
    }

    return Matching;
}

int main() {
    std::vector<int> A = {0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0};
    std::vector<int> B = {0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0};
    double peso = 0;
    auto Matching  = minMatching(A, B, A.size(), &peso);
    for (auto pair : Matching) {
        std::cout << "(" << pair.first << "," << pair.second << ")\n";
    }
    std::cout << "Peso: " << peso << std::endl;
}
