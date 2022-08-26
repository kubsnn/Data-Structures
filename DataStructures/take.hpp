#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Iter>
	struct _Take_iterator;

	template <class _Container, bool lvalue_reference>
	class _Take : _Container_wrapper<_Container, lvalue_reference>
	{
	public:
		using value_type = typename _Container::value_type;

		using container_iterator = typename _Container::iterator;
		using container_const_iterator = typename _Container::const_iterator;

		using iterator = _Take_iterator<container_iterator>;
		using const_iterator = _Take_iterator<container_const_iterator>;

		using base = _Container_wrapper<_Container, lvalue_reference>;
		using base::_Data;

		constexpr _Take(_Container& _C, size_t _Len)
			: base(_C)
			, _Len(_Len)
		{ }
		constexpr _Take(_Container&& _C, size_t _Len)
			: base(move(_C))
			, _Len(_Len)
		{ }
		constexpr auto begin() {
			return _Take_iterator(_Data.begin(), _Len);
		}
		constexpr auto begin() const {
			return _Take_iterator(_Data.begin(), _Len);
		}
		constexpr sentinel end() {
			return sentinel{};
		}
		constexpr sentinel end() const {
			return sentinel{};
		}
	private:
		size_t _Len;
	};

	template <class _Iter>
	struct _Take_iterator
	{
		using category = forward_iterator;

		constexpr _Take_iterator(_Iter _Iterator, size_t _Length)
			: _It(_Iterator)
			, _Left(_Length)
		{ }
		constexpr decltype(auto) operator*() {
			return *_It;
		}
		constexpr decltype(auto) operator++() {
			--_Left;
			return ++_It;
		}
		constexpr auto operator++(int) {
			auto _Tmp = *this;
			++*this;
			return _Tmp;
		}
		constexpr friend bool operator==(const _Take_iterator& _Left, sentinel) {
			return _Left._Left == 0;
		}
		constexpr friend bool operator==(const _Take_iterator& _Left, const _Take_iterator& _Right) {
			return _Left._It == _Right._It;
		}

		_Iter _It;
		size_t _Left;
	};

	struct take_fn
	{
		template <class _Container>
		constexpr _Take<_Container, true> operator()(_Container& _C, size_t _Length) const {
			return _Take<_Container, true>(_C, _Length);
		}
		template <class _Container>
		constexpr _Take<_Container, false> operator()(_Container&& _C, size_t _Length) const {
			return _Take<_Container, false>(move(_C), _Length);
		}
		constexpr _Pipe_obj_arg<take_fn, size_t> operator()(size_t _Length) const {
			return { *this, _Length };
		}
	};

	constexpr take_fn take = {};
}