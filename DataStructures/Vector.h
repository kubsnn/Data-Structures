#pragma once

#include "Utility.h"
#include "memory.h"

template<class _Ty, class _Allocator = allocator<_Ty>>
class vector
{
public:
	using iterator = _Ty*;
	using const_iterator = const _Ty*;

	vector();
	vector(size_t _Size);
	vector(size_t _Size, const _Ty& _Val);
	vector(const vector& _Vec);
	vector(vector&& _Vec) noexcept;
	~vector();

	void append(const _Ty& _Val);
	void append(_Ty&& _Val);

	template<class ..._Values>
	void emplace_back(_Values&&... args);
	void append(const vector& _Vec);
	void append(vector&& _Vec);
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

	void fill(const _Ty& _Val);

	void clear();

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	vector& operator=(const vector& _Vec);
	vector& operator=(vector&& _Vec) noexcept;

	bool operator==(const vector& _Vec) const;
	constexpr bool operator!=(const vector& _Vec) const;
	bool operator<(const vector& _Vec) const;
	constexpr bool operator<=(const vector& _Vec) const;
	bool operator>(const vector& _Vec) const;
	constexpr bool operator>=(const vector& _Vec) const;

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
	void _Copy_from(const vector& _Vec);
	void _Clear();
};

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>::vector()
{
	_MaxSize = 8;
	_Data = _Allocator::allocate(_MaxSize);
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>::vector(size_t _Size)
{
	_MaxSize = nearest_bigger_power_of_2(_Size);
	this->_Size = _Size;
	_Data = _Allocator::allocate(_MaxSize);
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>::vector(size_t _Size, const _Ty& _Val)
{
	_MaxSize = nearest_bigger_power_of_2(_Size);
	_Data = _Allocator::allocate(_MaxSize);
	this->_Size = _Size;
	_Fill_in_place(_Data, _Data + _Size, _Val);
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>::vector(const vector& _Vec)
{
	_Copy_from(_Vec);
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>::vector(vector&& _Vec) noexcept
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = _Vec._Data;

	_Vec._Data = nullptr;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>::~vector()
{
	_Clear();
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::append(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::append(_Ty&& _Val)
{
	_Emplace_back(move<_Ty>(_Val));
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::append(const vector& _Vec)
{
	_Try_resize(nearest_bigger_power_of_2(_Size + _Vec._Size));

	_Copy_in_place(_Vec._Data, _Vec._Data + _Vec._Size, _Data + _Size);
	_Size += _Vec._Size;
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::append(vector&& _Vec)
{
	_Try_resize(nearest_bigger_power_of_2(_Size + _Vec._Size));

	_Move_in_place(_Vec._Data, _Data + _Size, _Vec._Size);
	_Size += _Vec._Size;

	_Vec._Data = NULL;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline void vector<_Ty, _Allocator>::emplace_back(_Values&&... _Vals)
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
template<class _FwdIt>
inline void vector<_Ty, _Allocator>::append(_FwdIt _First, const _FwdIt _Last)
{
	for (; _First != _Last; ++_First) {
		_Emplace_back(*_First);
	}
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline void vector<_Ty, _Allocator>::emplace(unsigned int _Index, _Values && ..._Vals)
{
	_Emplace(_Index, forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::insert(unsigned int _Index, const _Ty& _Val)
{
	_Emplace(_Index, _Val);
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::insert(unsigned int _Index, _Ty&& _Val)
{
	_Emplace(_Index, move(_Val));
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::remove_at(unsigned int _Index)
{
	--_Size;
	_Allocator::destroy(_Data[_Index]);
	_Move_in_place(_Data + _Index + 1, _Data + _Index, _Size - _Index);
	_Try_resize(_Get_new_size_2());
}

template<class _Ty, class _Allocator>
inline constexpr size_t vector<_Ty, _Allocator>::size() const
{
	return _Size;
}

template<class _Ty, class _Allocator>
inline constexpr _Ty* vector<_Ty, _Allocator>::begin()
{
	return _Data;
}

template<class _Ty, class _Allocator>
inline constexpr const _Ty* vector<_Ty, _Allocator>::begin() const
{
	return _Data;
}

template<class _Ty, class _Allocator>
inline constexpr _Ty* vector<_Ty, _Allocator>::end()
{
	return _Data + _Size;
}

template<class _Ty, class _Allocator>
inline constexpr const _Ty* vector<_Ty, _Allocator>::end() const
{
	return _Data + _Size;
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::fill(const _Ty& _Val)
{
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::clear()
{
	_Clear();
	_MaxSize = 8;
	_Data = _Allocator::allocate(_MaxSize);
}

template<class _Ty, class _Allocator>
inline _Ty& vector<_Ty, _Allocator>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty, class _Allocator>
inline const _Ty& vector<_Ty, _Allocator>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>& vector<_Ty, _Allocator>::operator=(const vector& _Vec)
{
	_Clear();
	_Copy_from(_Vec);
	return *this;
}

template<class _Ty, class _Allocator>
inline vector<_Ty, _Allocator>& vector<_Ty, _Allocator>::operator=(vector&& _Vec) noexcept
{
	_Clear();
	_Size = _Size;
	_MaxSize = _MaxSize;
	_Data = _Vec._Data;

	_Vec._Data = nullptr;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
	return *this;
}

template<class _Ty, class _Allocator>
inline bool vector<_Ty, _Allocator>::operator==(const vector& _Vec) const
{
	if (_Size != _Vec._Size) return false;
	for (int i = 0; i < _Size; ++i) {
		if (_Data[i] != _Vec._Data[i]) return false;
	}
	return true;
}

template<class _Ty, class _Allocator>
inline constexpr bool vector<_Ty, _Allocator>::operator!=(const vector& _Vec) const
{
	return !(*this == _Vec);
}

template<class _Ty, class _Allocator>
inline bool vector<_Ty, _Allocator>::operator<(const vector& _Vec) const
{
	return ::lexicographical_compare(begin(), end(), _Vec.begin(), _Vec.end());
}

template<class _Ty, class _Allocator>
inline constexpr bool vector<_Ty, _Allocator>::operator<=(const vector& _Vec) const
{
	return !(*this > _Vec);
}

template<class _Ty, class _Allocator>
inline bool vector<_Ty, _Allocator>::operator>(const vector& _Vec) const
{
	return ::lexicographical_compare(_Vec.begin(), _Vec.end(), begin(), end());
}

template<class _Ty, class _Allocator>
inline constexpr bool vector<_Ty, _Allocator>::operator>=(const vector& _Vec) const
{
	return !(*this < _Vec);
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline void vector<_Ty, _Allocator>::_Emplace_back(_Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Get_new_size());

	_Allocator::construct(&_Data[_Size - 1], forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline void vector<_Ty, _Allocator>::_Emplace(size_t _Where, _Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Get_new_size());
	_Move_in_place(_Data + _Where, _Data + _Where + 1, _Size - _Where - 1);
	_Allocator::construct(&_Data[_Where], forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::_Try_resize(size_t _NewSize)
{
	if (_NewSize == _MaxSize) return;

	_Allocator::reallocate(_Data, _Size, _NewSize);

	_MaxSize = _NewSize;
}

template<class _Ty, class _Allocator>
inline size_t vector<_Ty, _Allocator>::_Get_new_size() const
{
	if (_Size >= _MaxSize) return _MaxSize << 1;
	return _MaxSize;
}

template<class _Ty, class _Allocator>
inline size_t vector<_Ty, _Allocator>::_Get_new_size_2() const
{
	if (_MaxSize > 8 && _Size <= _MaxSize / 3) return _MaxSize >> 1;
	return _MaxSize;
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::_Copy_from(const vector& _Vec)
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = _Allocator::allocate(_MaxSize);
	_Copy_in_place(_Vec._Data, _Vec._Data + _Vec._Size, _Data);
}

template<class _Ty, class _Allocator>
inline void vector<_Ty, _Allocator>::_Clear()
{
	if (!_Data) return;
	_Allocator::destroy_range(_Data, _Data + _Size);
	_Allocator::deallocate(_Data);
	_Size = 0;
	_MaxSize = 0;
	_Data = nullptr;
}