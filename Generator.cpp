#include "Generator.h"


Generator::Generator(int seed) : p(383), q(503) {
	M = p * q;
	if (seed < 0 || seed >= M) seed = 1;
	x = seed;
}

int Generator::nextBit()
{
	int bit = 0;

	x = (x * x) % M;
	long long temp = x;
	while (temp > 0) {
		bit = bit ^ (temp & 1);
		temp = temp >> 1;
	}
	return bit;
}

int Generator::nextInt()
{
	int value = 0;

	for (int i = 0; i < INT_BITS; i++) {
		value = (value << 1) | nextBit();
	}

	return value;
}

double Generator::nextReal()
{
	unsigned int r = (unsigned int)nextInt();
	return r / (double)UINT_MAX;
}

int Generator::nextIntAB(int a, int b) {

	unsigned int r = (unsigned int) nextInt();
	return a + (r % (b - a + 1));
}
