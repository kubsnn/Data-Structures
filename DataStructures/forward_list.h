#pragma once

#include "utility.h"

template <class _Ty>
struct forward_list_iterator;

template <class _Ty>
struct forward_list_node;

template <class _Ty>
class forward_list
{
public:
	using node = forward_list_node<_Ty>;
	using iterator = forward_list_iterator<_Ty>;
	using const_iterator = forward_list_iterator<const _Ty>;
	using value_type = _Ty;

	forward_list();
	forward_list(const forward_list& _Other);
	forward_list(forward_list&& _Other) noexcept;
	~forward_list();
	forward_list& operator=(const forward_list& _Other);
	forward_list& operator=(forward_list&& _Other) noexcept;

	void push_front(const _Ty& _Val);
	void push_front(_Ty&& _Val);
	template <class... _Values>
	void emplace_front(_Values&&... _Vals);

	void remove(const _Ty& _Val);
	void remove_at(unsigned int _Index);
	iterator remove(iterator _Where);

	_Ty& at(unsigned int _Index);
	const _Ty& at(unsigned int _Index) const;
	iterator find(const _Ty& _Val);
	const_iterator find(const _Ty& _Val) const;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	constexpr size_t size() const;
private:
	template <class... _Values>
	void _Emplace_front(_Values&&... _Vals);
	void _Copy(const forward_list& _Other);
	void _Clear();

	node* _Head;
	size_t _Size;
};


template <class _Ty>
struct forward_list_node
{
public:
	using node = forward_list_node<_Ty>;
	friend class forward_list<_Ty>;
	friend struct forward_list_iterator<_Ty>;
	friend struct forward_list_iterator<const _Ty>;

	forward_list_node() : _Next(nullptr) 
	{ }
	forward_list_node(const node& _Other)
		: _Value(_Other._Value)
	{
		_Next = _Other._Next;
	}
	forward_list_node(const _Ty& _Val) 
		: _Value(_Val), _Next(nullptr)
	{ }
	forward_list_node(_Ty&& _Val)
		: _Value(move(_Val)), _Next(nullptr)
	{ }
	template <class... _Args>
	forward_list_node(_Args&&... args)
		: _Value(forward<_Args>(args)...), _Next(nullptr)
	{ }

	node& operator=(const _Ty& _Other) {
		_Next = _Other._Next;
		_Value = _Other._Value;
		return *this;
	}
	node& operator=(_Ty&& _Other) {
		_Next = _Other._Next;
		_Value = move(_Other._Value);
		return *this;
	}

	bool operator==(const _Ty& _Other) const {
		return _Value == _Other._Value;
	}
	bool operator!=(const _Ty& _Other) const {
		return _Value != _Other._Value;
	}
private:
	node* _Next;
	_Ty _Value;
};

template <class _Ty>
struct forward_list_iterator
{
public:
	using category = forward_iterator;

	using node = forward_list_node<remove_const_t<_Ty>>;
	using iterator = forward_list_iterator<_Ty>;
	friend class forward_list<remove_const_t<_Ty>>;

	forward_list_iterator(node* _Pointer) {
		_Ptr = _Pointer;
	}
	_Ty& operator*() {
		return _Ptr->_Value;
	}
	_Ty* operator->() {
		return &_Ptr->_Value;
	}

	iterator& operator++() {
		_Ptr = _Ptr->_Next;
		return *this;
	}
	iterator operator++(int) {
		iterator tmp = *this;
		_Ptr = _Ptr->_Next;
		return tmp;
	}

	bool operator==(const iterator& _Other) const {
		return _Ptr == _Other._Ptr;
	}
	bool operator!=(const iterator& _Other) const {
		return _Ptr != _Other._Ptr;
	}

private:
	node* _Ptr;
};

template<class _Ty>
inline forward_list<_Ty>::forward_list()
{
	_Size = 0;
	_Head = NULL;
}

template<class _Ty>
inline forward_list<_Ty>::forward_list(const forward_list& _Other)
{
	_Copy(_Other);
}

template<class _Ty>
inline forward_list<_Ty>::forward_list(forward_list&& _Other) noexcept
{
	_Head = _Other._Head;
	_Size = _Other._Size;
	_Other._Head = nullptr;
	_Other._Size = 0;
}

template<class _Ty>
inline forward_list<_Ty>::~forward_list()
{
	if (_Head == nullptr) return;

	_Clear();
}

template<class _Ty>
inline forward_list<_Ty>& forward_list<_Ty>::operator=(const forward_list& _Other)
{
	if (this == &_Other) return *this;
	_Clear();
	_Copy(_Other);
	return *this;
}

template<class _Ty>
inline forward_list<_Ty>& forward_list<_Ty>::operator=(forward_list&& _Other) noexcept
{
	if (this == &_Other) return *this;
	_Clear();
	_Head = _Other._Head;
	_Size = _Other._Size;
	_Other._Head = nullptr;
	_Other._Size = 0;
	return *this;
}

template<class _Ty>
inline void forward_list<_Ty>::push_front(const _Ty& _Val)
{
	_Emplace_front(_Val);
}

template<class _Ty>
inline void forward_list<_Ty>::push_front(_Ty&& _Val)
{
	_Emplace_front(move(_Val));
}

