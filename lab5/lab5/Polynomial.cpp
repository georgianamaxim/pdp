#include "pch.h"
#include "Polynomial.h"
#include <iostream>

const std::vector<int> &Polynomial::getCoefficients() const {
	return this->coeff;
}

void Polynomial::setCoeff(std::vector<int> &newCoeff) {
	this->coeff = newCoeff;
}

std::string Polynomial::toString() {
	std::string poly = "";
	int power = this->coeff.size() - 1;
	for (int i = getDegree(); i >= 0; i--) {
		if (this->coeff[i] == 0) {
			continue;
		}
		poly += " ";
		std::string c = std::to_string(coeff[i]);
		poly += c;
		poly += "x^";
		std::string p = std::to_string(power);
		poly += p;
		poly += " +";
		power -= 1;
	}
	poly.erase(std::prev(poly.end()));
	return poly;
}