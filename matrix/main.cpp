#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <time.h>
#include <windows.h>
#include <intrin.h>
#pragma intrinsic(__rdtsc)
unsigned long long a1, a2;

using std::fstream;
using std::string;
using std::to_string;

static const string TEST_NAME = "sekwencyjnie-IJK3"; // nazwa testu, identyfikator
static const int NSIZE = 1200;		//rozmiar
static const int RSIZE = 300;

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
	double delta = (a2 - a1) / 3.1;
	printf("%s Czas: %f ns, \n", name.c_str(), delta);
	fileStream << name + ": " << delta << " ns \n";
}
#pragma endregion

#pragma region Mno¿enie macierzy
void multiply_matrices_IJK() {
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLUMNS; j++)
			for (int k = 0; k < COLUMNS; k++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];

}

void multiply_matrices_IKJ() {
	for (int i = 0; i < ROWS; i++)
		for (int k = 0; k < COLUMNS; k++)
			for (int j = 0; j < COLUMNS; j++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];

}
void multiply_matrices_JIK() {
	for (int j = 0; j < COLUMNS; j++)
		for (int i = 0; i < ROWS; i++)
			for (int k = 0; k < COLUMNS; k++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];

}
void multiply_matrices_JKI() {
	for (int j = 0; j < COLUMNS; j++)
		for (int k = 0; k < COLUMNS; k++)
			for (int i = 0; i < ROWS; i++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];

}
void multiply_matrices_KIJ() {
	for (int k = 0; k < COLUMNS; k++)
		for (int i = 0; i < ROWS; i++)
			for (int j = 0; j < COLUMNS; j++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];

}
void multiply_matrices_KJI() {
	for (int k = 0; k < COLUMNS; k++)
		for (int j = 0; j < COLUMNS; j++)
			for (int i = 0; i < ROWS; i++)
				matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];

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

	//a1 = __rdtsc();
	//multiply_matrices_IJK();
	//a2 = __rdtsc();
	//print_elapsed_time("IJK3");
	a1 = __rdtsc();
	multiply_matrices_IKJ();
	a2 = __rdtsc();
	print_elapsed_time("IKJ3");

	//a1 = __rdtsc();
	//multiply_matrices_JIK();
	//a2 = __rdtsc();
	//print_elapsed_time("JIK3");
	//a1 = __rdtsc();
	//multiply_matrices_JKI();
	//a2 = __rdtsc();
	//print_elapsed_time("JKI3");

	//a1 = __rdtsc();
	//multiply_matrices_KIJ();
	//a2 = __rdtsc();
	//print_elapsed_time("KIJ3");
	//a1 = __rdtsc();
	//multiply_matrices_KJI();
	//a2 = __rdtsc();
	//print_elapsed_time("KJI3");



	fileStream.close();
	return(0);
}