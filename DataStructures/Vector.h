#pragma once

#include "utility.h"
#include "memory.h"
#include "pointer_iterator.h"

template<class _Ty, class _Alloc = allocator<_Ty>>
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

	constexpr void push_back(const _Ty& _Val);
	constexpr void push_back(_Ty&& _Val);

	template<class ..._Values>
	constexpr void emplace_back(_Values&&... args);
	constexpr void push_back(const vector& _Vec);
	constexpr void push_back(vector&& _Vec);
	template <class _FwdIt>
	constexpr void push_back(const _FwdIt _First, const _FwdIt _Last);

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

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>::vector()
	: _MaxSize(static_cast<size_t>(8))
{
	_Data = _Alloc::allocate(_MaxSize);
}

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>::vector(size_t _Size)
{
	_MaxSize = _Size > 8 ? nearest_bigger_power_of_2(_Size) : static_cast<size_t>(8);
	this->_Size = _Size;
	_Data = _Alloc::allocate(_MaxSize);
	_Alloc::construct_range(_Data, _Data + _Size);
}

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>::vector(size_t _Size, const _Ty& _Val)
{
	_MaxSize = _Size > 8 ? nearest_bigger_power_of_2(_Size) : static_cast<size_t>(8);
	_Data = _Alloc::allocate(_MaxSize);
	this->_Size = _Size;
	_Fill_in_place(_Data, _Data + _Size, _Val);
}

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>::vector(const vector& _Vec)
{
	_Copy_from(_Vec);
}

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>::vector(vector&& _Vec) noexcept
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = _Vec._Data;

	_Vec._Data = nullptr;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty, class _Alloc>
