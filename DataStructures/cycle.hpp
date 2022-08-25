#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Iter>
	struct _Cycle_iterator;

	template <class _Container, bool lvalue_ref>
	class _Cycle : _Container_wrapper<_Container, lvalue_ref>
	{
	public:
		using container_iterator = typename _Container::iterator;
		using container_const_iterator = typename _Container::const_iterator;

		using iterator = _Cycle_iterator<container_iterator>;
		using const_iterator = _Cycle_iterator<container_const_iterator>;

		using base = _Container_wrapper<_Container, lvalue_ref>;
		using base::_Data;

		friend struct iterator;
		friend struct const_iterator;

		constexpr _Cycle(_Container& _C)
			: base(_C)
		{ }
		constexpr _Cycle(_Container&& _C)
			: base(move(_C))
		{ }
		constexpr auto begin() {
			return _Cycle_iterator(_Data.begin(), _Data.end());
		}
		constexpr auto begin() const {
			return _Cycle_iterator(_Data.begin(), _Data.end());
		}
		constexpr sentinel end() {
			return sentinel{};
		}
		constexpr sentinel end() const {
			return sentinel{};
		}
	};


	template <class _Iter>
	struct _Cycle_iterator
	{
		using category = forward_iterator;

		constexpr _Cycle_iterator(_Iter&& _First, _Iter&& _Last)
			: _It(_First)
			, _Begin(static_cast<_Iter&&>(_First))
			, _End(static_cast<_Iter&&>(_Last))
		{ }
		constexpr decltype(auto) operator*() {
			return *_It;
		}
		constexpr decltype(auto) operator++() {
			++_It;
			if (_It == _End) _It = _Begin;
			return *this;
		}
		constexpr auto operator++(int) {
			auto _Tmp = *this;
			++_It;
			if (_It == _End) _It = _Begin;
			return _Tmp;
		}
		template <class _OtherIt>
		constexpr friend bool operator==(const _Cycle_iterator& _Left, const _Cycle_iterator<_OtherIt>& _Right) {
			return _Left._It == _Right._It;
		}
		constexpr friend bool operator==(const _Cycle_iterator&, sentinel) {
			return false;
		}
		constexpr friend bool operator!=(const _Cycle_iterator&, sentinel) {
			return true;
		}

		_Iter _It;
		_Iter _Begin;
		_Iter _End;
	};

	struct cycle_fn
	{
		template <class _Container>
		constexpr _Cycle<_Container, true> operator()(_Container& _C) const {
			return _Cycle<_Container, true>(_C);
		}
		template <class _Container>
		constexpr _Cycle<_Container, false> operator()(_Container&& _C) const {
			return _Cycle<_Container, false>(move(_C));
		}
	};

	constexpr cycle_fn cycle = {};
}