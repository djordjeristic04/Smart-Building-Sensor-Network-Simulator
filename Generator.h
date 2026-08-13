#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>
using namespace std;
#define INT_BITS 32

class Generator {
public:

	Generator(int seed);
	int nextBit();
	int nextInt();
	double nextReal();
	int nextIntAB(int a, int b);

private:

	long long p, q;
	long long M;
	long long x;
};
#endif