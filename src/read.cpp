#include "read.h"
#include <stdexcept>
#include <fstream>
#include "machine.h"
#include <SDL3_ttf/SDL_ttf.h>

// IDX files are big-endian, most PCs are little-endian — must reverse bytes
int reverseInt(int i) {
    unsigned char c1, c2, c3, c4;
    c1 = i         & 255;
    c2 = (i >> 8)  & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
    return ((int)c1 << 24) | ((int)c2 << 16) | ((int)c3 << 8) | c4;
}

// Each image is returned as a (1x784) Matrix, normalized to [0,1]
std::vector<Matrix> loadImages(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open: " + path);

    int magic, numImages, rows, cols;
    file.read((char*)&magic,     4); magic     = reverseInt(magic);
    file.read((char*)&numImages, 4); numImages = reverseInt(numImages);
    file.read((char*)&rows,      4); rows      = reverseInt(rows);
    file.read((char*)&cols,      4); cols      = reverseInt(cols);

    if (magic != 2051)
        throw std::runtime_error("Invalid image file magic number");

    std::vector<Matrix> images;
    images.reserve(numImages);

    for (int i = 0; i < numImages; i++) {
        Matrix img(1, rows * cols);
        for (int p = 0; p < rows * cols; p++) {
            unsigned char pixel;
            file.read((char*)&pixel, 1);
            img[0][p] = pixel / 255.0f;  // normalize to [0, 1]
        }
        images.push_back(img);
    }
    return images;
}

std::vector<int> loadLabels(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open: " + path);

    int magic, numLabels;
    file.read((char*)&magic,     4); magic     = reverseInt(magic);
    file.read((char*)&numLabels, 4); numLabels = reverseInt(numLabels);

    if (magic != 2049)
        throw std::runtime_error("Invalid label file magic number");

    std::vector<int> labels(numLabels);
    for (int i = 0; i < numLabels; i++) {
        unsigned char label;
		file.read((char*)&label, 1);
        labels[i] = (int)label;
    }
    return labels;
}

void saveParameters(const std::string& path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for saving: " + path);

    auto writeMatrix = [&](Matrix& m) {
        for (int i = 0; i < m.rows; i++)
            for (int j = 0; j < m.columns; j++)
                file.write((char*)&m[i][j], sizeof(float));
    };

    writeMatrix(weights0); writeMatrix(bias0);
    writeMatrix(weights1); writeMatrix(bias1);
    writeMatrix(weights2); writeMatrix(bias2);
}


void loadWeights(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for loading: " + path);

    auto readMatrix = [&](Matrix& m) {
        for (int i = 0; i < m.rows; i++)
            for (int j = 0; j < m.columns; j++)
                file.read((char*)&m[i][j], sizeof(float));
    };

    readMatrix(weights0); readMatrix(bias0);
    readMatrix(weights1); readMatrix(bias1);
    readMatrix(weights2); readMatrix(bias2);
}
