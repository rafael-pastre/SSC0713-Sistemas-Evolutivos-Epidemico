#include "model.h"

Model* allocate_model(unsigned int max_days){
	Model* model;
	
	// Model allocation
	model = (Model*)malloc(sizeof(Model));
	if(model == NULL) return NULL;
	
	// Parameter reseting
	model->beta = 0.0;
	model->gamma = 0.0;
	model->epsilon = 0.0;
	
	// Time series allocation
	model->S = (double*)malloc(sizeof(double) * max_days);
	model->I = (double*)malloc(sizeof(double) * max_days);
	model->R = (double*)malloc(sizeof(double) * max_days);
	model->D = (double*)malloc(sizeof(double) * max_days);
	//model->N = (double*)malloc(sizeof(double) * max_days);
	
	if(model->S == NULL || model->I == NULL || model->R == NULL || model->D == NULL) return NULL;
	
	model->max_days = max_days;
	
	return model;
}

void set_initial_conditions(Model* model, double S, double I, double R, double D){
	// Valid model checking
	if(model == NULL) return;
	
	// Attributions
	model->S[0] = S;
	model->I[0] = I;
	model->R[0] = R;
	model->D[0] = D;
	model->N = S + I + R + D;
}

void set_parameters(Model* model, double beta, double gamma, double epsilon){
	// Valid model checking
	if(model == NULL) return;
	
	// Attributions
	model->beta = beta;
	model->gamma = gamma;
	model->epsilon = epsilon;
}

void predict(Model* model, unsigned int predicted_days){
	// Valid model checking
	if(model == NULL) return;
	if(model->max_days < predicted_days) return;
	
	// Iterations
	for(int i = 0; i < predicted_days-1; i++){
		//model->N[i] = S[i] + I[i] + R[i] + D[i];
		model->S[i+1] = model->S[i] - model->beta * model->S[i] * model->I[i] / model->N;
		model->I[i+1] = model->I[i] + model->beta * model->S[i] * model->I[i] / model->N - model->gamma * model->I[i] - model->epsilon * model->I[i];
		model->R[i+1] = model->R[i] + model->gamma * model->I[i];
		model->D[i+1] = model->D[i] + model->epsilon * model->I[i];
	}
}