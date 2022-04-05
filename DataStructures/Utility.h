#pragma once

#include "Hash.h"

#ifdef _WIN64
typedef unsigned __int64 size_t;
typedef __int64          ptrdiff_t;
typedef __int64          intptr_t;
#else
typedef unsigned int     size_t;
typedef int              ptrdiff_t;
typedef int              intptr_t;
#endif

template<class _Ty1, class _Ty2>
struct Pair
{
	_Ty1 first;
	_Ty2 second;

	Pair(_Ty1 _First, _Ty2 _Second) {
		first = _First;
		second = _Second;
	}
};

template<class _Ty1, class _Ty2>
struct Hash<Pair<_Ty1, _Ty2>>
{
	size_t operator()(const Pair<_Ty1, _Ty2>& _Pair) {
		size_t hash1 = Hash<_Ty1>()(_Pair.first);
		size_t hash2 = Hash<_Ty2>()(_Pair.second);
		return hash1 + hash2;
	}
};

template<class _Ty>
constexpr const _Ty& max(const _Ty& _Left, const _Ty& _Right)
{
	return _Left < _Right ? _Right : _Left;
}

template<class _Ty>
constexpr const _Ty& min(const _Ty& _Left, const _Ty& _Right)
{
	return _Left > _Right ? _Right : _Left;
}

template<class _Ty>
inline _Ty&& move(_Ty& _Arg)
{
	return static_cast<_Ty&&>(_Arg);
}

template<class _Ty>
inline void swap(_Ty& _Left, _Ty& _Right)
{
	_Ty _Tmp(move(_Right));
	_Right = move(_Left);
	_Left = move(_Tmp);
}

template<class _FwdIt, class _Ty>
inline void fill(const _FwdIt _First, const _FwdIt _Last, const _Ty& _Val)
{
	auto _UFirst = const_cast<_FwdIt>(_First);
	for (; _UFirst != _Last; ++_UFirst) {
		*_UFirst = _Val;
	}
}

template<class _FwdIt, class _Ty>
constexpr inline void fill_n(const _FwdIt _First, size_t _Count, const _Ty& _Val)
{
	auto _UFirst = const_cast<_FwdIt>(_First);
	for (size_t i = 0; i < _Count; ++i) {
		*(_UFirst++) = _Val;
	}
}

template<class _InIt, class _OutIt>
inline void copy(_InIt _First, _InIt _Last, _OutIt _Dest)
{
	for (; _First != _Last; ++_First, ++_Dest) {
		*_Dest = *_First;
	}
}

template<class _InIt, class _OutIt>
inline void move_mem(_InIt _Src, _OutIt _Dst, size_t _Count)
{
	if (_Src == _Dst) return;
	if constexpr (_Src > _Dst) {
		for (int i = 0; i < _Count; ++i) {
			*_Dst = move(*_Src);
			++_Dst; ++_Src;
		}
	} else {
		_InIt _RSrc = _Src + _Count - 1;
		_OutIt _RDst = _Dst + _Count - 1;
		for (int i = 0; i < _Count; ++i) {
			*_RDst = move(*_RSrc);
			--_RDst; --_RSrc;
		}
	}
}

template<class _Ty>
inline constexpr _Ty nearest_bigger_power_of_2(const _Ty& _Val)
{
	int _Pow = 1;
	while (_Pow < _Val) _Pow >>= 1;
	return _Pow;
}