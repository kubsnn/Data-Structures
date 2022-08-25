#pragma once

#include "Utility.h"
#include "memory.h"
#include "pointer_iterator.h"

template<class _Ty, class _Allocator = allocator<_Ty>>
class vector
{
public:
	using iterator = pointer_iterator<_Ty>;
	using const_iterator = pointer_iterator<const _Ty>;
	using value_type = _Ty;

	friend struct iterator;
	friend struct const_iterator;

	constexpr vector();
	constexpr vector(size_t _Size);
	constexpr vector(size_t _Size, const _Ty& _Val);
	constexpr vector(const vector& _Vec);
	constexpr vector(vector&& _Vec) noexcept;
	~vector();

	constexpr void append(const _Ty& _Val);
	constexpr void append(_Ty&& _Val);

	template<class ..._Values>
	constexpr void emplace_back(_Values&&... args);
	constexpr void append(const vector& _Vec);
	constexpr void append(vector&& _Vec);
	template <class _FwdIt>
	constexpr void append(const _FwdIt _First, const _FwdIt _Last);

	constexpr void insert(unsigned int _Index, const _Ty& _Val);
	constexpr void insert(unsigned int _Index, _Ty&& _Val);
	template <class ..._Values>
	constexpr void emplace(unsigned int _Index, _Values&&... _Vals);

	constexpr void remove_at(unsigned int _Index);
	constexpr iterator remove(iterator _Where);

	constexpr size_t size() const;
	constexpr size_t _max_size() const;

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	constexpr void fill(const _Ty& _Val);
	constexpr void reserve(size_t _Count);
	constexpr void shrink_to_fit();
	constexpr void clear();

	constexpr _Ty& operator[](unsigned int _Index);
	constexpr const _Ty& operator[](unsigned int _Index) const;

	constexpr vector& operator=(const vector& _Other);
	constexpr vector& operator=(vector&& _Other) noexcept;

	constexpr bool operator==(const vector& _Vec) const;
	constexpr bool operator!=(const vector& _Vec) const;
	constexpr bool operator<(const vector& _Vec) const;
	constexpr bool operator<=(const vector& _Vec) const;
	constexpr bool operator>(const vector& _Vec) const;
	constexpr bool operator>=(const vector& _Vec) const;

private:
	size_t _MaxSize = 8;
	size_t _Size = 0;
	_Ty* _Data;

