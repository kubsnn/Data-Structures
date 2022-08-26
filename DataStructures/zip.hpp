#pragma once

#include "utility.h"

namespace pipeline
{
	template <class _Iter1, class _Iter2>
	struct _Zip_iterator;

	template <class _Ty1, class _Ty2, bool lvalue_1, bool lvalue_2>
	struct zip_pair;

	template <class _Container1, class _Container2, bool lvalue1, bool lvalue2>
	class _Zip : private zip_pair<_Container1, _Container2, lvalue1, lvalue2>
	{
	public:
		using base = zip_pair<_Container1, _Container2, lvalue1, lvalue2>;
		using base::first;
		using base::second;
		
		using value_type_1 = typename _Container1::value_type;
		using value_type_2 = typename _Container2::value_type;
		using value_type = zip_pair<value_type_1, value_type_2, false, false>;


		using iterator = _Zip_iterator<typename _Container1::iterator, typename _Container2::iterator>;
		using const_iterator = _Zip_iterator<typename _Container1::const_iterator, typename _Container2::const_iterator>;

		constexpr _Zip(_Container1& __C1, _Container2& __C2) 
			: base(__C1, __C2)
		{ }
		constexpr _Zip(_Container1&& __C1, _Container2& __C2)
			: base(move(__C1), __C2)
		{ }
		constexpr _Zip(_Container1& __C1, _Container2&& __C2)
			: base(__C1, move(__C2))
		{ }
		constexpr _Zip(_Container1&& __C1, _Container2&& __C2)
			: base(move(__C1), move(__C2))
		{ }
		
		constexpr auto begin() {
			return _Zip_iterator(first.begin(), second.begin());
		}
		constexpr auto begin() const {
			return _Zip_iterator(first.begin(), second.begin());
		}
		constexpr auto end() {
			return _Zip_iterator(first.end(), second.end());
		}
		constexpr auto end() const {
			return _Zip_iterator(first.end(), second.end());
		}
	};
	
	template <class _Iter1, class _Iter2>
	struct _Zip_iterator
	{
		using category = forward_iterator;
		
		constexpr _Zip_iterator(_Iter1 _It1, _Iter2 _It2)
			: _It_1(_It1)
			, _It_2(_It2)
		{ }

		constexpr decltype(auto) operator*() {
			if constexpr (is_lvalue_reference<decltype(*_It_1)>) {
				if constexpr (is_lvalue_reference<decltype(*_It_2)>) 
					return zip_pair<decltype(*_It_1), decltype(*_It_2), true, true>(*_It_1, *_It_2);
				else
					return zip_pair<decltype(*_It_1), decltype(*_It_2), true, false>(*_It_1, *_It_2);
			}
			else {
				if constexpr (is_lvalue_reference<decltype(*_It_2)>)
					return zip_pair<decltype(*_It_1), decltype(*_It_2), false, true>(*_It_1, *_It_2);
				else
					return zip_pair<decltype(*_It_1), decltype(*_It_2), false, false>(*_It_1, *_It_2);
			}
		}
		constexpr decltype(auto) operator++() {
			++_It_1;
			++_It_2;
			return *this;
		}
		constexpr decltype(auto) operator++(int) {
			auto _Tmp = *this;
			++_It_1;
			++_It_2;
			return _Tmp;
		}
		template <class _OtherIt1, class _OtherIt2>
		constexpr friend bool operator==(const _Zip_iterator& _Left, const _Zip_iterator<_OtherIt1, _OtherIt2>& _Right) {
			return _Left._It_1 == _Right._It_1 || _Left._It_2 == _Right._It_2;
		}
		
		_Iter1 _It_1;
		_Iter2 _It_2;
	};

	template <class _Ty1, class _Ty2, bool = true, bool = true>
	struct zip_pair
	{
		constexpr zip_pair(_Ty1& _F, _Ty2& _S)
			: first(_F), second(_S) { }

		template<bool _B_1, bool _B_2>
		constexpr zip_pair(const zip_pair<_Ty1, _Ty2, _B_1, _B_2>&) = delete;
		template<bool _B_1, bool _B_2>
		constexpr zip_pair(zip_pair<_Ty1, _Ty2, _B_1, _B_2>&&) = delete;

		_Ty1& first;
		_Ty2& second;
	};
	template <class _Ty1, class _Ty2>
	struct zip_pair<_Ty1, _Ty2, true, false>
	{
		constexpr zip_pair(_Ty1& _F, _Ty2&& _S)
			: first(_F), second(move(_S)) { }
		
		_Ty1& first;
		_Ty2 second;
	};
	template <class _Ty1, class _Ty2>
	struct zip_pair<_Ty1, _Ty2, false, true>
	{
		constexpr zip_pair(_Ty1&& _F, _Ty2& _S)
			: first(move(_F)), second(_S) { }

		_Ty1 first;
		_Ty2& second;
	};
	template <class _Ty1, class _Ty2>
	struct zip_pair<_Ty1, _Ty2, false, false>
	{
		constexpr zip_pair(_Ty1&& _F, _Ty2&& _S)
			: first(move(_F)), second(move(_S)) { }

		template<class __Ty1, class __Ty2, bool _B_1, bool _B_2>
		constexpr zip_pair(zip_pair<__Ty1, __Ty2, _B_1, _B_2>&& _Other) 
			: first(move(_Other.first))
			, second(move(_Other.second))
		{ }

		_Ty1 first;
		_Ty2 second;
	};

	struct zip_fn
	{
		template <class _Container_1, class _Container_2>
		constexpr auto operator()(_Container_1& _C_1, _Container_2& _C_2) const {
			return _Zip<_Container_1, _Container_2, true, true>(_C_1, _C_2);
		}
		template <class _Container_1, class _Container_2>
		constexpr auto operator()(_Container_1&& _C_1, _Container_2& _C_2) const {
			return _Zip<_Container_1, _Container_2, false, true>(move(_C_1), _C_2);
		}
		template <class _Container_1, class _Container_2>
		constexpr auto operator()(_Container_1& _C_1, _Container_2&& _C_2) const {
			return _Zip<_Container_1, _Container_2, true, false>(_C_1, move(_C_2));
		}
		template <class _Container_1, class _Container_2>
		constexpr auto operator()(_Container_1&& _C_1, _Container_2&& _C_2) const {
			return _Zip<_Container_1, _Container_2, false, false>(move(_C_1), move(_C_2));
		}
	};
	
	// can be only a param to pipe operator (can be lhs)
	constexpr zip_fn zip = {};
}