template<class _Ty>
template<class ..._Values>
inline void forward_list<_Ty>::emplace_front(_Values&&... _Vals)
{
	_Emplace_front(forward<_Values>(_Vals)...);
}

template<class _Ty>
inline void forward_list<_Ty>::remove(const _Ty& _Val)
{
	if (_Head == nullptr) return;
	if (_Head->_Value == _Val) {
		node* _Tmp = _Head->_Next;
		delete _Head;
		_Head = _Tmp;
		--_Size;
		return;
	}
	node* _Tmp = _Head;

	while (_Tmp->_Next != nullptr && _Tmp->_Next->_Value != _Val) {
		_Tmp = _Tmp->_Next;
	}
	if (_Tmp->_Next == nullptr) return;
	
	node* _Next = _Tmp->_Next->_Next;
	delete _Tmp->_Next;
	_Tmp->_Next = _Next;
	--_Size;
}

template<class _Ty>
inline void forward_list<_Ty>::remove_at(unsigned int _Index)
{
	if (!(0 <= _Index && _Index < _Size)) return;

	--_Size;

	if (_Index == 0) {
		node* _Next = _Head->_Next;
		delete _Head;
		_Head = _Next;
		return;
	}

	node* _Tmp = _Head;

	for (unsigned int i = 1; i < _Index; ++i) {
		_Tmp = _Tmp->_Next;
	}
	node* _Next = _Tmp->_Next->_Next;
	delete _Tmp->_Next;
	_Tmp->_Next = _Next;
}

template<class _Ty>
inline forward_list_iterator<_Ty> forward_list<_Ty>::remove(iterator _Where)
{
	--_Size;

	if (_Where == _Head) {
		node* _Next = _Head->_Next;
		delete _Head;
		_Head = _Next;
		return _Head;
	}

	node* _Tmp = _Head;

	while (_Tmp->_Next != _Where) {
		_Tmp = _Tmp->_Next;
	}

	node* _Next = _Tmp->_Next->_Next;
	delete _Tmp->_Next;
	_Tmp->_Next = _Next;
	return iterator(_Tmp->_Next);
}

template<class _Ty>
inline _Ty& forward_list<_Ty>::at(unsigned int _Index)
{
	node* _Tmp = _Head;
	for (unsigned int i = 0; i < _Index; ++i) {
		_Tmp = _Tmp->_Next;
	}
	return _Tmp->_Value;
}

template<class _Ty>
inline const _Ty& forward_list<_Ty>::at(unsigned int _Index) const
{
	node* _Tmp = _Head;
	for (unsigned int i = 0; i < _Index; ++i) {
		_Tmp = _Tmp->_Next;
	}
	return _Tmp->_Value;
}

template<class _Ty>
inline forward_list_iterator<_Ty> forward_list<_Ty>::find(const _Ty& _Val)
{
	for (node* _Tmp = _Head; _Tmp != nullptr; _Tmp = _Tmp->_Next) {
		if (_Tmp->_Value == _Val) return iterator(_Tmp);
	}
	return iterator(nullptr);
}

template<class _Ty>
inline forward_list_iterator<const _Ty> forward_list<_Ty>::find(const _Ty& _Val) const
{
	for (node* _Tmp = _Head; _Tmp != nullptr; _Tmp = _Tmp->_Next) {
		if (_Tmp->_Value == _Val) return iterator(_Tmp);
	}
	return iterator(nullptr);
}

template <class _Ty>
inline forward_list_iterator<_Ty> forward_list<_Ty>::begin()
{
	return iterator(_Head);
}

template <class _Ty>
inline forward_list_iterator<const _Ty> forward_list<_Ty>::begin() const
{
	return const_iterator(_Head);
}

template <class _Ty>
inline forward_list_iterator<_Ty> forward_list<_Ty>::end()
{
	return iterator(nullptr);
}

template <class _Ty>
inline forward_list_iterator<const _Ty> forward_list<_Ty>::end() const
{
	return const_iterator(nullptr);
}

template<class _Ty>
inline constexpr size_t forward_list<_Ty>::size() const
{
	return _Size;
}

template<class _Ty>
template<class ..._Values>
inline void forward_list<_Ty>::_Emplace_front(_Values&& ..._Vals)
{
	node* _Old_head = _Head;

	_Head = new node(forward<_Values>(_Vals)...);
	_Head->_Next = _Old_head;

	_Size++;
}

template<class _Ty>
inline void forward_list<_Ty>::_Copy(const forward_list& _Other)
{
	if (_Other._Size == 0) return;

	_Head = new node(_Other._Head->_Value);

	node* src = _Other._Head;
	node* dst = _Head;
	while (src->_Next != nullptr) {
		dst->_Next = new node(src->_Next->_Value);
		dst = dst->_Next;
		src = src->_Next;
	}
	_Size = _Other._Size;
}

template<class _Ty>
inline void forward_list<_Ty>::_Clear()
{
	node* tmp = _Head;
	while (tmp != nullptr) {
		auto deleted = tmp;
		tmp = tmp->_Next;
		delete deleted;
	}
	_Head = nullptr;
	_Size = 0;
}

