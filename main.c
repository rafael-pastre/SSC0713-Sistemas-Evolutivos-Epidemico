#include <stdio.h>
#include <conio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "model.h"
#include "series.h"

//#define REAL_DATA

#ifdef	REAL_DATA
#define POP_SIZE		209500000
#else
#define POP_SIZE		10000
#endif

#define SERIES_LENGHT	146
#define NUM_MODELS		5
#define TAX_MUT 		0.25

	
// Reference data (used to calculate fitness)
double S[SERIES_LENGHT];	// Suceptible population
double T[SERIES_LENGHT];	// Total cases
double D[SERIES_LENGHT];	// Total deaths

// Genetical Functions
double fitness (Model* model);
int elitismo(Model* model_pop[NUM_MODELS], double fit[NUM_MODELS]);
int predacao(Model* model_pop[NUM_MODELS], double fit[NUM_MODELS]);

int main(){
	// Genetical Entities
	Model* model_pop[NUM_MODELS];
	double fit[NUM_MODELS];
	int the_best = 0;
	int geracao = 0;

	// Data I/O
	FILE* data_ref;
	FILE* data_out;
	FILE* plot_out;
	
	// Other
	char ch = 's';
	
	srand(time(NULL));
	
	
	// Read input data
	#ifdef REAL_DATA
	double dummy1, dummy2;
	data_ref = fopen("./data/brasil-data.csv", "rt");
	
	for(int i = 0; i < SERIES_LENGHT; i++){
		fscanf(data_ref, "%lf,%lf,%lf,%lf", &dummy1, T+i ,&dummy2, D+i);
		S[i] = POP_SIZE - T[i] - D[i];
	}
	
	fclose(data_ref);
	#else
	data_ref = fopen("./data/data.csv", "rt");
	
	for(int i = 0; i < SERIES_LENGHT; i++){
		fscanf(data_ref, "%lf,%lf", T+i, D+i);
		S[i] = POP_SIZE - T[i] - D[i];
	}
	
	fclose(data_ref);
	#endif
	
	for(int i = 0; i < SERIES_LENGHT; i++){
		printf("%lf \t %lf \t %lf \n", T[i], D[i], S[i]);
	}
	
	// Create population of Models
	for(int i = 0; i < NUM_MODELS; i++){
		model_pop[i] = allocate_model(SERIES_LENGHT);
		
		set_initial_conditions(model_pop[i], POP_SIZE, 1, 0, 0);
	
		do{
			set_parameters(model_pop[i], ((double)rand()/(double)rand()), ((double)rand()/(double)rand()), ((double)rand()/(double)rand()));
			predict(model_pop[i], SERIES_LENGHT);
		}while(isnan(fitness(model_pop[i])));
	}
	
	// Preprare gnuplot script to plot the best
	plot_out = fopen("./scripts/plot_script.gp", "wt");
	fprintf(plot_out, "set term png\n");
	fprintf(plot_out, "set output \"./img/plot.png\"\n");
	fprintf(plot_out, "set title 'Melhor Modelo - AG'\n");
	fprintf(plot_out, "set xrange [0:%d]\n", SERIES_LENGHT);
	fprintf(plot_out, "set yrange [0:%d]\n", POP_SIZE);
	fprintf(plot_out, "plot './data/output.csv' using 1:2 t'S', './data/output.csv' using 1:3 t'I', './data/output.csv' using 1:4 t'R', './data/output.csv' using 1:5 t'D'\n");
	fclose(plot_out);
	
	// Evolve Models
	do{

		// Calculate fintess for all models
		for(int i = 0; i < NUM_MODELS; i++){
			predict(model_pop[i], SERIES_LENGHT);
			fit[i] = fitness(model_pop[i]);
		}

		// Genetical functions
		the_best = elitismo(model_pop, fit);

		predacao(model_pop, fit);
		
		// Print best and asks for new generation
		geracao++;
		
		if(kbhit() && getch() == 'q' || geracao % 10 == 0) {
			// Save best in csv file
			data_out = fopen("./data/output.csv", "wt");
			for(int i = 0; i < SERIES_LENGHT; i++)
				fprintf(data_out, "%d, %lf, %lf, %lf, %lf\n", i, model_pop[the_best]->S[i], model_pop[the_best]->I[i], model_pop[the_best]->R[i], model_pop[the_best]->D[i]);
			fclose(data_out);
			
			// Print csv with the best data using gnuplot
			system("gnuplot ./scripts/plot_script.gp");
			
			// Print the best data on console
			printf("The best(%d): %lf\n", the_best, fit[the_best]);
			printf("Parameters: beta = %lf, gamma = %lf, epsilon = %lf\n", model_pop[the_best]->beta, model_pop[the_best]->gamma, model_pop[the_best]->epsilon);
		    printf("\n\nContinuar evolucao (s/n)?");
	    	fflush(stdin);
			ch = getchar();
		};
	} while (ch != 'n' && ch != 'N');
	
	
	return 0;
}


/*
 *	Genetical Functions
 */
