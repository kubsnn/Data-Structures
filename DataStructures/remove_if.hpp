#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Iter, class _Fun, class _EndIter>
	struct _Remove_if_iterator;

	template <class _Container, class _Fun, bool lvalue_reference>
	class _Remove_if : _Container_wrapper<_Container, lvalue_reference>
	{
	public:
		using value_type = typename _Container::value_type;

		using container_iterator = typename _Container::iterator;
		using container_const_iterator = typename _Container::const_iterator;

		using iterator = _Remove_if_iterator<container_iterator, _Fun, container_iterator>;
		using const_iterator = _Remove_if_iterator<container_const_iterator, _Fun, container_iterator>;

		using base = _Container_wrapper<_Container, lvalue_reference>;
		using base::_Data;

		constexpr _Remove_if(_Container& _C, _Fun _Func)
			: base(_C)
			, _Fn(_Func)
		{ }
		constexpr _Remove_if(_Container&& _C, _Fun _Func)
			: base(move(_C))
			, _Fn(_Func)
		{ }
		constexpr auto begin() {
			return _Remove_if_iterator(_Data.begin(), _Fn, _Data.end());
		}
		constexpr auto begin() const {
			return _Remove_if_iterator(_Data.begin(), _Fn, _Data.end());
		}
		constexpr auto end() {
			return _Remove_if_iterator(_Data.end(), _Fn, _Data.end());
		}
		constexpr auto end() const {
			return _Remove_if_iterator(_Data.end(), _Fn, _Data.end());
		}
	private:
		_Fun _Fn;
	};

	template <class _Iter, class _Fun, class _EndIter>
	struct _Remove_if_iterator
	{
	public:
		using category = forward_iterator;

		constexpr _Remove_if_iterator(_Iter _Iterator, _Fun _Function, _EndIter _End)
			: _It(_Iterator)
			, _Fn(_Function)
			, _End(_End)
		{ }
		constexpr decltype(auto) operator*() {
			return *_It;
		}
		constexpr decltype(auto) operator++() {
			while (_Fn(*++_It)) {
				if (_It == _End) break;
			}
			return *this;
		}
		constexpr auto operator++(int) {
			auto _Tmp = *this;
			++*this;
			return *this;
		}
		template <class _OtherIt1, class _OtherIt2>
		constexpr friend bool operator==(
			const _Remove_if_iterator& _Left, 
			const _Remove_if_iterator<_OtherIt1, _Fun, _OtherIt2>& _Right) {
			return _Left._It == _Right._It;
		}

		_Iter _It;
		_EndIter _End;
		_Fun _Fn;
	};

	struct remove_if_fn
	{
		template <class _Fun>
		constexpr _Pipe_obj_arg<remove_if_fn, _Fun> operator()(_Fun _Fn) const {
			return { *this, _Fn };
		}
		template <class _Container, class _Fun>
		constexpr _Remove_if<_Container, _Fun, true> operator()(_Container& _C, _Fun _Fn) const {
			return _Remove_if<_Container, _Fun, true>(_C, _Fn);
		}
		template <class _Container, class _Fun>
		constexpr _Remove_if<_Container, _Fun, false> operator()(_Container&& _C, _Fun _Fn) const {
			return _Remove_if<_Container, _Fun, false>(move(_C), _Fn);
		}
	};

	constexpr remove_if_fn remove_if = {};
}