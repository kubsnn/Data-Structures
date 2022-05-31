#pragma once

#include "Utility.h"

template<class _Ty>
class Vector
{
public:
	using iterator = _Ty*;
	using const_iterator = const _Ty*;
	
	Vector();
	Vector(size_t _Size);
	Vector(size_t _Size, const _Ty& _Val);
	Vector(const Vector& _Vec);
	Vector(Vector&& _Vec) noexcept;
	~Vector();

	void append(const _Ty& _Val);
	void append(_Ty&& _Val);

	template<class ..._Values>
	void emplace_back(_Values&&... args);
	void append(const Vector& _Vec);
	void append(Vector&& _Vec);
	template <class _FwdIt>
	void append(const _FwdIt _First, const _FwdIt _Last);

	void insert(unsigned int _Index, const _Ty& _Val);
	void insert(unsigned int _Index, _Ty&& _Val);
	template <class ..._Values>
	void emplace(unsigned int _Index, _Values&&... _Vals);


	void remove_at(unsigned int _Index);

	constexpr size_t size() const;

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	void clear();

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	Vector& operator=(const Vector& _Vec);
	Vector& operator=(Vector&& _Vec) noexcept;

	bool operator==(const Vector& _Vec) const;
	constexpr bool operator!=(const Vector& _Vec) const;
	bool operator<(const Vector& _Vec) const;
	constexpr bool operator<=(const Vector& _Vec) const;
	bool operator>(const Vector& _Vec) const;
	constexpr bool operator>=(const Vector& _Vec) const;

private:
	size_t _MaxSize = 8;
	size_t _Size = 0;
	_Ty* _Data;

	template <class ..._Values>
	void _Emplace_back(_Values&& ..._Vals);
	template <class ..._Values>
	void _Emplace(size_t _Where, _Values&& ..._Vals);
	void _Try_resize(size_t _NewSize);
	size_t _Get_new_size() const;
	size_t _Get_new_size_2() const;
	void _Copy_from(const Vector<_Ty>& _Vec);
	void _Clear();
};

template<class _Ty>
inline Vector<_Ty>::Vector()
{
	_MaxSize = 8;
	_Data = new _Ty[_MaxSize];
}

template<class _Ty>
inline Vector<_Ty>::Vector(size_t _Size)
{
	_MaxSize = nearest_bigger_power_of_2(_Size);
	_Data = new _Ty[_MaxSize];
}

