#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <time.h>
#include <windows.h>
#include <omp.h>
#include <chrono>
#include <intrin.h>
#pragma intrinsic(__rdtsc)
unsigned long long a1, a2;

#define USE_MULTIPLE_THREADS true
#define MAXTHREADS 128

using std::fstream;
using std::string;
using std::to_string;

static const string TEST_NAME = "powinowactwo-dynamiczne-JKI3"; // nazwa testu, identyfikator
static const int NSIZE = 1200;		//rozmiar
static const int RSIZE = 300;

#pragma region Pozosta�e deklaracje
static const int ROWS = NSIZE;     // liczba wierszy macierzy
static const int COLUMNS = NSIZE;  // lizba kolumn macierzy

int NumThreads;
double start;
fstream fileStream;
float matrix_a[ROWS][COLUMNS];    // lewy operand 
float matrix_b[ROWS][COLUMNS];    // prawy operand
float matrix_r[ROWS][COLUMNS];    // wynik
float matrix_r_sequence[NSIZE][NSIZE]; //wynik kontrolny
#pragma endregion

#pragma region Zapis i inicjowanie macierzy
void initialize_matrices() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			matrix_a[i][j] = (float)rand() / RAND_MAX;
			matrix_b[i][j] = (float)rand() / RAND_MAX;
			matrix_r[i][j] = 0.0;
		}
	}
}
void initialize_matricesZ() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			matrix_r[i][j] = 0.0;
		}
	}
}

void print_elapsed_time(string name) {
	double delta = (a2 - a1) / 3.1;
	printf("%s Czas: %f ns, \n", name.c_str(), delta);
	fileStream << name + ": " << delta << " ns \n";
}
#pragma endregion

void setup_thread_affinity() {
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	int th_id = omp_get_thread_num();
	DWORD_PTR mask = (1 << (th_id %  SysInfo.dwNumberOfProcessors));
	SetThreadAffinityMask(GetCurrentThread(), mask);
}

#pragma region Mno�enie macierzy
void multiply_matrices_JKI() {
	#pragma omp parallel
	{
		setup_thread_affinity();
		#pragma omp for schedule(dynamic)
		for (int j = 0; j < COLUMNS; j++)
			for (int k = 0; k < COLUMNS; k++)
				for (int i = 0; i < ROWS; i++)
					matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];
	}
}
#pragma endregion


int main(int argc, char* argv[]) {
	//Determine the number of threads to use
	if (USE_MULTIPLE_THREADS) {
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		NumThreads = SysInfo.dwNumberOfProcessors;
		if (NumThreads > MAXTHREADS)
			NumThreads = MAXTHREADS;
	} else
		NumThreads = 1;

	string resultFileName =
		"wynik_" + TEST_NAME + "_" + to_string(NSIZE) + "_"
		+ to_string(RSIZE) + "_" + to_string(NumThreads) + ".txt";

	fileStream.open(resultFileName, std::ios::out | std::ios::trunc);
	if (!fileStream.good()) {
		fprintf(stderr, "Nie mozna otworzyc pliku wyniku %s\n", resultFileName);
	}


	fileStream << "File: " << resultFileName << "\n";
	printf("%s\n\n", resultFileName);

	initialize_matrices();
	a1 = __rdtsc();
	multiply_matrices_JKI();
	a2 = __rdtsc();
	print_elapsed_time("JKI3");

	fileStream.close();
	
	return(0);
}