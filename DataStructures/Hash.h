#pragma once
#include <iostream>

template<typename _Ty>
struct Hash
{
	virtual size_t operator()(const _Ty& item) = delete;
};

template<>
struct Hash<int>
{
	static constexpr const size_t _Mask = -1;
	static constexpr const size_t _Magic = _Mask ^ 2763427254;

	constexpr size_t operator()(const int& i) const
	{
		return i * _Magic;
	}
};

template<>
struct Hash<char>
{
	constexpr size_t operator()(const char& c) const
	{
		return Hash<int>()(c);
	}
};

template<>
struct Hash<long long>
{
	constexpr size_t operator()(const long long& ll) const
	{
		unsigned long long x = ll;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
		x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
		x = x ^ (x >> 31);
		return x;
	}
};

template<>
struct Hash<double>
{
	constexpr  size_t operator()(const double& d) const
	{
		return Hash<long long>()(*(size_t*)&d);
	}
};

template<>
struct Hash<std::string>
{
	constexpr size_t operator()(const std::string& s) const
	{
		size_t sum = 1;
		size_t n = s.length();
		size_t power = 1;
		const size_t MOD = static_cast<size_t>(1e9) + 7;

		for (int i = 0; i < n; ++i) {
			sum = (sum + (size_t)(s[i]) * power) % MOD;
			power = (power * 31) % MOD;
		}
		return sum;
	}
};
