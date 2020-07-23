#ifndef PROYECTOADA_IMAGETRANSFORMATION_H
#define PROYECTOADA_IMAGETRANSFORMATION_H

#include <vector>
#define INF 100000000
#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image.h"

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


// Greedy

double GreedyMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* matching) {
    std::vector<int> blocksA = getBlocks(std::move(A));
    std::vector<int> blocksB = getBlocks(std::move(B));
    int m = blocksA.size();
    int n = blocksB.size();
    if (n == 0 || m == 0)
        return 0;
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


// DP

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

double DPMinMatching(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* Matching) {
    std::vector<int> blocksA = getBlocks(std::move(A));
    std::vector<int> blocksB = getBlocks(std::move(B));
    int m = blocksA.size();
    int n = blocksB.size();
    if (n == 0 || m == 0)
        return 0;
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

double DPMinTransform(std::vector<std::string> matrixA, std::vector<std::string> matrixB,
                      std::vector<std::vector<std::pair<int,int>>>* transform) {
    int q = matrixA.size();
    double peso = 0;
    for (int i = 0; i < q; ++i) {
        std::vector<int> A = getVectorFromString(matrixA[i]);
        std::vector<int> B = getVectorFromString(matrixB[i]);
        std::vector<std::pair<int,int>> matching;
        peso += DPMinMatching(A, B, &matching);
        (*transform)[i] = matching;
    }
    return peso;
}


// DP Mean

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


double DPMinMatchingMean(std::vector<int> A, std::vector<int> B, std::vector<std::pair<int,int>>* Matching) {
    std::vector<int> blocksA = getBlocks(std::move(A));
    std::vector<int> blocksB = getBlocks(std::move(B));
    int m = blocksA.size();
    int n = blocksB.size();
    if (n == 0 || m == 0)
        return 0;
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



// Image

std::vector<std::string> getStringsFromPNG(const char* filename, int umbral, double Rc, double Gc, double Bc) {
    int width, height, bpp;
    unsigned char* image = stbi_load(filename, &width, &height, &bpp, 3);

    std::vector<std::string> matrix(height);
    for (int i = 0; i < height; ++i) {
        std::string vec;
        vec.resize(width);
        for (int j = 0; j < width; ++j) {
            unsigned char* pixelOffset = image + (j + width * i) * 3;
            double Y = Rc*pixelOffset[0] + Gc*pixelOffset[1] + Bc*pixelOffset[2];
            if (Y > umbral)
                vec[j] = '0';
            else
                vec[j] = '1';
        }
        matrix[i] = vec;
    }
    stbi_image_free(image);
    return matrix;
}






#endif //PROYECTOADA_IMAGETRANSFORMATION_H
