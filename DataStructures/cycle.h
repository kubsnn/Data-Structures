#pragma once

#include "Utility.h"

namespace pipeline
{
	template <class It, class _Container>
	struct _Cycle_iterator;

	template <class _Container>
	class _Cycle
	{
	public:
		using iterator = _Cycle_iterator<typename _Container::iterator, _Container>;
		using const_iterator = _Cycle_iterator<typename _Container::const_iterator, _Container>;

		friend struct iterator;
		friend struct const_iterator;

		constexpr _Cycle(_Container& _Val)
			: _Data(_Val)
		{ }
		constexpr _Cycle(_Container&& _Val)
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
	struct _Cycle_iterator
	{
		using category = typename It::category;

		constexpr _Cycle_iterator(It&& _It, _Container& _C)
			: _Iter(static_cast<It&&>(_It)), _Data(_C)
		{ }
		constexpr decltype(auto) operator*() {
			return *_Iter;
		}
		constexpr decltype(auto) operator->() {
			return _Iter.operator->();
		}
		constexpr _Cycle_iterator& operator++() {
			++_Iter;

			if (_Iter == _Data.end()) _Iter = _Data.begin();
			return *this;
		}
		constexpr _Cycle_iterator operator++(int) {
			auto _Tmp = *this;
			++_Iter;
			if (_Iter == _Data.end()) _Iter = _Data.begin();
			return _Tmp;
		}
		constexpr bool operator==(const _Cycle_iterator& _Other) const {
			return _Iter == _Other._Iter;
		}
		constexpr bool operator!=(const _Cycle_iterator& _Other) const {
			return _Iter != _Other._Iter;
		}

	private:
		_Container& _Data;
		It _Iter;
	};

	struct cycle_fn
	{
		template <class _Container>
		constexpr auto operator()(_Container& _C) const {
			return _Cycle<_Container>(_C);
		}
		template <class _Container>
		constexpr auto operator()(_Container&& _C) const {
			return _Cycle<_Container>(move(_C));
		}
	};

	constexpr cycle_fn cycle = {};
}