double fitness (Model* model) {
	double Taux[SERIES_LENGHT];		// Auxiliar series to sum I an R series from model. Thats necessary, because real data commonly comes in form of a "Total Cases" series
	
	for(int i = 0; i < SERIES_LENGHT; i++)
		Taux[i] = model->I[i] + model->R[i];
		
	//return	dist_euclidian(T, Taux, SERIES_LENGHT) +
	//		dist_euclidian(D, model->D, SERIES_LENGHT) +
	//		1*dist_DWT(T, Taux, SERIES_LENGHT, SERIES_LENGHT) +
	//		1*dist_DWT(D, model->D, SERIES_LENGHT, SERIES_LENGHT);
	
	return dist_euclidian(T, Taux, SERIES_LENGHT) + dist_euclidian(D, model->D, SERIES_LENGHT);
	//return dist_DWT(T, Taux, SERIES_LENGHT, SERIES_LENGHT) + dist_DWT(D, model->D, SERIES_LENGHT, SERIES_LENGHT);
}

int elitismo(Model* model_pop[NUM_MODELS], double fit[NUM_MODELS]) {
	int the_best = 0;
	double best_fit = DBL_MAX;
	double f;

	// Encontra Melhor
	for(int i = 0; i < NUM_MODELS; i++){
		f = fitness(model_pop[i]);
		if(f < best_fit){
			the_best = i;
			best_fit = f;
		}
	}

	// Cruza e muta os individuos
    for (int i = 0; i < NUM_MODELS; i++) {	
        if (i == the_best) continue;		// Protege o melhor individuo

		// Crossover
		model_pop[i]->beta = (model_pop[i]->beta + model_pop[the_best]->beta) / 2.0;
		model_pop[i]->gamma = (model_pop[i]->gamma + model_pop[the_best]->gamma) / 2.0;
		model_pop[i]->epsilon = (model_pop[i]->epsilon + model_pop[the_best]->epsilon) / 2.0;

		// Mutação
		model_pop[i]->beta += model_pop[i]->beta*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop[i]->beta -= model_pop[i]->beta*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop[i]->gamma += model_pop[i]->gamma*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop[i]->gamma -= model_pop[i]->gamma*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop[i]->epsilon += model_pop[i]->epsilon*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop[i]->epsilon -= model_pop[i]->epsilon*((double)rand()/RAND_MAX) * TAX_MUT;
		
    }	
	return the_best;
}

void torneio2(Model* model_pop[NUM_MODELS], double fit[NUM_MODELS]) {
	Model* model_pop_aux[NUM_MODELS];
    int a, b, pai1, pai2;
	int the_best = 0;
	double best_fit = DBL_MAX;
	double f;

	// Encontra Melhor
	for(int i = 0; i < NUM_MODELS; i++){
		f = fitness(model_pop[i]);
		if(f < best_fit){
			the_best = i;
			best_fit = f;
		}
	}
	
	// Allocation of Auxiliar Models
	for(int i = 0; i < NUM_MODELS; i++){
		model_pop_aux[i] = allocate_model(SERIES_LENGHT);
		set_initial_conditions(model_pop_aux[i], POP_SIZE, 1, 0, 0);
	}
	
	// Torneio
    for (int i = 0; i <= NUM_MODELS; i++) {
        
		//Pula The Best
		if (i == the_best) {
             continue;
        }

        // Sorteia dois individuos para 1º torneio
        a = (rand() % NUM_MODELS);
        b = (rand() % NUM_MODELS);
        
		if (fit[a] > fit[b]) pai1 = a;
        else pai1 = b;

        // Sorteia mais dois individuos para 2º torneio
        a = (rand() % NUM_MODELS);
        b = (rand() % NUM_MODELS);
        
		if (fit[a] > fit[b]) pai2 = a;
        else pai2 = b;

        // Crossover
		model_pop_aux[i]->beta = (model_pop[pai1]->beta + model_pop[pai2]->beta) / 2.0;
		model_pop_aux[i]->gamma = (model_pop[pai1]->gamma + model_pop[pai2]->gamma) / 2.0;
		model_pop_aux[i]->epsilon = (model_pop[pai1]->epsilon + model_pop[pai2]->epsilon) / 2.0;

        // Mutação
		model_pop_aux[i]->beta += model_pop_aux[i]->beta*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop_aux[i]->beta -= model_pop_aux[i]->beta*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop_aux[i]->gamma += model_pop_aux[i]->gamma*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop_aux[i]->gamma -= model_pop_aux[i]->gamma*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop_aux[i]->epsilon += model_pop_aux[i]->epsilon*((double)rand()/RAND_MAX) * TAX_MUT;
		model_pop_aux[i]->epsilon -= model_pop_aux[i]->epsilon*((double)rand()/RAND_MAX) * TAX_MUT;
    }
    
	//Copia o População auxiliar para população principal
    for (int i = 0; i <= NUM_MODELS; i++){
		//Pula The Best
		if (i == the_best) {
             continue;
        }
		
		model_pop[i]->beta = model_pop_aux[i]->beta;
		model_pop[i]->gamma = model_pop_aux[i]->gamma;
		model_pop[i]->epsilon = model_pop_aux[i]->epsilon;
	}
}


int predacao(Model* model_pop[NUM_MODELS], double fit[NUM_MODELS]) {
	int the_worst = 0;
	double worst_fit = 0;
	double f;
    
    //Busca pior a ser predado

	for(int i = 0; i < NUM_MODELS; i++){
		f = fitness(model_pop[i]);
		if(f > worst_fit){
			the_worst = i;
			worst_fit = f;
		}
	}
    
    // Mata o Pior e troca por um Randômico
    do{
		set_parameters(model_pop[the_worst], ((double)rand()/(double)rand()), ((double)rand()/(double)rand()), ((double)rand()/(double)rand()));
		predict(model_pop[the_worst], SERIES_LENGHT);
	}while(isnan(fitness(model_pop[the_worst])));
	
	return the_worst;
}