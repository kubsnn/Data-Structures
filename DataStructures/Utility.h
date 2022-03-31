#pragma once

#include "Hash.h"

template<class _Ty1, class _Ty2>
struct pair
{
	_Ty1 first;
	_Ty2 second;

	pair(_Ty1 _First, _Ty2 _Second) {
		first = _First;
		second = _Second;
	}
};

template<class _Ty1, class _Ty2>
struct Hash<pair<_Ty1, _Ty2>>
{
	size_t operator()(const pair<_Ty1, _Ty2>& _Pair) {
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
	_Ty tmp(move(_Right));
	_Right = move(_Left);
	_Left = move(tmp);
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
inline void fill_n(const _FwdIt _First, size_t _Count, const _Ty& _Val)
{
	auto _UFirst = const_cast<_FwdIt>(_First);
	for (size_t i = 0; i < _Count; ++i) {
		*(_UFirst++) = _Val;
	}
}
