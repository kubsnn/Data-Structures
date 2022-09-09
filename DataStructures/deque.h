#pragma once

#include "utility.h"
#include "memory.h"

template <class _Ty>
struct _Deque_iterator;

template <class _Ty, class _Alloc = allocator<_Ty>>
class deque
{
public:
	using iterator = _Deque_iterator<remove_const_t<_Ty>>;
	using const_iterator = _Deque_iterator<const _Ty>;
	using value_type = _Ty;

	friend struct iterator;
	friend struct const_iterator;

	constexpr deque();
	constexpr deque(const deque& _Other);
	constexpr deque(deque&& _Other) noexcept;
	constexpr ~deque();

	constexpr void push_back(const _Ty& _Val);
	constexpr void push_back(_Ty&& _Val);
	template <class... _Values>
	constexpr void emplace_back(_Values&&... _Vals);

	constexpr void push_front(const _Ty& _Val);
	constexpr void push_front(_Ty&& _Val);
	template <class... _Values>
	constexpr void emplace_front(_Values&&... _Vals);

	constexpr _Ty pop_back();
	constexpr _Ty pop_front();

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	constexpr size_t size() const;
	constexpr bool empty() const;
	constexpr _Ty& back();
	constexpr const _Ty& back() const;
	constexpr _Ty& front();
	constexpr const _Ty& front() const;
	
	constexpr _Ty& operator[](unsigned int _Index);
	constexpr const _Ty& operator[](unsigned int _Index) const;

	constexpr deque& operator=(const deque& _Other);
	constexpr deque& operator=(deque&& _Other) noexcept;
protected:
	using _Block_al = allocator<_Ty*>;

	static constexpr size_t _Block_size = max(static_cast<size_t>(32 / sizeof(_Ty)), static_cast<size_t>(2));

	template <class... _Values>
	constexpr void _Emplace_back(_Values&&... _Vals);
	template <class... _Values>
	constexpr void _Emplace_front(_Values&&... _Vals);
private:
	template <class... _Values>
	constexpr void _Emplace_first_el(_Values&&... _Vals);
	constexpr void _Extend_back();
	constexpr void _Extend_front();
	constexpr void _Copy(const deque& _Other);
	constexpr void _Move(deque&& _Other) noexcept;
	constexpr void _Clear();

	_Ty** _Blocks;
	iterator _Begin;
	iterator _End;
	size_t _Block_count;
	size_t _Size;
};


template <class _Ty>
struct _Deque_iterator
{
	using category = random_access_iterator;
	friend class deque<remove_const_t<_Ty>>;
	friend class deque<const _Ty>;

	friend struct _Deque_iterator<remove_const_t<_Ty>>;
	friend struct _Deque_iterator<const remove_const_t<_Ty>>;
private:
	static constexpr size_t _Block_size = deque<_Ty>::_Block_size;

public:
	constexpr _Deque_iterator() = default;
	constexpr _Deque_iterator(const _Deque_iterator<const remove_const_t<_Ty>>& _Other) 
		: _Block(_Other._Block), _Ptr(_Other._Ptr)
	{ }
	constexpr _Deque_iterator(_Deque_iterator&&) = default;
	constexpr _Deque_iterator(const _Ty** const& _B, const _Ty* const& _P)
		: _Block(_B), _Ptr(_P)
	{ }
	constexpr _Deque_iterator(const _Deque_iterator<remove_const_t<_Ty>>& _Other)
		: _Block(const_cast<_Ty**>(_Other._Block)), _Ptr(_Other._Ptr)
	{ }
	constexpr _Deque_iterator& operator=(const _Deque_iterator& _Other) = default;
	constexpr _Deque_iterator& operator=(_Deque_iterator&& _Other) = default;
	
