#pragma once

#include "Utility.h"
#include <cstdlib>

template<class _Ty>
class Vector
{
public:
	Vector();
	Vector(const Vector<_Ty>& _Vec);
	Vector(Vector<_Ty>&& _Vec);
	~Vector();

	void append(const _Ty& _Val);
	void append(_Ty&& _Val);

	void remove_at(unsigned int _Index);

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	Vector<_Ty>& operator=(const Vector<_Ty>& _Vec);
	Vector<_Ty>& operator=(Vector<_Ty>&& _Vec);

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
inline void Vector<_Ty>::remove_at(unsigned int _Index)
{
	--_Size;
	move_mem(_Data + _Index + 1, _Data + _Index, _Size - _Index);
	_Try_resize(_Get_new_size_2());
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
