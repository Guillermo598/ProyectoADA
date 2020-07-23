#include <iostream>
#include "ImageTransformation.h"

bool transformImages(const char* imageA, const char* imageB, int method);

int main() {
    if (!transformImages("images\\A.png", "images\\B.png", 2))
        std::cout << "Matching Error\n";
}

bool transformImages(const char* imageA, const char* imageB, int method) {
    auto matrixA = getStringsFromPNG(imageA, 127, 0.2126, 0.7152, 0.0722);
    auto matrixB = getStringsFromPNG(imageB, 127, 0.2126, 0.7152, 0.0722);
    int q = matrixA.size();
    std::vector<std::vector<std::pair<int,int>>> transform(q);
    double peso;
    if (method == 0)
        peso = GreedyMinTransform(matrixA, matrixB, &transform);
    else if (method == 1)
        peso = DPMinTransform(matrixA, matrixB, &transform);
    else if (method == 2)
        peso = DPMinTransformMean(matrixA, matrixB, &transform);
    else
        return false;

    for (const auto& matching : transform) {
        for (auto pair : matching)
            std::cout << "(" << pair.first << "," << pair.second << ")";
        std::cout << "\n";
    }
    std::cout << "Peso: " << peso << std::endl;

    return true;
}