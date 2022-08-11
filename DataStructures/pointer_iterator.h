#pragma once

#include "Utility.h"

template <class _Ty>
struct pointer_iterator
{
public:
	using category = random_access_iterator;

	constexpr pointer_iterator(_Ty* _Pointer)
		: _Pos(_Pointer)
	{ }
	constexpr pointer_iterator(const pointer_iterator& _Other)
		: _Pos(_Other._Pos)
	{ }
	constexpr _Ty& operator*() {
		return *_Pos;
	}
	constexpr const _Ty& operator*() const {
		return *_Pos;
	}
	constexpr _Ty* operator->() {
		return _Pos;
	}
	constexpr const _Ty* operator->() const {
		return _Pos;
	}
	constexpr pointer_iterator& operator++() {
		++_Pos;
		return *this;
	}
	constexpr pointer_iterator operator++(int) {
		auto _Tmp = *this;
		++_Pos;
		return _Tmp;
	}
	constexpr pointer_iterator& operator--() {
		--_Pos;
		return *this;
	}
	constexpr pointer_iterator operator--(int) {
		auto _Tmp = *this;
		--_Pos;
		return _Tmp;
	}
	constexpr bool operator==(const pointer_iterator& _Other) const {
		return _Pos == _Other._Pos;
	}
	constexpr bool operator!=(const pointer_iterator& _Other) const {
		return _Pos != _Other._Pos;
	}
	constexpr bool operator>(const pointer_iterator& _Other) const {
		return _Pos > _Other._Pos;
	}
	constexpr bool operator<(const pointer_iterator& _Other) const {
		return _Pos < _Other._Pos;
	}
	constexpr bool operator>=(const pointer_iterator& _Other) const {
		return _Pos >= _Other._Pos;
	}
	constexpr bool operator<=(const pointer_iterator& _Other) const {
		return _Pos <= _Other._Pos;
	}
	constexpr pointer_iterator& operator=(const pointer_iterator& _Other) {
		_Pos = _Other._Pos;
		return *this;
	}
	constexpr pointer_iterator& operator=(pointer_iterator&& _Other) {
		_Pos = _Other._Pos;
		_Other._Pos = nullptr;
		return *this;
	}
	constexpr pointer_iterator operator+(const pointer_iterator& _Other) {
		return pointer_iterator(_Pos + _Other._Pos);
	}
	constexpr pointer_iterator& operator+=(const pointer_iterator& _Other) {
		_Pos += _Other._Pos;
		return *this;
	}
	constexpr pointer_iterator operator-(const pointer_iterator& _Other) {
		return pointer_iterator(_Pos - _Other._Pos);
	}
	constexpr pointer_iterator& operator-=(const pointer_iterator& _Other) {
		_Pos -= _Other._Pos;
		return *this;
	}
	template <class _NumTy>
	constexpr pointer_iterator operator+(const _NumTy& _Other) {
		static_assert(is_numeric<_NumTy>, "Value type must be numeric!");
		return pointer_iterator(static_cast<_Ty*>(_Pos + _Other));
	}
	template <class _NumTy>
	constexpr pointer_iterator& operator+=(const _NumTy& _Other) {
		static_assert(is_numeric<_NumTy>, "Value type must be numeric!");
		_Pos += _Other;
		return *this;
	}
	template <class _NumTy>
	constexpr pointer_iterator operator-(const _NumTy& _Other) {
		static_assert(is_numeric<_NumTy>, "Value type must be numeric!");
		return pointer_iterator(static_cast<_Ty*>(_Pos - _Other));
	}
	template <class _NumTy>
	constexpr pointer_iterator& operator-=(const _NumTy& _Other) {
		static_assert(is_numeric<_NumTy>, "Value type must be numeric!");
		_Pos -= _Other;
		return *this;
	}
	constexpr _Ty* unwrap() {
		return _Pos;
	}
	constexpr const _Ty* unwrap() const {
		return _Pos;
	}
private:
	_Ty* _Pos;
};