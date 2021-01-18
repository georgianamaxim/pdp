#pragma once
#include <vector>
#include <algorithm>
#include <ostream>
#include <string>

class Polynomial {
private:
	std::vector<int> coeff;
	static std::vector<int> vectorToPolynomial(std::vector<int> &poly) {
		std::reverse(poly.begin(), poly.end());
		return poly;
	}

public:
	Polynomial() = default;

	explicit Polynomial(std::vector<int> &numVect): coeff{vectorToPolynomial(numVect)} {}


	const std::vector<int>& getCoefficients() const;
	void setCoeff(std::vector<int> &newCoeff);
	int getDegree() {
		return this->coeff.size() - 1;
	}

	int getLength() {
		return this->coeff.size();
	}

	std::string toString();
};