#include <iostream>

template<typename T>
T pow(T a, uint64_t n)
{
	T res(1);

	while (n--)
		res *= a;

	return res;
}

template<typename T>
T pow_fast(T a, uint64_t n)
{
	if (n == 0)
		return 1;

	// a * pow_fast(a, n / 2) * pow_fast(a, n / 2)

	T temp = pow_fast(a, n / 2);

	return n & 1 ? a * temp * temp : temp * temp;
}

template<typename T>
T pow_fast(T a, uint64_t n, uint64_t mod)
{
	if (n == 0)
		return 1;

	T temp = pow_fast(a, n / 2);

	return (n & 1 ? a * temp * temp : temp * temp) % mod;
}

uint64_t fib(uint64_t index)
{
	return index < 2 ? 1 : fib(index - 1) + fib(index - 2);
}

uint64_t fib_last_digit(uint64_t index)
{
	return index < 2 ? 1 : (fib_last_digit(index - 1) + fib_last_digit(index - 2)) % 10;
}

uint64_t fib_last_digit_n(uint64_t index)
{
	uint64_t prev = 1, curr = 1;

	for (uint64_t i = 1; i < index; ++i)
	{
		uint64_t temp = curr;
		curr += prev;
		curr %= 10;
		prev = temp;
	}

	return curr;
}

struct Matrix2x2
{
	uint64_t m[2][2];

	Matrix2x2(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
	{
		m[0][0] = a;
		m[0][1] = b;
		m[1][0] = c;
		m[1][1] = d;
	}

	Matrix2x2(uint64_t n)
	{
		m[0][0] = m[1][1] = n;
		m[0][1] = m[1][0] = 0;
	}

	Matrix2x2& operator%=(uint64_t n)
	{
		m[0][0] %= 10;
		m[0][1] %= 10;
		m[1][0] %= 10;
		m[1][1] %= 10;

		return *this;
	}

	Matrix2x2& operator*=(const Matrix2x2& rhs)
	{
		uint64_t temp[2][2] = {
			{
				m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
				m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1]
			},
			{
				m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
				m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
			},
		};

		m[0][0] = temp[0][0];
		m[0][1] = temp[0][1];
		m[1][0] = temp[1][0];
		m[1][1] = temp[1][1];

		return *this;
	}
};

Matrix2x2 operator*(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
	Matrix2x2 temp(lhs);
	return temp *= rhs;
}

Matrix2x2 operator%(const Matrix2x2& lhs, uint64_t rhs)
{
	Matrix2x2 temp(lhs);
	return temp %= rhs;
}

uint64_t fast_fib_last_digit(uint64_t index)
{
	return pow_fast(Matrix2x2(1,1,1,0), index, 10).m[0][0];
}

int main()
{
	/*for (int i = 0; i < 9; ++i)
		std::cout << fib_last_digit(i) << std::endl;

	std::cout << std::endl;

	for (int i = 0; i < 9; ++i)
		std::cout << fib_last_digit_n(i) << std::endl;

	std::cout << std::endl;

	for (int i = 0; i < 9; ++i)
		std::cout << fast_fib_last_digit(i) << std::endl;*/

	std::cout << fib(10) << std::endl;

	return 0;
}
