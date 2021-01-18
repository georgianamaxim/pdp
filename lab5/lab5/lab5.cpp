

#include "pch.h"
#include "Polynomial.h"
#include "multiplications.h"
#include <iostream>
#include <thread>

Polynomial multiplicationSequential(Polynomial &p1, Polynomial &p2) {
	int sizeOfResultCoeffList = p1.getDegree() + p2.getDegree() + 1;
	std::vector<int> coeff;
	for (int i = 0; i < sizeOfResultCoeffList; i++) {
		coeff.push_back(0);
	}

	for (int i = 0; i < p1.getCoefficients().size(); i++) {
		for (int j = 0; j < p2.getCoefficients().size(); j++) {
			int ind = i + j;
			int value = p1.getCoefficients()[i] * p2.getCoefficients()[j];
			coeff[ind] += value;
		}
	}
	return Polynomial(coeff);
}

bool isPowerOf2(int n) {
	return (n & (n - 1)) == 0;
}

Polynomial getRandomPolynomial(int n, int min, int max) {
	while (!isPowerOf2(n)) {
		n++;
	}
	std::vector<int> coeffs;
	for (int i = 0; i < n; i++) {
		int currCoeff = rand() % (max - min) + min;
		coeffs.push_back(currCoeff);
	}

	return Polynomial(coeffs);
}


int main()
{
	int threads = 12;
	double extime;
	Polynomial p1 = getRandomPolynomial(32, 1, 10);
	Polynomial p2 = getRandomPolynomial(32, 1, 10);
	std::cout << p1.toString() << '\n';
	std::cout << p2.toString() << '\n';
	auto startTime = std::chrono::steady_clock::now();
	auto rez = multiplicationSequential(p1, p2);
	auto stopTime = std::chrono::steady_clock::now();
	auto diff = stopTime - startTime;
	auto time = std::chrono::duration<double, std::milli>(diff).count();
	time = time * 0.001;
	std::cout << "Sequential O(n^2) time: " << time << '\n';
	rez = naiveParallelMultiplication(p1, p2, threads, extime);
	std::cout << "Parallel O(n^2) time: " << extime << '\n';
	rez = karatsubaSequentialMultiplication(p1, p2, extime);
	std::cout << "Karatsuba's sequential time: " << extime << '\n';
	rez = karatsubaParallelMultiplication(p1, p2, threads, extime);
	std::cout << "Karatsuba's parallel time: " << extime << '\n';

	return 0;
}

