#pragma once

#include "utility.h"
#include "pointer_iterator.h"


template <class _Ty>
class dynarray
{
public:
	using iterator = pointer_iterator<_Ty>;
	using const_iterator = pointer_iterator<const _Ty>;
	using value_type = _Ty;

	constexpr dynarray();
	constexpr dynarray(size_t _Array_size);
	constexpr dynarray(size_t _Array_size, const _Ty& _Val);
	constexpr dynarray(const dynarray& _Other);
	constexpr dynarray(dynarray&& _Other);

	constexpr _Ty& operator[](unsigned int _Index);
	constexpr const _Ty& operator[](unsigned int _Index) const;

	constexpr void fill(const _Ty& _Val);
	constexpr void resize(size_t _New_size);

	constexpr size_t size() const;

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	constexpr _Ty* data();
	constexpr const _Ty* data() const;

	constexpr dynarray& operator=(const dynarray& _Other);
	constexpr dynarray& operator=(dynarray&& _Other);
private:
	_Ty* _Data;
	size_t _Size;
};

template<class _Ty>
inline constexpr dynarray<_Ty>::dynarray()
{
	_Size = 0;
	_Data = new _Ty[0];
}

template<class _Ty>
inline constexpr dynarray<_Ty>::dynarray(size_t _Array_size)
{
	_Size = _Array_size;
	_Data = new _Ty[_Size];
}

template<class _Ty>
inline constexpr dynarray<_Ty>::dynarray(size_t _Array_size, const _Ty& _Val)
{
	_Size = _Array_size;
	_Data = new _Ty[_Size];
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty>
inline constexpr dynarray<_Ty>::dynarray(const dynarray& _Other)
{
	_Size = _Other._Size;
	_Data = new _Ty[_Size];
	::copy(_Other._Data, _Other._Data + _Size, _Data);
}

template<class _Ty>
inline constexpr dynarray<_Ty>::dynarray(dynarray&& _Other)
{
	_Size = _Other._Size;
	_Data = _Other._Data;
	_Other._Data = nullptr;
}

template<class _Ty>
inline constexpr _Ty& dynarray<_Ty>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty>
inline constexpr const _Ty& dynarray<_Ty>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}

template<class _Ty>
inline constexpr void dynarray<_Ty>::fill(const _Ty& _Val)
{
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty>
inline constexpr void dynarray<_Ty>::resize(size_t _New_size)
{
	::reallocate(_Data, _Size, _New_size);
	_Size = _New_size;
}

template<class _Ty>
inline constexpr size_t dynarray<_Ty>::size() const
{
	return _Size;
}

template<class _Ty>
inline constexpr pointer_iterator<_Ty> dynarray<_Ty>::begin()
{
	return _Data;
}

template<class _Ty>
inline constexpr pointer_iterator<const _Ty> dynarray<_Ty>::begin() const
{
	return _Data;
}

template<class _Ty>
inline constexpr pointer_iterator<_Ty> dynarray<_Ty>::end()
{
	return _Data + _Size;
}

template<class _Ty>
inline constexpr pointer_iterator<const _Ty> dynarray<_Ty>::end() const
{
	return _Data + _Size;
}

template<class _Ty>
inline constexpr _Ty* dynarray<_Ty>::data()
{
	return _Data;
}

template<class _Ty>
inline constexpr const _Ty* dynarray<_Ty>::data() const
{
	return _Data;
}

template<class _Ty>
inline constexpr dynarray<_Ty>& dynarray<_Ty>::operator=(const dynarray& _Other)
{
	_Size = _Other._Size;
	delete[] _Data;
	_Data = new _Ty[_Size];
	::copy(_Other._Data, _Other._Data + _Other._Size, _Data);
}

template<class _Ty>
inline constexpr dynarray<_Ty>& dynarray<_Ty>::operator=(dynarray&& _Other)
{
	_Size = _Other._Size;
	delete[] _Data;
	_Data = _Other._Data;
	_Other._Data = nullptr;
}
