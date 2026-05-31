#pragma once
#include "matrix.h"
int reverseInt(int i);
std::vector<Matrix> loadImages(const std::string& path);
std::vector<int> loadLabels(const std::string& path);
void saveWeights(const std::string& path);
void loadWeights(const std::string& path);
