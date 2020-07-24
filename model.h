/*
 *	Model:
 *	M = Maternal imune
 *	S = Suceptible
 *	E = Exposed to virus in latency period
 *	I = Symptomatic Infectious
 *	A = Assymptomatic
 *	R = Recovered with imunity
 *	D = Dead
 */

#include <stdlib.h>

typedef struct {
	// Time series
	double*	S;		// Suceptible group
	double*	I;		// Infected group
	double*	R;		// Recovered group
	double*	D;		// Dead group

	double	N;		// Total population
	unsigned int max_days;
	
	// Parameters
	double beta;	// Infection rate
	double gamma;	// Recovery rate
	double epsilon;	// Death rate
}Model;

Model* allocate_model(unsigned int max_days);

void set_initial_conditions(Model* model, double S, double I, double R, double D);

void set_parameters(Model* model, double beta, double gamma, double epsilon);

void predict(Model* model, unsigned int predicted_days);