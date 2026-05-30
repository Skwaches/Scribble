#pragma once
#include <SDL3/SDL.h>
#include "matrix.h"

void initMachine(void);
Matrix forward(void);
void classify(Matrix input);
float cost();
float train(int batchSize = 32, float rate = 0.1f);
void saveWeights(const std::string& path);
void loadWeights(const std::string& path);
void trainer(void);
extern std::string weightsFile;
