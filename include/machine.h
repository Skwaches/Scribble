#pragma once
#include <SDL3/SDL.h>
#include "matrix.h"

void initMachine(void);
Matrix forward(void);
int classify(Matrix input, bool silent = false);
Matrix distribution_Data(Matrix input);
float cost(int correct);
float train(int batchSize = 32, float rate = 0.1f);
void trainer(void);
float test(bool testing = true, bool silent = false);

extern std::string weightsFile;

extern int inputSize ;
extern int node0Size ;
extern int node1Size ;
extern int outputSize;

extern Matrix image;
extern Matrix weights0;
extern Matrix weights1;
extern Matrix weights2;

extern Matrix bias0;
extern Matrix bias1;
extern Matrix bias2;

extern Matrix node0;
extern Matrix node1;
extern Matrix output;

extern Matrix d_weights0;
extern Matrix d_weights1;
extern Matrix d_weights2;

extern Matrix d_bias0;
extern Matrix d_bias1;
extern Matrix d_bias2;
