#pragma once
#include <iostream>

template<typename T>
struct Hash
{
	virtual size_t operator()(const T& item, int iteration = 1) = delete;
};

template<>
struct Hash<int>
{
	size_t operator()(const int& i, int iteration = 1) const
	{
		int x = i + iteration;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		return x;
	}
};

template<>
struct Hash<char>
{
	inline size_t operator()(const char& c, int iteration = 1) const
	{
		return Hash<int>()(c, iteration);
	}
};

template<>
struct Hash<long long>
{
	size_t operator()(const long long& ll, int iteration = 1) const
	{
		unsigned long long x = ll + iteration;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
		x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
		x = x ^ (x >> 31);
		return x;
	}
};

template<>
struct Hash<double>
{
	inline size_t operator()(const double& d, int iteration = 1) const
	{
		double x = d + iteration;
		return Hash<long long>()(*(size_t*)&x, iteration);
	}
};

template<>
struct Hash<std::string>
{
	size_t operator()(const std::string& s, int iteration = 1) const
	{
		size_t sum = 1;
		size_t n = s.length();
		size_t power = 1;
		const size_t MOD = 1e9 + 7;

		for (int i = 0; i < n; ++i) {
			sum = (sum + (size_t)(s[i] + iteration) * power) % MOD;
			power = (power * 31) % MOD;
		}
		return sum;
	}
};
