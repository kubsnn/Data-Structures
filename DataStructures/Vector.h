#pragma once

#include "Utility.h"

template<class _Ty>
class Vector
{
public:
	Vector();
	Vector(size_t _Size);
	Vector(const Vector<_Ty>& _Vec);
	Vector(Vector<_Ty>&& _Vec);
	~Vector();

	void append(const _Ty& _Val);
	void append(_Ty&& _Val);

	void insert(unsigned int _Index, const _Ty& _Val);
	void insert(unsigned int _Index, _Ty&& _Val);

	void remove_at(unsigned int _Index);

	constexpr size_t size() const;

	constexpr _Ty* begin();
	constexpr const _Ty* begin() const;
	constexpr _Ty* end();
	constexpr const _Ty* end() const;

	void clear();

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	Vector<_Ty>& operator=(const Vector<_Ty>& _Vec);
	Vector<_Ty>& operator=(Vector<_Ty>&& _Vec);

	bool operator==(const Vector<_Ty>& _Vec) const;
	bool operator!=(const Vector<_Ty>& _Vec) const;
	

private:
	size_t _MaxSize = 8;
	size_t _Size = 0;
	_Ty* _Data;

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
inline Vector<_Ty>::Vector(const Vector<_Ty>& _Vec)
{
	_Copy_from(_Vec);
}

template<class _Ty>
inline Vector<_Ty>::Vector(Vector<_Ty>&& _Vec)
{
	_Size = move(_Size);
	_MaxSize = move(_MaxSize);
	_Data = move(_Vec._Data);

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
	++_Size;
	_Try_resize(_Get_new_size());

	_Data[_Size - 1] = _Val;
}

template<class _Ty>
inline void Vector<_Ty>::append(_Ty&& _Val)
{
	++_Size;
	_Try_resize(_Get_new_size());

	_Data[_Size - 1] = move(_Val);
}

template<class _Ty>
inline void Vector<_Ty>::insert(unsigned int _Index, const _Ty& _Val)
{
	++_Size;
	_Try_resize(_Get_new_size());
	move_mem(_Data + _Index, _Data + _Index + 1, _Size - _Index);
	_Data[_Index] = _Val;
}

template<class _Ty>
inline void Vector<_Ty>::insert(unsigned int _Index, _Ty&& _Val)
{
	++_Size;
	_Try_resize(_Get_new_size());
	move_mem(_Data + _Index, _Data + _Index + 1, _Size - _Index);
	_Data[_Index] = move(_Val);
}

template<class _Ty>
inline void Vector<_Ty>::remove_at(unsigned int _Index)
{
	--_Size;
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
inline constexpr const _Ty* Vector<_Ty>::begin() const
{
	return _Data;
}

template<class _Ty>
inline constexpr _Ty* Vector<_Ty>::end()
{
	return _Data + _Size;
}

template<class _Ty>
inline constexpr const _Ty* Vector<_Ty>::end() const
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
inline Vector<_Ty>& Vector<_Ty>::operator=(Vector<_Ty>&& _Vec)
{
	_Clear();
	_Size = move(_Size);
	_MaxSize = move(_MaxSize);
	_Data = move(_Vec._Data);

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
inline bool Vector<_Ty>::operator!=(const Vector<_Ty>& _Vec) const
{
	return !(*this == _Vec);
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
	if (_Size == _MaxSize) return _MaxSize << 1;
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
	delete[] _Data;
	_Data = NULL;
	_MaxSize = 0;
	_Size = 0;
}
