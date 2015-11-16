
//no elo
// Celem tego programu jest prezentacja pomiaru i analizy 
//efektywnosci programu za pomoc¹  CodeAnalyst(tm).
// Implementacja mno¿enia macierzy jest realizowana za pomoca typowego 
// algorytmu podrêcznikowego. 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <time.h>
#include <windows.h>
#include <omp.h>
#include <chrono>

#define USE_MULTIPLE_THREADS true
#define MAXTHREADS 128

using std::fstream;
using std::chrono::system_clock;
int NumThreads;
double start;

static const int ROWS = 1000;     // liczba wierszy macierzy
static const int COLUMNS = 1000;  // lizba kolumn macierzy

float matrix_a[ROWS][COLUMNS];    // lewy operand 
float matrix_b[ROWS][COLUMNS];    // prawy operand
float matrix_r[ROWS][COLUMNS];    // wynik

fstream fileStream;

void initialize_matrices() {
	// zdefiniowanie zawarosci poczatkowej macierzy
	//#pragma omp parallel for 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			matrix_a[i][j] = (float)rand() / RAND_MAX;
			matrix_b[i][j] = (float)rand() / RAND_MAX;
			matrix_r[i][j] = 0.0;
		}
	}
}

void initialize_matricesZ() {
	// zdefiniowanie zawarosci poczatkowej macierzy
#pragma omp parallel for 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			matrix_r[i][j] = 0.0;
		}
	}
}

void multiply_matrices_IJK() {
	// mnozenie macierzy 
#pragma omp parallel for 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			float sum = 0.0;
			for (int k = 0; k < COLUMNS; k++) {
				sum = sum + matrix_a[i][k] * matrix_b[k][j];
			}
			matrix_r[i][j] = sum;
		}
	}
}

void print_elapsed_time() {
	double elapsed;
	double resolution;

	// wyznaczenie i zapisanie czasu przetwarzania
	elapsed = (double)clock() / CLK_TCK;
	resolution = 1.0 / CLK_TCK;
	printf("Czas: %8.4f sec \n",
		elapsed - start);

	fileStream << "Czas wykonania programu: " << elapsed - start << " sec (" << resolution << " sec rozdzielczosc pomiaru)\n";
}

int main(int argc, char* argv[]) {
	//	 start = (double) clock() / CLK_TCK ;
	std::time_t end_time = system_clock::to_time_t(system_clock::now());
	std::cout << std::ctime(&end_time) << "\n";
	fileStream.open("results.txt", std::ios::in | std::ios::out | std::ios::trunc);

	if (!fileStream.good()) {
		fprintf(stderr, "nie mozna otworzyc pliku wyniku \n");
		return(EXIT_FAILURE);
	}


	//Determine the number of threads to use
	if (USE_MULTIPLE_THREADS) {
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		NumThreads = SysInfo.dwNumberOfProcessors;
		if (NumThreads > MAXTHREADS)
			NumThreads = MAXTHREADS;
	} else
		NumThreads = 1;

	fileStream << "Klasyczny algorytm mnozenia macierzy, liczba watkow" << NumThreads << "\n";
	printf("liczba watkow  = %d\n\n", NumThreads);

	initialize_matrices();
	start = (double)clock() / CLK_TCK;
	multiply_matrices_IJK();
	printf("IJK ");
	print_elapsed_time();
	initialize_matricesZ();


	fileStream.close();
	//fclose(result_file);

	return(0);
}
