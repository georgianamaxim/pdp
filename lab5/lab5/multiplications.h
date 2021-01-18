#pragma once
#include "Polynomial.h"


void numberDiff(std::vector<int>* a, std::vector<int>* b);

void multiplyPolynomialsParallelNaive(
	std::vector<int> *a,
	std::vector<int> *b,
	std::vector<int> *result, 
	int threadNum);

std::vector<int> karatsubaSequential(
	std::vector<int> A, 
	std::vector<int> B);

Polynomial naiveParallelMultiplication(
	Polynomial A, 
	Polynomial B, 
	int numThreads, 
	double &execTime);

Polynomial karatsubaSequentialMultiplication(
	Polynomial A, 
	Polynomial B, 
	double &execTime);

Polynomial karatsubaParallelMultiplication(
	Polynomial A, 
	Polynomial B, 
	int numThreads, 
	double &execTime);

