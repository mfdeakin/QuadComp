
#include <array>
#include <cmath>
#include <stdio.h>
#include <random>

#include "accurate_math.hpp"

enum ldtype {
	LD_QUADRUPLE,
	LD_DOUBLEEXT,
	LD_DOUBLE,
	LD_BADCOMPILER,
	LD_UNKNOWN
};

const char *ldnames[] = {"Quadruple", "Double Extended", "Double", "Bad Compiler Error", "Unknwon"};

enum ldtype testLDType() {
	long double s, t = 3.0;
	s = 1.0 - (4.0 / t - 1.0) * t;
	int exponent;
	std::frexp(s, &exponent);
	exponent--;
	if(exponent == -112)
		return LD_QUADRUPLE;
	else if(exponent == -52)
		return LD_DOUBLE;
	else if(exponent == -63)
		return LD_DOUBLEEXT;
	else if(s == 0.0)
		return LD_BADCOMPILER;
	else
		return LD_UNKNOWN;
}

template <typename fptype, unsigned n>
fptype evalPolynomial(const fptype coeffs[n], fptype s) {
	fptype sum = 0.0;
	for(int i = 0; i < n; i++) {
		sum = sum * s + coeffs[i];
	}
	return sum;
}

template <typename intype, typename calctype>
std::array<calctype, 2> naiveQuadraticRoots(const intype coeffs[3]) {
	calctype disc = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	std::array<calctype, 2> roots;
	roots[0] = (-coeffs[1] - copysign(1.0, coeffs[1]) * sqrt(disc)) /
		2 / coeffs[0];
	roots[1] = 2 * coeffs[2] /
		(-coeffs[1] - copysign(1.0, coeffs[1]) * sqrt(disc));
	return roots;
}

template <typename fptype>
std::array<fptype, 2> kahanQuadraticRoots(const fptype coeffs[3]) {
	fptype disc = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	std::array<fptype, 2> roots;
	return roots;
}

int main(int argc, char **argv) {
  std::random_device rd;
  std::mt19937_64 engine(rd());
	constexpr float maxMag = 1024.0 * 1024.0;
  std::uniform_real_distribution<float> rgenf(-maxMag,
                                              maxMag);
	constexpr int numTrials = 1;
	for(int i = 0; i < numTrials; i++) {
		float coeffs[3];
		for(int j = 0; j < 3; j++)
			coeffs[j] = rgenf(engine);
		coeffs[0] = 1.0;
		coeffs[1] = 1.0;
		coeffs[2] = -12.0;
		auto correctRoots = naiveQuadraticRoots<float, long double>(coeffs);
		for(int j = 0; j < 2; j++) {
			float ev = evalPolynomial<float, 3>(coeffs, correctRoots[j]);
			printf("Root %d: %Le -> %e\n", j, correctRoots[j], ev);
		}
	}
	return 0;
}
