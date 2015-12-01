#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <time.h>
#include <windows.h>

using std::fstream;
using std::string;
using std::to_string;

static const string TEST_NAME = "sekwencyjnie-IJK6"; // nazwa testu, identyfikator
static const int NSIZE = 600;		//rozmiar
static const int RSIZE = 150;

#pragma region Pozosta³e deklaracje
static const int ROWS = NSIZE;     // liczba wierszy macierzy
static const int COLUMNS = NSIZE;  // lizba kolumn macierzy

int NumThreads;
double start;
fstream fileStream;
float matrix_a[ROWS][COLUMNS];    // lewy operand 
float matrix_b[ROWS][COLUMNS];    // prawy operand
float matrix_r[ROWS][COLUMNS];    // wynik
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

	printf("%s Czas: %8.4f sec, \n", name.c_str(), elapsed - start);
	fileStream << name + ": " << elapsed - start << " sec (" << resolution << " sec rozdzielczosc pomiaru)\n";
}

#pragma endregion

#pragma region Mno¿enie macierzy
void multiply_matrices_IJK6() {
	int n = NSIZE;
	int r = RSIZE;
	for (int i = 0; i < n; i += r)
	for (int j = 0; j < n; j += r)
	for (int k = 0; k < n; k += r)
	for (int ii = i; ii < i + r; ii++)
	for (int jj = j; jj < j + r; jj++)
	for (int kk = k; kk < k + r; kk++)
		matrix_r[ii][jj] += matrix_a[ii][kk] * matrix_b[kk][jj];
}
#pragma endregion

int main(int argc, char* argv[]) {

	//Determine the number of threads to use
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

	initialize_matricesZ();
	start = (double)clock() / CLK_TCK;
	multiply_matrices_IJK6();
	print_elapsed_time("IJK6");

	fileStream.close();
	system("PAUSE");
	return(0);
}