#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Container>
	struct _Enumerate_iterator;

	template <class _Ty, bool lvalue_ref>
	struct enumerate_pair;

	template <class _Container, bool lvalue_ref>
	class _Enumerate : _Container_wrapper<_Container, lvalue_ref>
	{
	public:

		using container_iterator = typename _Container::iterator;
		using container_const_iterator = typename _Container::const_iterator;

		using iterator = _Enumerate_iterator<container_iterator>;
		using const_iterator = _Enumerate_iterator<container_const_iterator>;

		using base = _Container_wrapper<_Container, lvalue_ref>;
		using base::_Data;

		using value_type = enumerate_pair<typename _Container::value_type, false>;//is_lvalue_reference<decltype(*_Data.begin())>>;

		friend struct iterator;
		friend struct const_iterator;

		constexpr _Enumerate(_Container& _Data_ref)
			: base(_Data_ref)
		{ }
		constexpr _Enumerate(_Container&& _Data_ref)
			: base(move(_Data_ref))
		{ }
		constexpr auto begin() {
			return _Enumerate_iterator(_Data.begin());
		}
		constexpr auto begin() const {
			return _Enumerate_iterator(_Data.begin());
		}
		constexpr auto end() {
			return _Enumerate_iterator(_Data.end());
		}
		constexpr auto end() const {
			return _Enumerate_iterator(_Data.end());
		}
	};

	template <class _Iter>
	struct _Enumerate_iterator
	{
		using category = forward_iterator;

		constexpr _Enumerate_iterator(_Iter&& It)
			: _It(static_cast<_Iter&&>(It))
		{ }
		constexpr decltype(auto) operator*() {
			if constexpr (is_lvalue_reference<decltype(*_It)>) {
				return enumerate_pair<decltype(*_It), true>(_Counter, *_It);
			} else {
				return enumerate_pair<decltype(*_It), false>(_Counter, *_It);
			}
		}
		constexpr decltype(auto) operator++() {
			++_It;
			++_Counter;
			return *this;
		}
		constexpr auto operator++(int) {
			auto _Tmp = *this;
			++_It;
			++_Counter;
			return *_Tmp;
		}
		template <class _OtherIt>
		constexpr friend bool operator==(const _Enumerate_iterator& _Left, const _Enumerate_iterator<_OtherIt>& _Right) {
			return _Left._It == _Right._It;
		}

		_Iter _It;
		size_t _Counter = 0;
	};

	template <class _Ty, bool lvalue_ref = true>
	struct enumerate_pair
	{
		constexpr enumerate_pair(size_t _I, _Ty& _Val)
			: index(_I), value(_Val)
		{ }
		const size_t index;
		_Ty& value;
	};
	template <class _Ty>
	struct enumerate_pair<_Ty, false>
	{
		constexpr enumerate_pair(size_t _I, _Ty&& _Val)
			: index(_I), value(move(_Val))
		{ }
		const size_t index;
		_Ty value;
	};

	struct enumerate_fn
	{
		template <class _Container>
		constexpr auto operator()(_Container& _C) const {
			return _Enumerate<_Container, true>(_C);
		}
		template <class _Container>
		constexpr auto operator()(_Container&& _C) const {
			return _Enumerate<_Container, false>(move(_C));
		}
	};

	constexpr const enumerate_fn enumerate{};

}