template<class _Ty>
inline Vector<_Ty>::Vector(size_t _Size, const _Ty& _Val)
{
	_MaxSize = nearest_bigger_power_of_2(_Size);
	_Data = new _Ty[_MaxSize];
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty>
inline Vector<_Ty>::Vector(const Vector<_Ty>& _Vec)
{
	_Copy_from(_Vec);
}

template<class _Ty>
inline Vector<_Ty>::Vector(Vector<_Ty>&& _Vec) noexcept
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = _Vec._Data;

	_Vec._Data = NULL;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty>
inline Vector<_Ty>::~Vector()
{
	_Clear();
}

template<class _Ty>
inline void Vector<_Ty>::append(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template<class _Ty>
inline void Vector<_Ty>::append(_Ty&& _Val)
{
	_Emplace_back(move<_Ty>(_Val));
}

template<class _Ty>
inline void Vector<_Ty>::append(const Vector<_Ty>& _Vec)
{
	_Try_resize(nearest_bigger_power_of_2(_Size + _Vec._Size));

	copy(_Vec._Data, _Vec._Data + _Vec._Size, _Data + _Size);
	_Size += _Vec._Size;
}

template<class _Ty>
inline void Vector<_Ty>::append(Vector<_Ty>&& _Vec)
{
	_Try_resize(nearest_bigger_power_of_2(_Size + _Vec._Size));

	move_mem(_Vec._Data, _Data + _Size, _Vec._Size);
	_Size += _Vec._Size;

	_Vec._Data = NULL;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty>
template<class ..._Values>
inline void Vector<_Ty>::emplace_back(_Values&&... _Vals)
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty>
template<class _FwdIt>
inline void Vector<_Ty>::append(_FwdIt _First, const _FwdIt _Last)
{
	for (; _First != _Last; ++_First) {
		_Emplace_back(*_First);
	}
}

template<class _Ty>
template<class ..._Values>
inline void Vector<_Ty>::emplace(unsigned int _Index, _Values && ..._Vals)
{
	_Emplace(_Index, forward<_Values>(_Vals)...);
}

template<class _Ty>
inline void Vector<_Ty>::insert(unsigned int _Index, const _Ty& _Val)
{
	_Emplace(_Index, _Val);
}

template<class _Ty>
inline void Vector<_Ty>::insert(unsigned int _Index, _Ty&& _Val)
{
	_Emplace(_Index, move(_Val));
}

template<class _Ty>
inline void Vector<_Ty>::remove_at(unsigned int _Index)
{
	--_Size;
	_Data[_Index].~_Ty();
	move_mem(_Data + _Index + 1, _Data + _Index, _Size - _Index);
	_Try_resize(_Get_new_size_2());
}

template<class _Ty>
inline constexpr size_t Vector<_Ty>::size() const
{
	return _Size;
}

template<class _Ty>
inline constexpr _Ty* Vector<_Ty>::begin()
{
	return _Data;
}

template<class _Ty>
inline constexpr Vector<_Ty>::const_iterator Vector<_Ty>::begin() const
{
	return _Data;
}

template<class _Ty>
inline constexpr _Ty* Vector<_Ty>::end()
{
	return _Data + _Size;
}

template<class _Ty>
inline constexpr Vector<_Ty>::const_iterator Vector<_Ty>::end() const
{
	return _Data + _Size;
}

template<class _Ty>
inline void Vector<_Ty>::clear()
{
	_Clear();
	_MaxSize = 8;
	_Data = new _Ty[_MaxSize];
}

template<class _Ty>
inline _Ty& Vector<_Ty>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty>
inline const _Ty& Vector<_Ty>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}

template<class _Ty>
inline Vector<_Ty>& Vector<_Ty>::operator=(const Vector<_Ty>& _Vec)
{
	_Clear();
	_Copy_from(_Vec);
	return *this;
}

template<class _Ty>
inline Vector<_Ty>& Vector<_Ty>::operator=(Vector<_Ty>&& _Vec) noexcept
{
	_Clear();
	_Size = _Size;
	_MaxSize = _MaxSize;
	_Data = _Vec._Data;

	_Vec._Data = NULL;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
	return *this;
}

template<class _Ty>
inline bool Vector<_Ty>::operator==(const Vector<_Ty>& _Vec) const
{
	if (_Size != _Vec._Size) return false;
	for (int i = 0; i < _Size; ++i) {
		if (_Data[i] != _Vec._Data[i]) return false;
	}
	return true;
}

template<class _Ty>
inline constexpr bool Vector<_Ty>::operator!=(const Vector<_Ty>& _Vec) const
{
	return !(*this == _Vec);
}

template<class _Ty>
inline bool Vector<_Ty>::operator<(const Vector<_Ty>& _Vec) const
{
	return ::lexicographical_compare(begin(), end(), _Vec.begin(), _Vec.end());
}

template<class _Ty>
inline constexpr bool Vector<_Ty>::operator<=(const Vector<_Ty>& _Vec) const
{
	return !(*this > _Vec);
}

template<class _Ty>
inline bool Vector<_Ty>::operator>(const Vector<_Ty>& _Vec) const
{
	return ::lexicographical_compare(_Vec.begin(), _Vec.end(), begin(), end());
}

template<class _Ty>
inline constexpr bool Vector<_Ty>::operator>=(const Vector<_Ty>& _Vec) const
{
	return !(*this < _Vec);
}

template<class _Ty>
template<class ..._Values>
inline void Vector<_Ty>::_Emplace_back(_Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Get_new_size());

	_Data[_Size - 1].~_Ty();
	new(&_Data[_Size - 1]) _Ty(forward<_Values>(_Vals)...);
}

template<class _Ty>
template<class ..._Values>
inline void Vector<_Ty>::_Emplace(size_t _Where, _Values && ..._Vals)
{
	++_Size;
	_Try_resize(_Get_new_size());
	move_mem(_Data + _Where, _Data + _Where + 1, _Size - _Where);
	_Data[_Where].~_Ty();
	new(&_Data[_Where]) _Ty(forward<_Values>(_Vals)...);
}

template<class _Ty>
inline void Vector<_Ty>::_Try_resize(size_t _NewSize)
{
	if (_NewSize == _MaxSize) return;

	auto _Tmp = _Data;
	_Data = new _Ty[_NewSize];
	move_mem(_Tmp, _Data, min(_Size, _NewSize));
 	delete[] _Tmp;

	_MaxSize = _NewSize;
}

template<class _Ty>
inline size_t Vector<_Ty>::_Get_new_size() const
{
	if (_Size >= _MaxSize) return _MaxSize << 1;
	return _MaxSize;
}

template<class _Ty>
inline size_t Vector<_Ty>::_Get_new_size_2() const
{
	if (_MaxSize > 8 && _Size <= _MaxSize / 3) return _MaxSize >> 1;
	return _MaxSize;
}

template<class _Ty>
inline void Vector<_Ty>::_Copy_from(const Vector<_Ty>& _Vec)
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = new _Ty[_MaxSize];
	copy(_Vec._Data, _Vec._Data + _Vec._Size, _Data);
}

template<class _Ty>
inline void Vector<_Ty>::_Clear()
{
	if (!_Data) return;
	_Size = 0;
	_MaxSize = 0;
	delete[] _Data;
	_Data = NULL;
}

