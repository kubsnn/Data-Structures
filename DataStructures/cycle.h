#pragma once

#include "Utility.h"

template <class It, class _Container>
struct cycle_iterator;

template <class _Container>
class cycle_t
{
public:
	using iterator = cycle_iterator<typename _Container::iterator, _Container>;
	using const_iterator = cycle_iterator<typename _Container::const_iterator, _Container>;

	friend struct iterator;
	friend struct const_iterator;

	constexpr cycle_t(const _Container& _Val) 
		: _Data(_Val)
	{ }
	constexpr cycle_t(_Container&& _Val) 
		: _Data(move(_Val))
	{ }
	constexpr iterator begin() {
		return iterator(_Data.begin(), _Data);
	}
	constexpr const_iterator begin() const {
		return const_iterator(_Data.begin(), _Data);
	}
	constexpr iterator end() {
		return iterator(_Data.end(), _Data);
	}
	constexpr const_iterator end() const {
		return const_iterator(_Data.end(), _Data);
	}
private:
	_Container _Data;
};


template <class It, class _Container>
struct cycle_iterator 
{
	using category = typename It::category;

	constexpr cycle_iterator(It&& _It, _Container& _C) 
		: _Iter(static_cast<It&&>(_It)), _Data(_C)
	{ }
	constexpr decltype(auto) operator*() {
		return *_Iter;
	}
	constexpr decltype(auto) operator->() {
		return _Iter.operator->();
	}
	constexpr cycle_iterator& operator++() {
		++_Iter;
		
		if (_Iter == _Data.end()) _Iter = _Data.begin();
		return *this;
	}
	constexpr cycle_iterator operator++(int) {
		auto _Tmp = *this;
		++_Iter;
		if (_Iter == _Data.end()) _Iter = _Data.begin();
		return _Tmp;
	}
	constexpr bool operator==(const cycle_iterator& _Other) const {
		return _Iter == _Other._Iter;
	}
	constexpr bool operator!=(const cycle_iterator& _Other) const {
		return _Iter != _Other._Iter;
	}

private:
	_Container& _Data;
	It _Iter;
};

struct cycle_fn
{
	template <class _Container>
	constexpr cycle_t<_Container> operator()(const _Container& _C) const {
		return cycle_t<_Container>(_C);
	}
};

constexpr const cycle_fn cycle{};

template <class _Container>
constexpr cycle_t<_Container> operator|(const _Container& _C, const cycle_fn& _Cycle) {
	return _Cycle(_C);
}