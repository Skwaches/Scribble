#include "machine.h"
#include "read.h"

float sigmoid(float x){
	return 1/(1 + SDL_expf(-x));
}

float randomF(int fan_in){
	float range = SDL_sqrtf(1.0f / fan_in);
	bool sign = SDL_randf() >= 0.5f;
	return sign ? SDL_randf() * range : -SDL_randf() * range;
}

std::string weightsFile = "784-128-128.bin";
int inputSize = 784;
int node0Size = 128;
int node1Size = 128;
int outputSize = 10;

Matrix image(1, inputSize);
Matrix weights0(inputSize, node0Size);
Matrix weights1(node0Size, node1Size);
Matrix weights2(node1Size, outputSize);

Matrix bias0(1, node0Size);
Matrix bias1(1, node1Size);
Matrix bias2(1, outputSize);

Matrix node0(1,  node0Size );
Matrix node1(1,  node1Size );
Matrix output(1, outputSize);

Matrix d_weights0 (inputSize, node0Size);
Matrix d_weights1 (node0Size, node1Size);
Matrix d_weights2 (node1Size, outputSize);

Matrix d_bias0(1, node0Size);
Matrix d_bias1(1, node1Size);
Matrix d_bias2(1, outputSize);

auto trainImages = loadImages("../assets/Training/images.idx3-ubyte");
auto trainLabels = loadLabels("../assets/Training/labels.idx1-ubyte");
void initMachine(){
	// Sanity check
	std::cout << "Loaded " << trainImages.size() << " training images\n";
	std::cout << "First label: "  << trainLabels[0] << "\n";
	std::cout << "First pixel: "  << trainImages[0][0][0] << "\n"; // should be 0.0 - 1.0
																   
	weights0.replace([](){ return randomF(inputSize); });
	weights1.replace([](){ return randomF(node0Size); });
	weights2.replace([](){ return randomF(outputSize); });
}

Matrix forward(){
	node0 = image * weights0;
	node0 += bias0;
	node0.apply(sigmoid);

	node1 = node0 * weights1  ;
	node1 += bias1;
	node1.apply(sigmoid);
	
	output = node1 * weights2;
	output += bias2;
	output.apply(sigmoid);
	return output;
}

float cost(int correct){
	float variance = 0;
	for(int i = 0; i < outputSize; i++){
		float expected = (i == correct) ? 1 : 0;
		variance += SDL_pow(output[0][i] - expected , 2);
	}
	return variance;
}

void backward_Propagation(int correct){

	Matrix d_aj(1, node1.columns);
	for(int k = 0; k < output.columns;k++){
		float ak = output[0][k]; 
		float y = (k == correct) ? 1.0f : 0.0f;
		float common = (ak - y) * (ak * ( 1 - ak ));
		d_bias2[0][k] += common;

		for(int j = 0; j < node1.columns;j++){
			float aj = node1[0][j];
			d_weights2[j][k] += common * aj;
			d_aj[0][j] += common * weights2[j][k];
		}
	}

	Matrix d_ai(1, node0.columns);
	for(int j = 0; j < node1.columns;j++){
		float aj = node1[0][j];
		float common = (d_aj[0][j]) * (aj * ( 1 - aj ));  
		d_bias1[0][j] += common;
		for(int i = 0; i < node0.columns;i++){
			float ai = node0[0][i];
			d_weights1[i][j] += common * ai;
			d_ai[0][i] += common * weights1[i][j];
		}
	}

	for(int i = 0; i < node0.columns; i++){
		float ai = node0[0][i];
		float common = (d_ai[0][i]) * (ai * ( 1 - ai));
		d_bias0[0][i] += common;
		for(int n = 0; n < image.columns; n++){
			float an = image[0][n];
			d_weights0[n][i] += common * an;
		}
	}
}

void update_Parameters(float rate){
	auto propagate = [rate](float x){return rate*x;} ;
	weights0 -= d_weights0.modify(propagate); 
	weights1 -= d_weights1.modify(propagate);
	weights2 -= d_weights2.modify(propagate);

	bias0 -= d_bias0.modify(propagate); 
	bias1 -= d_bias1.modify(propagate);
	bias2 -= d_bias2.modify(propagate);
}

void resetGradients(){
	auto zero = [](){return 0;}; 
	d_weights0.replace(zero); 
	d_weights1.replace(zero); 
	d_weights2.replace(zero);

	d_bias0.replace(zero); 
	d_bias1.replace(zero); 
	d_bias2.replace(zero);

}

float train(int batchSize, float rate){
	float loss = 0;
	float scaled_rate = rate/batchSize;
	for( int i = 0; i < trainImages.size(); i++){
		if(!(i % batchSize))	
			resetGradients();
		image = trainImages[i];
		forward();
		loss += cost(trainLabels[i]);
		backward_Propagation(trainLabels[i]);

		if( ! ((i + 1) % batchSize) ){
			update_Parameters(rate);
			saveWeights(weightsFile);
		}
	}
	return loss/trainImages.size();
}

Matrix distribution_Data(Matrix input){
	image = input;
	forward();
	return output;
}

int classify(Matrix input, bool silent){
	image = input;
	forward();
	float sum = 0;
	if(!silent){
		//Convert output to percentage.
		output.apply([](float x){return SDL_expf(x);});
		output.pass([&sum](float x){ sum += x;});
		output.apply([sum](float x){return x/sum * 100;});
	}
	int chosen = 0;
	float value = 0;
	for(int i = 0; i < outputSize; i++){
		if(!silent)
			SDL_Log("%i:\t%f",i, output[0][i]);
		if(output[0][i] >= value){
			value = output[0][i];
			chosen = i;
		}
	}

	if(!silent)
		SDL_Log("Chose %i\n\n\n", chosen);
	return chosen;
}


void trainer(void){
	SDL_Log("Training has started!\nPlease wait");
		Uint32 before = SDL_GetTicks();
		float average_loss = train();
		Uint32 after = SDL_GetTicks();
		int time = (after - before)/1000;
		SDL_Log("Training complete :\n\tTime taken = %i secs \n\tLoss %f\n\n", time, average_loss);
}

auto testImages = loadImages("../assets/Testing/images.idx3-ubyte"); 
auto testLabels = loadLabels("../assets/Testing/labels.idx1-ubyte");
float test(bool testing, bool silent){
	auto& images = testing ? testImages:trainImages;
	auto& labels = testing ? testLabels:trainLabels;

	float success = 0;
	float totalLoss = 0;
	for(int i = 0; i < images.size(); i++){
		int chosen = classify(images[i], true);	
		if( chosen == labels[i] )
			success+=1;
		totalLoss += cost(labels[i]);
	}
	
	success/=images.size();
	success*=100;
	totalLoss/=images.size();
	if(!silent)
		SDL_Log("Testing complete:\n\tSuccess rate = %.2f%%\n\tMse Loss = %.7f",success,totalLoss);
	return success;
}
