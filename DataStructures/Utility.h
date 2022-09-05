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

#if defined(_MSVC_LANG)
#define __CPPVER _MSVC_LANG
#else
#define __CPPVER __cplusplus
#endif

struct _Empty {};

//
//   iterator categories

/*
	iterator must have:
		- operator++ / operator--
		- operator*
		- operator==
	should have:
		- operator->

	random access iterator requirements:
		- unwrap() method which returns pointer to memory
		- operator<, >, <=, >=
*/
struct random_access_iterator {};
struct forward_iterator {};
struct bidirectional_iterator {};

// !-->

template <class _Ty>
struct remove_const {
	using type = _Ty;
};

template <class _Ty>
struct remove_const<const _Ty> {
	using type = _Ty;
};

template <class _Ty>
using remove_const_t = typename remove_const<_Ty>::type;

template <class _Ty>
struct remove_volatile {
	using type = _Ty;
};

template <class _Ty>
struct remove_volatile<volatile _Ty> {
	using type = _Ty;
};

template <class _Ty>
using remove_volatile_t = typename remove_volatile<_Ty>::type;

template <class>
constexpr bool is_pointer_v = false; 

template <class _Ty>
constexpr bool is_pointer_v<_Ty*> = true;

template <class _Ty>
constexpr bool is_pointer_v<_Ty* const> = true;

template <class _Ty>
constexpr bool is_pointer_v<_Ty* volatile> = true;

template <class _Ty>
constexpr bool is_pointer_v<_Ty* const volatile> = true;

template <class, class>
constexpr bool is_same = false;

template <class _Ty>
constexpr bool is_same<_Ty, _Ty> = true;

template <class _Ty, class _Type = void>
struct enable_if {
	using type = _Type;
};

template <class _Ty, class _Tag, class Enable = void>
constexpr bool has_tag = false;

template <class _Ty, class _Tag>
constexpr bool has_tag<_Ty*, _Tag> = true;

template <class _Ty, class _Tag>
constexpr bool has_tag<_Ty* const, _Tag> = true;

template <class _Ty, class _Tag>
constexpr bool has_tag<_Ty* volatile, _Tag> = true;

template <class _Ty, class _Tag>
constexpr bool has_tag<_Ty, _Tag, typename enable_if<typename _Ty::category>::type> = is_same<typename _Ty::category, _Tag>;

template <class _Ty>
constexpr bool is_random_access = has_tag<_Ty, random_access_iterator>;

template <class _Ty, class Enable = void>
struct tag_of {
	using tag = void;
};

template <class _Ty>
struct tag_of<_Ty*> {
	using tag = random_access_iterator;
};

template <class _Ty>
struct tag_of<_Ty* const> {
	using tag = random_access_iterator;
};

template <class _Ty>
struct tag_of<_Ty* volatile> {
	using tag = random_access_iterator;
};

template <class _Ty>
struct tag_of<_Ty, typename enable_if<typename _Ty::category>::type> {
	using tag = typename _Ty::category;
};

template <class _Ty>
using tag_of_t = typename tag_of<_Ty>::tag;

template <class _Ty>
inline constexpr const _Ty& max(const _Ty& _Left, const _Ty& _Right) {
	return _Left < _Right ? _Right : _Left;
}

template <class _Ty>
inline constexpr const _Ty& min(const _Ty& _Left, const _Ty& _Right) {
	return _Left > _Right ? _Right : _Left;
}

