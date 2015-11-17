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
using std::string;

static const string TEST_NAME = "rownolegle"; // nazwa testu, identyfikator
static const int ISIZE = 700;		//rozmiar

#pragma region Pozosta³e deklaracje
static const int ROWS = ISIZE;     // liczba wierszy macierzy
static const int COLUMNS = ISIZE;  // lizba kolumn macierzy

int NumThreads;
double start;
fstream fileStream;
float matrix_a[ROWS][COLUMNS];    // lewy operand 
float matrix_b[ROWS][COLUMNS];    // prawy operand
float matrix_r[ROWS][COLUMNS];    // wynik
float matrix_r_sequence[ISIZE][ISIZE]; //wynik kontrolny
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
	double elapsed;
	double resolution;

	// wyznaczenie i zapisanie czasu przetwarzania
	elapsed = (double)clock() / CLK_TCK;
	resolution = 1.0 / CLK_TCK;

	bool isOk = true;
	for (int i = 0; i < ISIZE; i++)
		for (int k = 0; k < ISIZE; k++)
			if (matrix_r[i][k] != matrix_r_sequence[i][k]) {
				isOk = false;
				break;
			}
				
	string ok = isOk ? "tak" : "nie";
	printf("%s Poprawnosc: %s Czas: %8.4f sec, \n", name.c_str(), ok.c_str(), elapsed - start );
	fileStream << "Poprawnoœæ: " + ok + " "<< name + ": " << elapsed - start << " sec (" << resolution << " sec rozdzielczosc pomiaru)\n";
}
#pragma endregion

#pragma region Mno¿enie macierzy
void multiply_matrices_JKI_sequence() {
#pragma omp parallel for 
	for (int j = 0; j < COLUMNS; j++)
		for (int k = 0; k < COLUMNS; k++)
			for (int i = 0; i < ROWS; i++)
				matrix_r_sequence[i][j] += matrix_a[i][k] * matrix_b[k][j];
}
void multiply_matrices_IJK_sequence() {
#pragma omp parallel for 
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			float sum = 0.0;
			for (int k = 0; k < COLUMNS; k++) {
				sum = sum + matrix_a[i][k] * matrix_b[k][j];
			}
			matrix_r_sequence[i][j] = sum;
		}
	}
}

void multiply_matrices_JKI() {
	#pragma omp parallel for 
	for (int j = 0; j < COLUMNS; j++)
		for (int k = 0; k < COLUMNS; k++)
			for (int i = 0; i < ROWS; i++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];
}
void multiply_matrices_IJK() {
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

	std::ostringstream size, numThread;
	size << ISIZE;
	numThread << NumThreads;
	string resultFileName = "wynik_" + TEST_NAME + "_" + size.str() + "_" + numThread.str() + ".txt";

	fileStream.open(resultFileName, std::ios::out | std::ios::trunc);
	if (!fileStream.good()) {
		fprintf(stderr, "Nie mozna otworzyc pliku wyniku %s\n", resultFileName);
	}


	fileStream << "File: " << resultFileName << "\n";
	printf("%s\n\n", resultFileName);

	initialize_matrices();
	multiply_matrices_JKI_sequence();
	start = (double)clock() / CLK_TCK;
	multiply_matrices_JKI();
	print_elapsed_time("JKI3");

	initialize_matricesZ();
	start = (double)clock() / CLK_TCK;
	multiply_matrices_IJK();
	print_elapsed_time("IJK6");

	fileStream.close();
	system("PAUSE");
	return(0);
}