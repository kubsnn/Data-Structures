#pragma once

#include "Utility.h"

template <class _Container>
struct _Enumerate_iterator;

template <class _Ty>
struct enumerate_pair;

template <class _Container, bool lvalue_reference = true>
class _Enumerate
{
public:
	using iterator = _Enumerate_iterator<typename _Container::iterator>;
	using const_iterator = _Enumerate_iterator<typename _Container::const_iterator>;

	friend struct iterator;
	friend struct const_iterator;

	constexpr _Enumerate(_Container& _Data_ref)
		: _Data(_Data_ref)
	{ }
	
	constexpr iterator begin() {
		return iterator(_Data.begin());
	}
	constexpr const_iterator begin() const {
		return const_iterator(_Data.begin());
	}
	constexpr iterator end() {
		return iterator(_Data.end());
	}
	constexpr const_iterator end() const {
		return const_iterator(_Data.end());
	}
private:
	_Container& _Data;
};

template <class _Container>
class _Enumerate<_Container, false>
{
public:
	using iterator = _Enumerate_iterator<typename _Container::iterator>;
	using const_iterator = _Enumerate_iterator<typename _Container::const_iterator>;

	friend struct iterator;
	friend struct const_iterator;

	constexpr _Enumerate(_Container&& _Data_ref)
		: _Data(move(_Data_ref))
	{ }

	constexpr iterator begin() {
		return iterator(_Data.begin());
	}
	constexpr const_iterator begin() const {
		return const_iterator(_Data.begin());
	}
	constexpr iterator end() {
		return iterator(_Data.end());
	}
	constexpr const_iterator end() const {
		return const_iterator(_Data.end());
	}
private:
	_Container _Data;
};

template <class _It>
struct _Enumerate_iterator
{
	using container_iterator = _It;

	using category = forward_iterator;

	constexpr _Enumerate_iterator(container_iterator&& It) 
		: _Iter(move(It))
	{ }
	constexpr decltype(auto) operator*() {
		return enumerate_pair<decltype(*_Iter)>(_Counter, *_Iter);
	}
	constexpr _Enumerate_iterator& operator++() {
		++_Iter;
		++_Counter;
		return *this;
	}
	constexpr _Enumerate_iterator& operator++(int) {
		auto _Tmp = *this;
		++_Iter;
		++_Counter;
		return *_Tmp;
	}
	constexpr bool operator==(const _Enumerate_iterator& _Other) const {
		return _Iter == _Other._Iter;
	}
	constexpr bool operator!=(const _Enumerate_iterator& _Other) const {
		return _Iter != _Other._Iter;
	}
	constexpr bool operator>(const _Enumerate_iterator& _Other) const {
		return _Iter > _Other._Iter;
	}
	constexpr bool operator<(const _Enumerate_iterator& _Other) const {
		return _Iter < _Other._Iter;
	}

	container_iterator _Iter;
	size_t _Counter = 0;
};

template <class _Ty>
struct enumerate_pair 
{
	constexpr enumerate_pair(size_t _I, _Ty& _Val) 
		: index(_I), value(_Val)
	{ }
	const size_t index;
	_Ty& value;
};

struct enumerate_fn
{
	template <class _Container>
	constexpr _Enumerate<_Container> operator()(_Container& _C) const {
		return _Enumerate<_Container, true>(_C);
	}
	template <class _Container>
	constexpr _Enumerate<_Container, false> operator()(_Container&& _C) const {
		return _Enumerate<_Container, false>(move(_C));
	}
};

constexpr const enumerate_fn enumerate{};

template <class _Container>
constexpr _Enumerate<_Container, true> operator|(_Container& _C, const enumerate_fn& _Enumerate) {
	return _Enumerate(_C);
}
template <class _Container>
constexpr _Enumerate<_Container, false> operator|(_Container&& _C, const enumerate_fn& _Enumerate) {
	return _Enumerate(move(_C));
}