	constexpr _Ty& operator*() {
		return *_Ptr;
	}
	constexpr const _Ty& operator*() const {
		return *_Ptr;
	}
	constexpr _Deque_iterator& operator++() {
		if (_Is_last_in_block()) {
			++_Block;
			_Ptr = *_Block;
		} else {
			++_Ptr;
		}
		return *this;
	}
	constexpr _Deque_iterator operator++(int) {
		auto _Tmp = *this;
		++*this;
		return _Tmp;
	}
	constexpr _Deque_iterator& operator--() {
		if (_Is_first_in_block()) {
			--_Block;
			_Ptr = *_Block + (_Block_size - 1);
		}
		else {
			--_Ptr;
		}
		return *this;
	}
	constexpr _Deque_iterator operator--(int) {
		auto _Tmp = *this;
		--*this;
		return _Tmp;
	}
	template <class _NumTy>
	constexpr _Deque_iterator& operator+=(const _NumTy& _Offset) {
		static_assert(is_numeric<_NumTy>, "Value type must be numeric!");
		if (_Offset == 0) {
			return *this;
		}
		const auto _Block_offset = _Offset / _Block_size;
		const auto _In_block_offset = _Offset % _Block_size;
		_Block += _Block_offset;
		_Ptr += _In_block_offset;
		if (_Ptr >= *_Block + _Block_size) {
			_Ptr -= _Block_size;
			++_Block;
		}
		return *this;
	}
	template <class _NumTy>
	constexpr _Deque_iterator& operator-=(const _NumTy& _Offset) {
		static_assert(is_numeric<_NumTy>, "Value type must be numeric!");
		if (_Offset == 0) {
			return *this;
		}
		const auto _Block_offset = _Offset / _Block_size;
		const auto _In_block_offset = _Offset % _Block_size;
		_Block -= _Block_offset;
		_Ptr -= _In_block_offset;
		if (_Ptr < *_Block) {
			_Ptr += _Block_size;
			--_Block;
		}
		return *this;
	}
	template <class _NumTy>
	constexpr _Deque_iterator operator+(const _NumTy& _Offset) const {
		auto _Tmp = *this;
		_Tmp += _Offset;
		return _Tmp;
	}
	template <class _NumTy>
	constexpr _Deque_iterator operator-(const _NumTy& _Offset) const {
		auto _Tmp = *this;
		_Tmp -= _Offset;
		return _Tmp;
	}
	constexpr int operator-(const _Deque_iterator& _Other) const {
		auto _Diff_1 = _Ptr - *_Block;
		auto _Diff_2 = _Other._Ptr - *_Other._Block;
		return (_Block - _Other._Block) * _Block_size + (_Diff_1 - _Diff_2);
	}
	constexpr _Ty& operator[](unsigned int _Index) {
		size_t _Offset = (_Ptr - *_Block);
		return _Block[_Index / _Block_size + _Offset][_Index & (_Block_size - 1)];
	}
	constexpr const _Ty& operator[](unsigned int _Index) const {
		size_t _Offset = (_Ptr - *_Block);
		return _Block[_Index / _Block_size + _Offset][_Index & (_Block_size - 1)];
	}
	constexpr bool operator==(const _Deque_iterator& _Other) const {
		return _Ptr == _Other._Ptr;
	}
	constexpr bool operator!=(const _Deque_iterator& _Other) const {
		return _Ptr != _Other._Ptr;
	}
	constexpr _Deque_iterator unwrap() const {
		return *this;
	}
private:
	constexpr bool _Is_last_in_block() const {
		return _Ptr == *_Block + (_Block_size - 1);
	}
	constexpr bool _Is_first_in_block() const {
		return _Ptr == *_Block;
	}

	_Ty** _Block = nullptr;
	_Ty* _Ptr = nullptr;
};

template<class _Ty, class _Alloc>
inline constexpr deque<_Ty, _Alloc>::deque()
	: _Block_count(4), _Size(0)
{
	_Blocks = _Block_al::allocate(_Block_count);
	_Blocks[1] = _Alloc::allocate(_Block_size);
}

template<class _Ty, class _Alloc>
inline constexpr deque<_Ty, _Alloc>::deque(const deque& _Other)
{
	if (this == &_Other) return;
	_Copy(_Other);
}

template<class _Ty, class _Alloc>
inline constexpr deque<_Ty, _Alloc>::deque(deque&& _Other) noexcept
{
	if (this == &_Other) return;
	_Move(move(_Other));
}