	template <class ..._Values>
	constexpr void _Emplace_back(_Values&& ..._Vals);
	template <class ..._Values>
	constexpr void _Emplace(size_t _Where, _Values&& ..._Vals);
	constexpr void _Try_resize(size_t _NewSize);
	constexpr size_t _Get_new_size() const;
	constexpr void _Copy_from(const vector& _Vec);
	constexpr void _Clear();
};

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>::vector()
	: _MaxSize(static_cast<size_t>(8))
{
	_Data = _Allocator::allocate(_MaxSize);
}

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>::vector(size_t _Size)
{
	_MaxSize = _Size > 8 ? nearest_bigger_power_of_2(_Size) : static_cast<size_t>(8);
	this->_Size = _Size;
	_Data = _Allocator::allocate(_MaxSize);
	_Allocator::construct_range(_Data, _Data + _Size);
}

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>::vector(size_t _Size, const _Ty& _Val)
{
	_MaxSize = _Size > 8 ? nearest_bigger_power_of_2(_Size) : static_cast<size_t>(8);
	_Data = _Allocator::allocate(_MaxSize);
	this->_Size = _Size;
	_Fill_in_place(_Data, _Data + _Size, _Val);
}

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>::vector(const vector& _Vec)
{
	_Copy_from(_Vec);
}

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>::vector(vector&& _Vec) noexcept
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
inline constexpr void vector<_Ty, _Allocator>::append(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::append(_Ty&& _Val)
{
	_Emplace_back(move<_Ty>(_Val));
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::append(const vector& _Vec)
{
	_Try_resize(_Size + _Vec._Size);

	_Copy_in_place(_Vec._Data, _Vec._Data + _Vec._Size, _Data + _Size);
	_Size += _Vec._Size;
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::append(vector&& _Vec)
{
	_Try_resize(_Size + _Vec._Size);

	_Move_in_place(_Vec._Data, _Data + _Size, _Vec._Size);
	_Size += _Vec._Size;

	_Vec._Data = NULL;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline constexpr void vector<_Ty, _Allocator>::emplace_back(_Values&&... _Vals)
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
template<class _FwdIt>
inline constexpr void vector<_Ty, _Allocator>::append(_FwdIt _First, const _FwdIt _Last)
{
	for (; _First != _Last; ++_First) {
		_Emplace_back(*_First);
	}
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline constexpr void vector<_Ty, _Allocator>::emplace(unsigned int _Index, _Values && ..._Vals)
{
	_Emplace(_Index, forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::insert(unsigned int _Index, const _Ty& _Val)
{
	_Emplace(_Index, _Val);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::insert(unsigned int _Index, _Ty&& _Val)
{
	_Emplace(_Index, move(_Val));
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::remove_at(unsigned int _Index)
{
	--_Size;
	_Allocator::destroy(_Data[_Index]);
	_Move_in_place(_Data + _Index + 1, _Data + _Index, _Size - _Index);
}

template<class _Ty, class _Allocator>
inline constexpr pointer_iterator<_Ty> vector<_Ty, _Allocator>::remove(iterator _Where)
{
	auto _Ptr = _Where.unwrap();
	size_t _Index = _Ptr - _Data;
	--_Size;
	_Allocator::destroy(_Ptr);
	_Move_in_place(_Data + _Index + 1, _Data + _Index, _Size - _Index);
	return _Where;
}

template<class _Ty, class _Allocator>
inline constexpr size_t vector<_Ty, _Allocator>::size() const
{
	return _Size;
}

template<class _Ty, class _Allocator>
inline constexpr size_t vector<_Ty, _Allocator>::_max_size() const
{
	return _MaxSize;
}

template<class _Ty, class _Allocator>
inline constexpr pointer_iterator<_Ty> vector<_Ty, _Allocator>::begin()
{
	return iterator(_Data);
}

template<class _Ty, class _Allocator>
inline constexpr pointer_iterator<const _Ty> vector<_Ty, _Allocator>::begin() const
{
	return const_iterator(_Data);
}

template<class _Ty, class _Allocator>
inline constexpr pointer_iterator<_Ty> vector<_Ty, _Allocator>::end()
{
	return iterator(_Data + _Size);
}

template<class _Ty, class _Allocator>
inline constexpr pointer_iterator<const _Ty> vector<_Ty, _Allocator>::end() const
{
	return const_iterator(_Data + _Size);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::fill(const _Ty& _Val)
{
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::shrink_to_fit()
{
	size_t _NewSize = _Size > 8 ? nearest_bigger_power_of_2(_Size) : static_cast<size_t>(8);
	auto _NewData = _Allocator::allocate(_NewSize);
	_Move_in_place(_Data, _NewData, _Size);
	_Allocator::deallocate(_Data);
	_Data = _NewData;
	_MaxSize = _NewSize;
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::reserve(size_t _Count)
{
	if (_Count < _MaxSize) return;

	size_t _NewSize = is_power_of_2(_Count) ? _Count : nearest_bigger_power_of_2(_Count);

	auto _NewData = _Allocator::allocate(_NewSize);
	_Move_in_place(_Data, _NewData, _Size);
	_Allocator::deallocate(_Data);
	_Data = _NewData;
	_MaxSize = _NewSize;
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::clear()
{
	_Allocator::destroy_range(_Data, _Data + _Size);
	_Size = 0;
}

template<class _Ty, class _Allocator>
inline constexpr _Ty& vector<_Ty, _Allocator>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty, class _Allocator>
inline constexpr const _Ty& vector<_Ty, _Allocator>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>& vector<_Ty, _Allocator>::operator=(const vector& _Other)
{
	if (this == &_Other) return *this;
	_Clear();
	_Copy_from(_Other);
	return *this;
}

template<class _Ty, class _Allocator>
inline constexpr vector<_Ty, _Allocator>& vector<_Ty, _Allocator>::operator=(vector&& _Other) noexcept
{
	if (this == &_Other) return *this;
	_Clear();
	_Size = _Size;
	_MaxSize = _MaxSize;
	_Data = _Other._Data;

	_Other._Data = nullptr;
	_Other._Size = 0;
	_Other._MaxSize = 0;
	return *this;
}

template<class _Ty, class _Allocator>
inline constexpr bool vector<_Ty, _Allocator>::operator==(const vector& _Vec) const
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
inline constexpr bool vector<_Ty, _Allocator>::operator<(const vector& _Vec) const
{
	return ::lexicographical_compare(begin(), end(), _Vec.begin(), _Vec.end());
}

template<class _Ty, class _Allocator>
inline constexpr bool vector<_Ty, _Allocator>::operator<=(const vector& _Vec) const
{
	return !(*this > _Vec);
}

template<class _Ty, class _Allocator>
inline constexpr bool vector<_Ty, _Allocator>::operator>(const vector& _Vec) const
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
inline constexpr void vector<_Ty, _Allocator>::_Emplace_back(_Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Size);

	_Allocator::construct(&_Data[_Size - 1], forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
template<class ..._Values>
inline constexpr void vector<_Ty, _Allocator>::_Emplace(size_t _Where, _Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Size);
	_Move_in_place(_Data + _Where, _Data + _Where + 1, _Size - _Where - 1);
	_Allocator::construct(&_Data[_Where], forward<_Values>(_Vals)...);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::_Try_resize(size_t _NewSize)
{
	if (_NewSize <= _MaxSize) return;

	size_t _Calculated_size = nearest_bigger_power_of_2(_NewSize);
	_Allocator::reallocate(_Data, _Size, _Calculated_size);

	_MaxSize = _NewSize;
}

template<class _Ty, class _Allocator>
inline constexpr size_t vector<_Ty, _Allocator>::_Get_new_size() const
{
	if (_Size >= _MaxSize) return _MaxSize << 1;
	return _MaxSize;
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::_Copy_from(const vector& _Vec)
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = _Allocator::allocate(_MaxSize);
	_Copy_in_place(_Vec._Data, _Vec._Data + _Vec._Size, _Data);
}

template<class _Ty, class _Allocator>
inline constexpr void vector<_Ty, _Allocator>::_Clear()
{
	if (!_Data) return;
	_Allocator::destroy_range(_Data, _Data + _Size);
	_Allocator::deallocate(_Data);
	_Size = 0;
	_MaxSize = 0;
	_Data = nullptr;
}

