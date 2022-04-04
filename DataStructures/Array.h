#pragma once

#include "Utility.h"

template<class _Ty, size_t _Size>
class Array
{
public:
	constexpr const size_t size() const;
	constexpr _Ty* data();
	constexpr const _Ty* data() const;
	constexpr void fill(const _Ty& _Val);

	constexpr _Ty* begin();
	constexpr const _Ty* begin() const;
	constexpr _Ty* end();
	constexpr const _Ty* end() const;

	constexpr _Ty& operator[](unsigned int _Index);
	constexpr const _Ty& operator[](unsigned int _Index) const;
private:
	_Ty _Data[_Size];
};

template<class _Ty, size_t _Size>
inline constexpr const size_t Array<_Ty, _Size>::size() const
{
	return _Size;
}

template<class _Ty, size_t _Size>
inline constexpr _Ty* Array<_Ty, _Size>::data()
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr const _Ty* Array<_Ty, _Size>::data() const
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr void Array<_Ty, _Size>::fill(const _Ty& _Val)
{
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty, size_t _Size>
inline constexpr _Ty* Array<_Ty, _Size>::begin()
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr const _Ty* Array<_Ty, _Size>::begin() const
{
	return _Data;
}

template<class _Ty, size_t _Size>
inline constexpr _Ty* Array<_Ty, _Size>::end()
{
	return _Data + _Size;
}

template<class _Ty, size_t _Size>
inline constexpr const _Ty* Array<_Ty, _Size>::end() const
{
	return _Data + _Size;
}

template<class _Ty, size_t _Size>
inline constexpr _Ty& Array<_Ty, _Size>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty, size_t _Size>
inline constexpr const _Ty& Array<_Ty, _Size>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}