template <class _Ty>
struct remove_reference {
	using type = _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&> {
	using type = _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&&> {
	using type = _Ty;
};

template <class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;

template <class _Ty>
inline constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) noexcept {
	return static_cast<_Ty&&>(_Arg);
}

template <class _Ty>
inline constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept { 
	return static_cast<_Ty&&>(_Arg);
}

template <class _Ty>
inline constexpr remove_reference_t<_Ty>&& move(_Ty& _Arg) noexcept {
	return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}

template <class _Ty>
inline constexpr void swap(_Ty& _Left, _Ty& _Right) {
	_Ty _Tmp(move(_Right));
	_Right = move(_Left);
	_Left = move(_Tmp);
}

template <class _Iter>
inline constexpr void iter_swap(_Iter _Left, _Iter _Right) {
	swap(*_Left, *_Right);
}

template <class _FwdIt, class _Ty>
inline constexpr void fill(const _FwdIt _First, const _FwdIt _Last, const _Ty& _Val) {
	auto _UFirst = const_cast<_FwdIt&>(_First);
	for (; _UFirst != _Last; ++_UFirst) {
		*_UFirst = _Val;
	}
}

template <class _FwdIt, class _Ty>
inline constexpr void fill_n(const _FwdIt _First, size_t _Count, const _Ty& _Val) {
	auto _UFirst = const_cast<_FwdIt&>(_First);
	for (size_t i = 0; i < _Count; ++i) {
		*(_UFirst++) = _Val;
	}
}

template <class _InIt, class _OutIt>
inline void copy(_InIt _First, _InIt _Last, _OutIt _Dest) {
	for (; _First != _Last; ++_First, ++_Dest) {
		*_Dest = *_First;
	}
}

template <class _InIt, class _OutIt>
inline void move_mem(_InIt _Src, _OutIt _Dst, size_t _Count) {
	if (_Src == _Dst) return;
	if (_Src > _Dst) {
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

template <class _Ty>
inline constexpr _Ty nearest_bigger_power_of_2(const _Ty& _Val) {
	int _Pow = 1;
	while (_Pow < _Val) _Pow <<= 1;
	return _Pow;
}

template <class InputIt1, class InputIt2>
inline bool lexicographical_compare(const InputIt1 _First1, const InputIt1 _Last1,
	const InputIt2 _First2, const InputIt2 _Last2) {
	auto _UFirst1 = const_cast<InputIt1&>(_First1);
	auto _UFirst2 = const_cast<InputIt2&>(_First2);
	for (; (_UFirst1 != _Last1) && (_UFirst2 != _Last2); ++_UFirst1, ++_UFirst2) {
		if (*_UFirst1 < *_UFirst2) return true;
		if (*_UFirst2 < *_UFirst1) return false;
	}
	return (_UFirst1 == _Last1) && (_UFirst2 != _Last2);
}

template <class InputIt1, class InputIt2>
inline constexpr bool arrays_compare(const InputIt1 _First1, const InputIt1 _Last1,
	const InputIt2 _First2, const InputIt2 _Last2) {
	if (_Last1 - _First1 != _Last2 - _First2) return false;

	auto _UFirst1 = const_cast<InputIt1&>(_First1);
	auto _UFirst2 = const_cast<InputIt2&>(_First2);
	for (; _UFirst1 != _Last1; ++_UFirst1, ++_UFirst2) {
		if (*_UFirst1 != *_UFirst2) return false;
	}
	return true;
}

template <class Iter>
inline constexpr decltype(auto) unwrap(Iter&& _It) {
	if constexpr (is_pointer_v<remove_reference_t<Iter>>)
		return _It;
	else
		return static_cast<Iter&&>(_It).unwrap();
}

template <class FwdIt1, class FwdIt2>
inline constexpr size_t _Distance_helper(const FwdIt1 _First, const FwdIt2 _Last) {
	size_t _Dist = 0;
	auto _UFirst = const_cast<FwdIt1&>(_First);
	for (; _UFirst != _Last; ++_UFirst, ++_Dist) { }
	return _Dist;
}

template <class FwdIt1, class FwdIt2>
inline constexpr size_t distance(const FwdIt1 _First, const FwdIt2 _Last) {

	if constexpr (is_random_access<FwdIt1> && is_random_access<FwdIt2>)
		return static_cast<size_t>(unwrap(_Last) - unwrap(_First));
	else
		return _Distance_helper(_First, _Last);
}

template <class Iter, class Distance>
constexpr void _Do_advance(Iter& _It, Distance _Count, forward_iterator) {
	while (_Count > 0) {
		++_It;
		--_Count;
	}
}

template <class Iter, class Distance>
constexpr void _Do_advance(Iter& _It, Distance _Count, bidirectional_iterator) {
	while (_Count > 0) {
		++_It;
		--_Count;
	}
	while (_Count < 0) {
		--_It;
		++_Count;
	}
}

template <class Iter, class Distance>
constexpr void _Do_advance(Iter& _It, Distance _Count, random_access_iterator) {
	_It += _Count;
}

template <class Iter, class Distance>
constexpr void advance(Iter& _It, Distance _Count) {
	_Do_advance(_It, _Count, tag_of_t<Iter>());
}

template <class _Ty>
inline constexpr bool is_power_of_2(const _Ty& _Val) {
	return (_Val & (_Val - 1)) == 0;
}

template <class _Ty1, class _Ty2>
class pair
{
public:
	constexpr pair()
		: first()
		, second()
	{ }
	constexpr pair(const pair& _Other)
		: first(_Other.first)
		, second(_Other.second)
	{ }
	constexpr pair(pair&& _Other) noexcept
		: first(move(_Other.first))
		, second(move(_Other.second))
	{ }
	constexpr pair(const _Ty1& _X, const _Ty2& _Y)
		: first(_X)
		, second(_Y)
	{ }
	constexpr pair(const _Ty1& _X, _Ty2&& _Y)
		: first(_X)
		, second(move(_Y))
	{ }
	constexpr pair(_Ty1&& _X, const _Ty2& _Y)
		: first(move(_X))
		, second(_Y)
	{ }
	constexpr pair(_Ty1&& _X, _Ty2&& _Y)
		: first(move(_X))
		, second(move(_Y))
	{ }

	constexpr pair& operator=(const pair& _Other) {
		first = _Other.first;
		second = _Other.second;
		return *this;
	}
	constexpr pair& operator=(pair&& _Other) noexcept {
		first = move(_Other.first);
		second = move(_Other.second);
		return *this;
	}
	constexpr bool operator==(const pair& _Other) const {
		return first == _Other.first && second == _Other.second;
	}

	_Ty1 first;
	_Ty2 second;
};

template <class _Container, bool lvalue_reference = true>
class _Container_wrapper
{
public:
	constexpr _Container_wrapper(_Container& _Ref)
		: _Data(_Ref)
	{ }
protected:
	_Container& _Data;
};

template <class _Container>
class _Container_wrapper<_Container, false>
{
public:
	constexpr _Container_wrapper(_Container&& _Ref)
		: _Data(move(_Ref))
	{ }
protected:
	_Container _Data;
};

template <class>
constexpr bool is_numeric = false;

template<>
constexpr bool is_numeric<bool> = true;

template<>
constexpr bool is_numeric<char> = true;

template<>
constexpr bool is_numeric<signed char> = true;

template<>
constexpr bool is_numeric<unsigned char> = true;

template<>
constexpr bool is_numeric<short> = true;

template<>
constexpr bool is_numeric<unsigned short> = true;

template<>
constexpr bool is_numeric<int> = true;

template<>
constexpr bool is_numeric<unsigned int> = true;

template<>
constexpr bool is_numeric<long> = true;

template<>
constexpr bool is_numeric<unsigned long> = true;

template<>
constexpr bool is_numeric<long long> = true;

template<>
constexpr bool is_numeric<unsigned long long> = true;

template <class>
constexpr bool is_lvalue_reference = false;

template <class _Ty>
constexpr bool is_lvalue_reference<_Ty&> = true;

template <class _Ty>
constexpr bool is_const = false;

template <class _Ty>
constexpr bool is_const<const _Ty> = true;

#if __CPPVER >= 201703L

namespace pipeline
{
	template <class _Fun, class _Arg>
	struct _Pipe_obj_arg
	{
		const _Fun& _Fn;
		_Arg _Arg;
	};
	template <class _Fun>
	struct _Pipe_obj
	{
		const _Fun& _Fn;
	};
	struct sentinel { };
}
#endif

namespace utils
{
	template <class Iter, class Pred>
	constexpr Iter find_if(Iter _First, Iter _Last, Pred _Pr) {
		for (; _First != _Last; ++_First) {
			if (_Pr(*_First)) return _First;
		}
		return _Last;
	}

	template <class Iter, class Fun>
	constexpr void transform(Iter _First, Iter _Last, Fun _Fn) {
		for (; _First != _Last; ++_First) {
			*_First = _Fn(*_First);
		}
	}

	template <class Iter, class Fun, class CondFun>
	constexpr void transform_if(Iter _First, Iter _Last, Fun _Fn, CondFun _Condition_fn) {
		for (; _First != _Last; ++_First) {
			if (_Condition_fn(*_First)) *_First = _Fn(*_First);
		}
	}

	template <class Iter, class _Ty>
	constexpr void replace(Iter _First, Iter _Last, const _Ty& _Old_value, const _Ty& _New_value) {
		for (; _First != _Last; ++_First) {
			if (*_First == _Old_value) *_First = _New_value;
		}
	}

	template <class Iter, class _Fun, class _Ty>
	constexpr void replace_if(Iter _First, Iter _Last, _Fun _Fn, const _Ty& _New_value) {
		for (; _First != _Last; ++_First) {
			if (_Fn(*_First)) *_First = _New_value;
		}
	}
}