inline vector<_Ty, _Alloc>::~vector()
{
	_Clear();
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::push_back(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::push_back(_Ty&& _Val)
{
	_Emplace_back(move<_Ty>(_Val));
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::push_back(const vector& _Vec)
{
	_Try_resize(_Size + _Vec._Size);

	_Copy_in_place(_Vec._Data, _Vec._Data + _Vec._Size, _Data + _Size);
	_Size += _Vec._Size;
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::push_back(vector&& _Vec)
{
	_Try_resize(_Size + _Vec._Size);

	_Move_in_place(_Vec._Data, _Data + _Size, _Vec._Size);
	_Size += _Vec._Size;

	_Vec._Data = NULL;
	_Vec._Size = 0;
	_Vec._MaxSize = 0;
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void vector<_Ty, _Alloc>::emplace_back(_Values&&... _Vals)
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
template<class _FwdIt>
inline constexpr void vector<_Ty, _Alloc>::push_back(_FwdIt _First, const _FwdIt _Last)
{
	for (; _First != _Last; ++_First) {
		_Emplace_back(*_First);
	}
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void vector<_Ty, _Alloc>::emplace(unsigned int _Index, _Values && ..._Vals)
{
	_Emplace(_Index, forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::insert(unsigned int _Index, const _Ty& _Val)
{
	_Emplace(_Index, _Val);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::insert(unsigned int _Index, _Ty&& _Val)
{
	_Emplace(_Index, move(_Val));
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::remove_at(unsigned int _Index)
{
	--_Size;
	_Alloc::destroy(_Data[_Index]);
	_Move_in_place(_Data + _Index + 1, _Data + _Index, _Size - _Index);
}

template<class _Ty, class _Alloc>
inline constexpr pointer_iterator<_Ty> vector<_Ty, _Alloc>::remove(iterator _Where)
{
	auto _Ptr = _Where.unwrap();
	size_t _Index = _Ptr - _Data;
	--_Size;
	_Alloc::destroy(_Ptr);
	_Move_in_place(_Data + _Index + 1, _Data + _Index, _Size - _Index);
	return _Where;
}

template<class _Ty, class _Alloc>
inline constexpr size_t vector<_Ty, _Alloc>::size() const
{
	return _Size;
}

template<class _Ty, class _Alloc>
inline constexpr size_t vector<_Ty, _Alloc>::_max_size() const
{
	return _MaxSize;
}

template<class _Ty, class _Alloc>
inline constexpr pointer_iterator<_Ty> vector<_Ty, _Alloc>::begin()
{
	return iterator(_Data);
}

template<class _Ty, class _Alloc>
inline constexpr pointer_iterator<const _Ty> vector<_Ty, _Alloc>::begin() const
{
	return const_iterator(_Data);
}

template<class _Ty, class _Alloc>
inline constexpr pointer_iterator<_Ty> vector<_Ty, _Alloc>::end()
{
	return iterator(_Data + _Size);
}

template<class _Ty, class _Alloc>
inline constexpr pointer_iterator<const _Ty> vector<_Ty, _Alloc>::end() const
{
	return const_iterator(_Data + _Size);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::fill(const _Ty& _Val)
{
	::fill(_Data, _Data + _Size, _Val);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::shrink_to_fit()
{
	size_t _NewSize = _Size > 8 ? nearest_bigger_power_of_2(_Size) : static_cast<size_t>(8);
	auto _NewData = _Alloc::allocate(_NewSize);
	_Move_in_place(_Data, _NewData, _Size);
	_Alloc::deallocate(_Data);
	_Data = _NewData;
	_MaxSize = _NewSize;
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::reserve(size_t _Count)
{
	if (_Count < _MaxSize) return;

	size_t _NewSize = is_power_of_2(_Count) ? _Count : nearest_bigger_power_of_2(_Count);

	auto _NewData = _Alloc::allocate(_NewSize);
	_Move_in_place(_Data, _NewData, _Size);
	_Alloc::deallocate(_Data);
	_Data = _NewData;
	_MaxSize = _NewSize;
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::clear()
{
	_Alloc::destroy_range(_Data, _Data + _Size);
	_Size = 0;
}

template<class _Ty, class _Alloc>
inline constexpr _Ty& vector<_Ty, _Alloc>::operator[](unsigned int _Index)
{
	return _Data[_Index];
}

template<class _Ty, class _Alloc>
inline constexpr const _Ty& vector<_Ty, _Alloc>::operator[](unsigned int _Index) const
{
	return _Data[_Index];
}

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>& vector<_Ty, _Alloc>::operator=(const vector& _Other)
{
	if (this == &_Other) return *this;
	_Clear();
	_Copy_from(_Other);
	return *this;
}

template<class _Ty, class _Alloc>
inline constexpr vector<_Ty, _Alloc>& vector<_Ty, _Alloc>::operator=(vector&& _Other) noexcept
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

template<class _Ty, class _Alloc>
inline constexpr bool vector<_Ty, _Alloc>::operator==(const vector& _Vec) const
{
	if (_Size != _Vec._Size) return false;
	for (int i = 0; i < _Size; ++i) {
		if (_Data[i] != _Vec._Data[i]) return false;
	}
	return true;
}

template<class _Ty, class _Alloc>
inline constexpr bool vector<_Ty, _Alloc>::operator!=(const vector& _Vec) const
{
	return !(*this == _Vec);
}

template<class _Ty, class _Alloc>
inline constexpr bool vector<_Ty, _Alloc>::operator<(const vector& _Vec) const
{
	return ::lexicographical_compare(begin(), end(), _Vec.begin(), _Vec.end());
}

template<class _Ty, class _Alloc>
inline constexpr bool vector<_Ty, _Alloc>::operator<=(const vector& _Vec) const
{
	return !(*this > _Vec);
}

template<class _Ty, class _Alloc>
inline constexpr bool vector<_Ty, _Alloc>::operator>(const vector& _Vec) const
{
	return ::lexicographical_compare(_Vec.begin(), _Vec.end(), begin(), end());
}

template<class _Ty, class _Alloc>
inline constexpr bool vector<_Ty, _Alloc>::operator>=(const vector& _Vec) const
{
	return !(*this < _Vec);
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void vector<_Ty, _Alloc>::_Emplace_back(_Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Size);

	_Alloc::construct(&_Data[_Size - 1], forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void vector<_Ty, _Alloc>::_Emplace(size_t _Where, _Values&& ..._Vals)
{
	++_Size;
	_Try_resize(_Size);
	_Move_in_place(_Data + _Where, _Data + _Where + 1, _Size - _Where - 1);
	_Alloc::construct(&_Data[_Where], forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::_Try_resize(size_t _NewSize)
{
	if (_NewSize <= _MaxSize) return;

	size_t _Calculated_size = nearest_bigger_power_of_2(_NewSize);
	_Alloc::reallocate(_Data, _Size, _Calculated_size);

	_MaxSize = _NewSize;
}

template<class _Ty, class _Alloc>
inline constexpr size_t vector<_Ty, _Alloc>::_Get_new_size() const
{
	if (_Size >= _MaxSize) return _MaxSize << 1;
	return _MaxSize;
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::_Copy_from(const vector& _Vec)
{
	_Size = _Vec._Size;
	_MaxSize = _Vec._MaxSize;
	_Data = _Alloc::allocate(_MaxSize);
	_Copy_in_place(_Vec._Data, _Vec._Data + _Vec._Size, _Data);
}

template<class _Ty, class _Alloc>
inline constexpr void vector<_Ty, _Alloc>::_Clear()
{
	if (!_Data) return;
	_Alloc::destroy_range(_Data, _Data + _Size);
	_Alloc::deallocate(_Data);
	_Size = 0;
	_MaxSize = 0;
	_Data = nullptr;
}

#ifdef __APPLY_HPP__

namespace pipeline {
	struct to_vector_fn
	{
		template <class _Range>
		constexpr auto operator()(_Range&& _Rng) const {
			size_t _Size = distance(_Rng.begin(), _Rng.end());

			vector<decltype(*_Rng.begin())> _Vec;
			_Vec.reserve(_Size);
			
			for (auto&& e : _Rng) {
				_Vec.emplace_back(move(e));
			}

			return _Vec;
		}
	};
	constexpr to_vector_fn to_vector = {};
}
#endif // __APPLY_HPP__
