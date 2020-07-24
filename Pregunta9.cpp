#include <iostream>
#include "ImageTransformation.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libraries/stb_image_write.h"

bool transformImages(const char* imageA, const char* imageB, int method);
void getTransition(const char* imageA, const char* imageB);

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
    getTransition(imageA, imageB);
    return true;
}

void getTransition(const char* imageA, const char* imageB) {
    int width, height, bpp;
    unsigned char* A = stbi_load(imageA, &width, &height, &bpp, 3);
    unsigned char* B = stbi_load(imageB, &width, &height, &bpp, 3);

    int arrSize = width * height * 3;
    unsigned char transition[arrSize];
    for (int i = 0; i < arrSize; ++i)
        transition[i] = (A[i] + B[i])/2;

    stbi_write_jpg("images\\transition.jpg", width, height, 3, transition, 100);
}
