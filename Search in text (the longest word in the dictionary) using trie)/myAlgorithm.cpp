#include "myAlgorithm.hpp"

long double quicklyPow(long double n, unsigned k) {
	if (k == 0) {
		return 1;
	}
	long double half = quicklyPow(n, k >> 1);
	return (k & 1) ? (half*half*n) : (half*half);
}

unsigned NOD(unsigned a,unsigned b) {
	if(a==0) {
		return b;
	}
	else if(b==0) {
		return a;
	}
	while(a!=b) {
		if(a>b) a-=b;
		else b-=a;
	}
	return a;
}

long long stepen(long long a, long long x, long long mod) {
	long long res = 1;
	while (x) {
		if (x & 1) {
			res = (res*a) % mod;
		}
		a = (a*a) % mod;
		x /= 2;
	}
	return res;
}

unsigned long long binom(unsigned n, unsigned k) {
	if (k == 0 || k==n) {
		return 1;
	}
	else if(k > n) {
		return 0;
	}
	++n;
	unsigned long long * arr=new unsigned long long[n+1];
	arr[n] = 1;
	for (unsigned i = 1; i <= n; ++i) {
		arr[i] = 1;
		for (unsigned j = i-1 ; j>1; --j) {
			arr[j] += arr[j - 1];
		}
	}

	unsigned long long value = arr[k];
	delete[] arr;

	return value;
}