#pragma once

#include "utility.h"
#include "pointer_iterator.h"

template<class _Ty, size_t _Size>
class array
{
public:
	using iterator = pointer_iterator<_Ty>;
	using const_iterator = pointer_iterator<const _Ty>;
	using value_type = _Ty;

	constexpr array();
	constexpr array(const _Ty& _Val_to_fill);

	constexpr size_t size() const;
	constexpr _Ty* data();
	constexpr const _Ty* data() const;
	constexpr void fill(const _Ty& _Val);

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	constexpr _Ty& operator[](unsigned int _Index);
	constexpr const _Ty& operator[](unsigned int _Index) const;
private:
	_Ty _Data[_Size];
};

template<class _Ty, size_t _Size>
inline constexpr array<_Ty, _Size>::array()
{
}

template<class _Ty, size_t _Size>
inline constexpr array<_Ty, _Size>::array(const _Ty& _Val_to_fill)
{
	::fill(_Data, _Data + _Size, _Val_to_fill);
}

template<class _Ty, size_t _Size>
inline constexpr size_t array<_Ty, _Size>::size() const
{
	return _Size;
}

template<class _Ty, size_t _Size>
inline constexpr _Ty* array<_Ty, _Size>::data()
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr const _Ty* array<_Ty, _Size>::data() const
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr void array<_Ty, _Size>::fill(const _Ty& _Val)
{
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty, size_t _Size>
inline constexpr pointer_iterator<_Ty> array<_Ty, _Size>::begin()
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr pointer_iterator<const _Ty> array<_Ty, _Size>::begin() const
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr pointer_iterator<_Ty> array<_Ty, _Size>::end()
{
	return _Data + _Size;
}

template<class _Ty, size_t _Size>
inline constexpr pointer_iterator<const _Ty> array<_Ty, _Size>::end() const
{
	return _Data + _Size;
}

template<class _Ty, size_t _Size>
inline constexpr _Ty& array<_Ty, _Size>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty, size_t _Size>
inline constexpr const _Ty& array<_Ty, _Size>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}