template<class _Ty, class _Alloc>
inline constexpr deque<_Ty, _Alloc>::~deque()
{
	if (!_Blocks) return;
	_Clear();
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::push_back(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::push_back(_Ty&& _Val)
{
	_Emplace_back(move(_Val));
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void deque<_Ty, _Alloc>::emplace_back(_Values&&... _Vals)
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::push_front(const _Ty& _Val)
{
	_Emplace_front(_Val);
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::push_front(_Ty&& _Val)
{
	_Emplace_front(move(_Val));
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void deque<_Ty, _Alloc>::emplace_front(_Values&&... _Vals)
{
	_Emplace_front(forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
inline constexpr _Ty deque<_Ty, _Alloc>::pop_back()
{
	_Ty _Val = move(*--_End);
	_Alloc::destroy(*_End._Ptr);
	--_Size;
	return _Val;
}

template<class _Ty, class _Alloc>
inline constexpr _Ty deque<_Ty, _Alloc>::pop_front()
{
	_Ty _Val = move(*_Begin);
	_Alloc::destroy(*_Begin._Ptr);
	++_Begin;
	--_Size;
	return _Val;
}

template<class _Ty, class _Alloc>
inline constexpr _Deque_iterator<remove_const_t<_Ty>> deque<_Ty, _Alloc>::begin()
{
	return _Begin;
}

template<class _Ty, class _Alloc>
inline constexpr _Deque_iterator<const _Ty> deque<_Ty, _Alloc>::begin() const
{
	return const_iterator(_Begin);
}

template<class _Ty, class _Alloc>
inline constexpr _Deque_iterator<remove_const_t<_Ty>> deque<_Ty, _Alloc>::end()
{
	return _End;
}

template<class _Ty, class _Alloc>
inline constexpr _Deque_iterator<const _Ty> deque<_Ty, _Alloc>::end() const
{
	return const_iterator(_End);
}

template<class _Ty, class _Alloc>
inline constexpr size_t deque<_Ty, _Alloc>::size() const
{
	return _Size;
}

template<class _Ty, class _Alloc>
inline constexpr bool deque<_Ty, _Alloc>::empty() const
{
	return _Size == 0;
}

template<class _Ty, class _Alloc>
inline constexpr _Ty& deque<_Ty, _Alloc>::back()
{
	return *(_End - 1);
}

template<class _Ty, class _Alloc>
inline constexpr const _Ty& deque<_Ty, _Alloc>::back() const
{
	return *(_End - 1);
}

template<class _Ty, class _Alloc>
inline constexpr _Ty& deque<_Ty, _Alloc>::front()
{
	return *_Begin;
}

template<class _Ty, class _Alloc>
inline constexpr const _Ty& deque<_Ty, _Alloc>::front() const
{
	return *_Begin;
}

template<class _Ty, class _Alloc>
inline constexpr _Ty& deque<_Ty, _Alloc>::operator[](unsigned int _Index)
{
	size_t _Offset = (_Begin._Ptr - *_Begin._Block);
	return _Begin._Block[_Index / _Block_size + _Offset][_Index & (_Block_size - 1)];
}

template<class _Ty, class _Alloc>
inline constexpr const _Ty& deque<_Ty, _Alloc>::operator[](unsigned int _Index) const
{
	size_t _Offset = (_Begin._Ptr - *_Begin._Block);
	return _Begin._Block[_Index / _Block_size + _Offset][_Index & (_Block_size - 1)];
}

template<class _Ty, class _Alloc>
inline constexpr deque<_Ty, _Alloc>& deque<_Ty, _Alloc>::operator=(const deque& _Other)
{
	if (this == &_Other) return *this;
	_Clear();
	_Copy(_Other);
	return *this;
}

template<class _Ty, class _Alloc>
inline constexpr deque<_Ty, _Alloc>& deque<_Ty, _Alloc>::operator=(deque&& _Other) noexcept
{
	if (this == &_Other) return *this;
	_Clear();
	_Move(_Other);
	return *this;
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::_Extend_back()
{
	size_t _Diff = _Block_count >> 1;
	size_t _New_size = _Block_count + _Diff;

	size_t _Begin_dis = _Begin._Block - _Blocks;
	size_t _End_dis = _End._Block - _Blocks;

	_Block_al::reallocate(_Blocks, _Block_count, _New_size);

	_Begin._Block = _Blocks + _Begin_dis;
	_End._Block = _Blocks + _End_dis;

	_Block_count = _New_size;
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::_Extend_front()
{
	size_t _Diff = _Block_count >> 1;
	size_t _New_size = _Block_count + _Diff;

	size_t _Begin_dis = _Begin._Block - _Blocks;
	size_t _End_dis = _End._Block - _Blocks;

	_Block_al::reallocate(_Blocks, _Block_count, _New_size, _Diff);

	_Begin._Block = _Blocks + (_Begin_dis + _Diff);
	_End._Block = _Blocks + (_End_dis + _Diff);

	_Block_count = _New_size;
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void deque<_Ty, _Alloc>::_Emplace_back(_Values&&... _Vals)
{
	++_Size;
	if (_Begin._Ptr == nullptr) {
		_Emplace_first_el(forward<_Values>(_Vals)...);
		return;
	}
	if (_End._Block == &_Blocks[_Block_count - 1]) {
		_Extend_back();
	}
	_Alloc::construct(_End._Ptr, forward<_Values>(_Vals)...);
	
	if (_End._Is_last_in_block()) {
		auto _New_block = _Alloc::allocate(_Block_size);
		_Block_al::construct(_End._Block + 1, _New_block);
	}
	++_End;
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void deque<_Ty, _Alloc>::_Emplace_front(_Values&&... _Vals)
{
	++_Size;
	if (_Begin._Ptr == nullptr) {
		_Emplace_first_el(forward<_Values>(_Vals)...);
		return;
	}
	if (_Begin._Block == &_Blocks[0]) {
		_Extend_front();
	}

	if (_Begin._Is_first_in_block()) {
		auto _New_block = _Alloc::allocate(_Block_size);
		_Block_al::construct(_Begin._Block - 1, _New_block);
	}
	--_Begin;
	_Alloc::construct(_Begin._Ptr, forward<_Values>(_Vals)...);
}

template<class _Ty, class _Alloc>
template<class ..._Values>
inline constexpr void deque<_Ty, _Alloc>::_Emplace_first_el(_Values&&... _Vals)
{
	_Alloc::construct(&_Blocks[1][0], forward<_Values>(_Vals)...);
	_Begin._Block = &_Blocks[1];
	_Begin._Ptr = &_Blocks[1][0];

	_End._Block = &_Blocks[1];
	_End._Ptr = &_Blocks[1][1];
}
 
template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::_Copy(const deque& _Other)
{
	_Block_count = _Other._Block_count;
	_Blocks = _Block_al::allocate(_Block_count);
	_Size = _Other._Size;

	_Begin._Block = _Blocks + (_Other._Begin._Block - _Other._Blocks);
	_End._Block = _Blocks + (_Other._End._Block - _Other._Blocks);

	// allocate required blocks
	for (auto _It = _Begin._Block; _It != _End._Block + 1; ++_It) {
		auto _New_block = _Alloc::allocate(_Block_size);
		new(_It) auto(_New_block);
	}

	_Begin._Ptr = *_Begin._Block + (_Other._Begin._Ptr - *_Other._Begin._Block);
	_End._Ptr = *_End._Block + (_Other._End._Ptr - *_Other._End._Block);

	// create by copying
	auto _Dst = _Begin;
	for (auto _It = _Other._Begin; _It != _Other._End; ++_It, ++_Dst) {
		_Alloc::construct(_Dst._Ptr, *_It);
	}
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::_Move(deque&& _Other) noexcept
{
	_Block_count = _Other._Block_count;
	_Size = _Other._Size;

	_Blocks = _Other._Blocks;
	_Other._Blocks = nullptr;

	_Begin = _Other._Begin;
	_Other._Begin._Ptr = nullptr;

	_End = _Other._End;
	_Other._End._Ptr = nullptr;
}

template<class _Ty, class _Alloc>
inline constexpr void deque<_Ty, _Alloc>::_Clear()
{
	if (!_Begin._Ptr) return;
	
	auto _First = _Begin;
	auto _Last = _End;
	
	if (_First._Block == _Last._Block) {
		_Alloc::destroy_range(_First._Ptr, _Last._Ptr);
		_Alloc::deallocate(*_First._Block);
		return;
	}

	// destroy first block
	_Alloc::destroy_range(_First._Ptr, *_First._Block + _Block_size);
	_Alloc::deallocate(*_First._Block);

	// destroy blocks between
	auto _Next_block = _First._Block + 1;
	while (_Next_block != _Last._Block) {
		_Alloc::destroy_range(*_Next_block, *_Next_block + _Block_size);
		_Alloc::deallocate(*_Next_block);
		++_Next_block;
	}

	// destroy last block
	_Alloc::destroy_range(*_End._Block, _End._Ptr);
	_Alloc::deallocate(*_End._Block);
	
	_Block_al::deallocate(_Blocks);
	
	// clear variables
	_Blocks = nullptr;
	_Begin._Ptr = nullptr;
	_End._Ptr = nullptr;
}