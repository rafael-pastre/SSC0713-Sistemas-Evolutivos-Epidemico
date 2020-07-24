#include "series.h"

// Auxiliaries math functions
int min_int(int a, int b){ return (a > b)?(b):(a); }
double min_double(double a, double b){ return (a > b)?(b):(a); }

// Euclidian Distance
double dist_euclidian(double* s1, double* s2, unsigned int series_size){
	double dist = 0;
	
	for(int i = 0; i < series_size; i++){
		dist += pow(s1[i] - s2[i], 2);
	}
	
	return sqrt(dist);
}

// DWT Distance
double dist_DWT(double* s1, double* s2, unsigned int s1_size, unsigned int s2_size){
	double DWT[s1_size][s2_size];
    int i, j;

    DWT[0][0] = fabs(s1[0] - s2[0]);

    //1a coluna da matriz
    for(i = 1; i < s1_size; i++)
        DWT[i][0] = fabs(s1[i] - s2[0]); //+ DWT[i-1][0];

    //1a linha da matriz
    for(i = 1; i < s2_size; i++)
        DWT[0][i] = fabs(s1[0] - s2[i]); //+ DWT[0][i-1];

    //restante da matriz
    for(i = 1; i < min_int(s1_size, s2_size); i++){
        //i-esima coluna
        for(j = i; j < s1_size; j++)
            DWT[j][i] = fabs(s1[j] - s2[i]) + min_double(DWT[j-1][i], min_double(DWT[j][i-1], DWT[j-1][i-1]));
        //i-esima linha
        for(j = i + 1; j < s2_size; j++)
            DWT[i][j] = fabs(s1[i] - s2[j]) + min_double(DWT[i-1][j], min_double(DWT[i][j-1], DWT[i-1][j-1]));
    }
/*
    for(i = s1_size - 1; i >= 0; i--){
        for(j = 0; j < s2_size; j++)
            printf("%lf ", DWT[i][j]);
        printf("\n");
    }
*/
    return DWT[s1_size-1][s2_size-1];
}