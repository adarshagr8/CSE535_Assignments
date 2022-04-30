/*

Name: Adarsh Agrawal
Roll: 18075005
Branch: CSE

*/

#include <bits/stdc++.h>
using namespace std;


/*
PRNG:

A pseudorandom number generator (PRNG), is an algorithm for generating a sequence
of numbers whose properties approximate the properties of sequences of random numbers

The PRNG-generated sequence is not truly random.

*/

class PsuedoRandomNumberGenerator {
	virtual void setSeed(int seed) = 0;
	virtual void updateSeed() = 0;
	virtual double nextNumber() = 0;
};

/*

Linear Congruential Generator:

*/

class LinearCongruentialGenerator : public PsuedoRandomNumberGenerator {
private:
	long long _multiplier;
	long long _increment;
	long long _modulus;
	long long _seed;

	void updateSeed() override {
		_seed = (_multiplier * 1ll * _seed + _increment) % _modulus;
	}

public:
	LinearCongruentialGenerator(long long a, long long c, long long mod): _multiplier(a), _increment(c), _modulus(mod) {
		_seed = 1;
	}

	void setSeed(int seed) override {
		_seed = seed;
	}

	double nextNumber() {
		updateSeed();
		return (double) _seed / _modulus;
	}
};

/*

Legged Fibonacci Generator

*/

class LeggedFibonacciGenerator : public PsuedoRandomNumberGenerator {
private:
	long long j;
	long long k;
	long long _modulus;
	vector<long long> f;
	int _seed;
	int _count;

	void updateSeed() override {
		const int a = 106;
		const int c = 1283;
		LinearCongruentialGenerator temp(a, c, _modulus);
		temp.setSeed(_seed);
		for(int i = 0; i < k; i++) {
			f[i] = temp.nextNumber() * _modulus;
		}
	}

public:
	LeggedFibonacciGenerator(int j, int k, long long mod, int seed = 3): j(j), k(k), _modulus(mod) {
		f.resize(k);
		_seed = seed;
		_count = 0;
		updateSeed();
	}

	void setSeed(int seed) override {
		_seed = seed;
		_count = 0;
		updateSeed();
	}

	double nextNumber() {
		if(_count < k) {
			return (double) f[_count++] / _modulus;
		}
		f[_count % k] = (f[(_count - j + k) % k] + f[(_count - k + k) % k]) % _modulus;
		int ans = f[_count % k];
		++_count;
		return (double) ans / _modulus;
	}
};


class ChiSquareTest {

private:
	long long n;

public:
	ChiSquareTest(long long _n): n(_n) {}

	double result(vector<double> seq) {
		unordered_map<int, int> freq;
		for(double &s : seq) {
			freq[s * n]++;
		}
		double ans = 0;
		double expected = (double) seq.size() / n;
		// trace(freq, expected);
		for(int s = 0; s < n; s++) {
			ans += (freq[s] - expected) * (freq[s] - expected) / expected;
		}
		return ans;
	}
};

class KolmogorovSmirnovTest {
private:
	int n;

public:
	KolmogorovSmirnovTest(int _n): n(_n) {}

	pair<double, double> result(vector<double> seq) {
		for(auto &s : seq)
			s *= n;
		sort(seq.begin(), seq.end());
		double maxDeviation = 0;
		double minDeviation = 0;

		for(int i = 0; i < seq.size(); i++) {
			if(i != seq.size() - 1 and seq[i + 1] == seq[i])
				continue;
			double fn = (double)(i + 1) / seq.size();
			double fX = (double)(seq[i] + 1) / n;
			double posDeviation = sqrt(n) * (fn - fX);
			double negDeviation = sqrt(n) * (fX - fn);

			maxDeviation = max(maxDeviation, posDeviation);
			minDeviation = max(minDeviation, negDeviation);
		}

		return {maxDeviation, minDeviation};
	}
};

int main() {

	/*
		With Linear Congruential Generator, it is important that we choose
		the values of the parameters carefully, since wrong choices would
		result in a bad PRNG algorithm.

		For this illustration, I have used the values from a paper
		Reference: https://dl.acm.org/doi/pdf/10.5555/2955239.2955463#:~:text=The%20ideal%20value%20is%2050,No%20LCG%20pass%20this%20test.

	*/

	const int a = 106;
	const int c = 1283;
	const long long mod1 = 6075;
	const long long mod2 = 6075;

	const int N = 100000;

	LinearCongruentialGenerator generatorLinear(a, c, mod1);


	/*
		With Legged Fibonacci Generator, I have chosen the parameters of 
		j and k according to the following archive:
		https://web.archive.org/web/20040309175607/http://www.ccs.uky.edu/csep/RN/RN.html

		For the initialisation of the first k values, I have used the 
		Linear Congruential Generator.
	*/

	const int j = 7;
	const int k = 10;

	LeggedFibonacciGenerator generatorLeggedFibonacci(j, k, mod2);
	
	vector<double> linearGeneratorOutput;

	vector<double> leggedGeneratorOutput;

	for(int i = 0; i < N; i++) {
		linearGeneratorOutput.push_back(generatorLinear.nextNumber());
		leggedGeneratorOutput.push_back(generatorLeggedFibonacci.nextNumber());
	}

	ChiSquareTest testerLinear(mod1);
	ChiSquareTest testerLegged(mod2);

	cout << "Chi Square Test Results:" << endl;
	cout << "------------------------" << endl;
	cout << "Linear Congruential Generator = " << testerLinear.result(linearGeneratorOutput) << endl;
	cout << "Legged Fibonacci Generator = " << testerLegged.result(leggedGeneratorOutput) << endl;

	cout << endl << endl;

	KolmogorovSmirnovTest testerLinear2(mod1);
	KolmogorovSmirnovTest testerLegged2(mod2);

	cout << "Kolmogorov-Smirnov Test Results:" << endl;
	cout << "-------------------------------" << endl;
	auto p1 = testerLinear2.result(linearGeneratorOutput);
	cout << "Linear Congruential Generator = " << "(K+: " << p1.first << ", K-: " << p1.second << ")"  << endl;
	auto p2 = testerLegged2.result(leggedGeneratorOutput);
	cout << "Legged Fibonacci Generator = " << "(K+: " << p2.first << ", K-: " << p2.second << ")"  << endl;


}