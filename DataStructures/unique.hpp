#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Iter>
	struct _Unique_iterator;

	template <class _Container, bool lvalue_ref>
	class _Unique : _Container_wrapper<_Container, lvalue_ref>
	{
	public:
		using value_type = typename _Container::value_type;

		using container_iterator = typename _Container::iterator;
		using container_const_iterator = typename _Container::const_iterator;

		using iterator = _Unique_iterator<container_iterator>;
		using const_iterator = _Unique_iterator<container_const_iterator>;

		using base = _Container_wrapper<_Container, lvalue_ref>;
		using base::_Data;

		constexpr _Unique(_Container& _C)
			: base(_C)
		{ }
		constexpr _Unique(_Container&& _C)
			: base(move(_C))
		{ }
		constexpr auto begin() {
			return _Unique_iterator(_Data.begin(), _Data.end());
		}
		constexpr auto begin() const {
			return _Unique_iterator(_Data.begin(), _Data.end());
		}
		constexpr auto end() {
			return sentinel{};
		}
		constexpr auto end() const {
			return sentinel{};
		}
	};

	template <class _Iter>
	struct _Unique_iterator
	{
		using category = forward_iterator;

		constexpr _Unique_iterator(_Iter&& _Iterator, _Iter&& _Last)
			: _It(static_cast<_Iter&&>(_Iterator))
			, _End(static_cast<_Iter&&>(_Last))
		{ }
		constexpr decltype(auto) operator*() {
			return *_It;
		}
		constexpr decltype(auto) operator++() {
			_Iter _Prev = _It++;
			while (*_Prev == *_It && _It != _End) {
				++_It;
			}
			return *this;
		}
		constexpr auto operator++(int) {
			auto _Tmp = *this;
			++*this;
			return _Tmp;
		}
		constexpr friend bool operator==(const _Unique_iterator& _It, sentinel) {
			return _It._It == _It._End;
		}
		template <class _OtherIt>
		constexpr friend bool operator==(const _Unique_iterator& _Left, const _Unique_iterator<_OtherIt>& _Right) {
			return _Left._It == _Right._It;
		}

		_Iter _It;
		_Iter _End;
	};

	struct unique_fn
	{
		template <class _Container>
		constexpr auto operator()(_Container& _C) const {
			return _Unique<_Container, true>(_C);
		}
		template <class _Container>
		constexpr auto operator()(_Container&& _C) const {
			return _Unique<_Container, false>(move(_C));
		}
	};

	constexpr unique_fn unique = {};
}