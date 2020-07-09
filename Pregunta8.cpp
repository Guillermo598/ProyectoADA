#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image.h"

std::vector<std::vector<int>> getMatrixFromPNG(const char* filename, int umbral, double Rc, double Gc, double Bc);

int main() {
    auto matrix = getMatrixFromPNG("images\\A.png", 127, 0.2126, 0.7152, 0.0722);

    for (auto & row : matrix) {
        for (int column : row)
            std::cout << column;
        std::cout << "\n";
    }

    return 0;
}

std::vector<std::vector<int>> getMatrixFromPNG(const char* filename, int umbral, double Rc, double Gc, double Bc) {
    int width, height, bpp;
    unsigned char* image = stbi_load(filename, &width, &height, &bpp, 3);
    std::vector<std::vector<int>> matrix(height);
    for (int i = 0; i < height; ++i) {
        std::vector<int> vec(width);
        for (int j = 0; j < width; ++j) {
            unsigned char* pixelOffset = image + (j + width * i) * 3;
            double Y = Rc*pixelOffset[0] + Gc*pixelOffset[1] + Bc*pixelOffset[2];
            if (Y > umbral)
                vec[j] = 0;
            else
                vec[j] = 1;
        }
        matrix[i] = vec;
    }
    stbi_image_free(image);
    return matrix;
}
