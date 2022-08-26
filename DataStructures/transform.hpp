#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Iter, class _Fun>
	struct _Transform_iterator;

	template <class _Container, class _Fun, bool lvalue_ref>
	class _Transform : _Container_wrapper<_Container, lvalue_ref>
	{
	public:

		using container_iterator = typename _Container::iterator;
		using container_const_iterator = typename _Container::const_iterator;

		using iterator = _Transform_iterator<container_iterator, _Fun>;
		using const_iterator = _Transform_iterator<container_const_iterator, _Fun>;

		using base = _Container_wrapper<_Container, lvalue_ref>;
		using base::_Data;

		constexpr _Transform(_Container& _Data, _Fun _Function)
			: base(_Data)
			, _Fn(_Function)
		{ }
		constexpr _Transform(_Container&& _Data, _Fun _Function)
			: base(move(_Data))
			, _Fn(_Function)
		{ }
		constexpr auto begin() {
			return _Transform_iterator(_Data.begin(), _Fn);
		}
		constexpr auto begin() const {
			return _Transform_iterator(_Data.begin(), _Fn);
		}
		constexpr auto end() {
			return _Transform_iterator(_Data.end(), _Fn);
		}
		constexpr auto end() const {
			return _Transform_iterator(_Data.end(), _Fn);
		}

		using value_type = typename _Container::value_type;
	private:
		_Fun _Fn;
	};

	template <class _Iter, class _Fun>
	struct _Transform_iterator
	{
		using category = forward_iterator;

		constexpr _Transform_iterator(_Iter&& _Iterator, _Fun _Function)
			: _It(static_cast<_Iter&&>(_Iterator))
			, _Fn(_Function)
		{ }
		constexpr decltype(auto) operator*() {
			return _Fn(*_It);
		}
		constexpr decltype(auto) operator++() {
			++_It;
			return *this;
		}
		constexpr auto operator++(int) {
			auto _Tmp = *this;
			++_It;
			return _Tmp;
		}
		template <class _OtherIt>
		constexpr friend bool operator==(const _Transform_iterator& _Left, const _Transform_iterator<_OtherIt, _Fun>& _Right) {
			return _Left._It == _Right._It;
		}

		_Iter _It;
		_Fun _Fn;
	};
	
	struct transform_fn
	{
		template <class _Fun>
		constexpr _Pipe_obj_arg<transform_fn, _Fun> operator()(_Fun _Fn) const {
			return { *this, _Fn };
		}
		template <class _Container, class _Fun>
		constexpr _Transform<_Container, _Fun, true> operator()(_Container& _C, _Fun _Fn) const {
			return _Transform<_Container, _Fun, true>(_C, _Fn);
		}
		template <class _Container, class _Fun>
		constexpr _Transform<_Container, _Fun, false> operator()(_Container&& _C, _Fun _Fn) const {
			return _Transform<_Container, _Fun, false>(move(_C), _Fn);
		}
	};

	constexpr transform_fn transform = {};
}