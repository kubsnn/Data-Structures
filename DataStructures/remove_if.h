#pragma once

#include "Utility.h"

template <class _Container, class _Fun, bool lvalue_reference>
class _Remove_if : _Container_wrapper<_Container, lvalue_reference>
{
public:
	using iterator = typename _Container::iterator;
	using const_iterator = typename _Container::const_iterator;

	using base = _Container_wrapper<_Container, lvalue_reference>;

	using base::base;
	using base::_Data;

	constexpr _Remove_if(_Container&& _C, _Fun _Func)
		: base(move(_C))
		, _Fn(_Func)
		, _End(unwrap(_Data.end()))
		, _CEnd(unwrap(_Data.end())) {
		run();
	}
	constexpr _Remove_if(_Container& _C, _Fun _Func)
		: base(_C)
		, _Fn(_Func)
		, _End(unwrap(_Data.end()))
		, _CEnd(unwrap(_Data.end())) {
		run();
	}
	constexpr void run() {
		auto _Next = find_if(_Data.begin(), _Data.end(), _Fn);
		if (_Next == _Data.end()) return;

		auto it = _Data.begin();
		for (; it != _Data.end(); ++it) {
			if (!_Fn(*it)) {
				*_Next = move(*it);
				++_Next;
			}
		}
		_End = _Next;
		_CEnd = const_iterator(unwrap(_Next));
	}
	constexpr iterator begin() {
		return iterator(_Data.begin());
	}
	constexpr const_iterator begin() const {
		return const_iterator(_Data.begin());
	}
	constexpr iterator end() {
		return _End;
	}
	constexpr const_iterator end() const {
		return _CEnd;
	}
private:
	_Fun _Fn;
	iterator _End;
	const_iterator _CEnd;
};

struct remove_if_fn;

template <class _Fun>
struct _Remove_if_fun 
{
	const remove_if_fn& _Rm;
	_Fun _Fn;
};

struct remove_if_fn
{
	template <class _Container, class _Fun>
	constexpr _Remove_if<_Container, _Fun, true> operator()(_Container& _C, _Fun _Func) const {
		_Remove_if<_Container, _Fun, true> f(_C, _Func);
		return f;
	}
	template <class _Container, class _Fun>
	constexpr _Remove_if<_Container, _Fun, false> operator()(_Container&& _C, _Fun _Func) const {
		return _Remove_if<_Container, _Fun, false>(move(_C), _Func);
	}
	template <class _Fun>
	constexpr _Remove_if_fun<_Fun> operator()(_Fun _Func) const {
		return { *this, _Func };
	}
};

constexpr remove_if_fn remove_if = {};

template <class _Container, class _Fun>
constexpr _Remove_if<_Container, _Fun, true> operator|(_Container& _C, const _Remove_if_fun<_Fun>& _Func) {
	return _Func._Rm(_C, _Func._Fn);
}

template <class _Container, class _Fun>
constexpr _Remove_if<_Container, _Fun, false> operator|(_Container&& _C, const _Remove_if_fun<_Fun>& _Func) {
	return _Func._Rm(move(_C), _Func._Fn);
}
