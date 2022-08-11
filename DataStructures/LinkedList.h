#pragma once
#include "Utility.h"

#pragma pack (push, 1)

template  <class _Ty>
struct linkedlist_node;

template  <class _Ty>
struct linkedlist_iterator;

template  <class _Ty>
class linkedlist
{
public:
	using iterator = linkedlist_iterator<_Ty>;
	using const_iterator = linkedlist_iterator<const _Ty>;

	using node = linkedlist_node<_Ty>;

	linkedlist();
	linkedlist(const linkedlist& _List);
	linkedlist(linkedlist&& _List);
	~linkedlist();
	void append(const _Ty& _Val);
	void append(_Ty&& _Val) noexcept;
	template <class _FwdIt>
	void append(_FwdIt _First, const _FwdIt _Last);
	template <class ..._Values>
	void emplace_back(_Values&&... _Vals) noexcept;
	void insert(unsigned int _Index, const _Ty& _Val);
	void insert(unsigned int _Index, _Ty&& _Val) noexcept;
	template <class ..._Values>
	void emplace(unsigned int _Index, _Values&&... _Vals) noexcept;
	bool remove_at(unsigned int _Index);
	bool remove(const _Ty& _Val);
	bool pop_begin();
	bool pop_back();
	void clear();
	iterator find(const _Ty& _Val);


	constexpr iterator begin();
	constexpr iterator begin() const;
	constexpr iterator end();
	constexpr iterator end() const;

	constexpr size_t size() const;

	_Ty& back();
	const _Ty& back() const;
	_Ty& front();
	const _Ty& front() const;

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	linkedlist& operator=(const linkedlist& _List);
	linkedlist& operator=(linkedlist&& _List);


	bool operator<(const linkedlist<_Ty>& _Vec) const;
	constexpr bool operator<=(const linkedlist& _Vec) const;
	bool operator>(const linkedlist<_Ty>& _Vec) const;
	constexpr bool operator>=(const linkedlist& _Vec) const;

private:
	node* _Head;
	node* _Tail;
	size_t _Size;

	template <class ..._Values>
	void _Emplace_back(_Values&& ..._Vals) noexcept;
	template <class ..._Values>
	void _Emplace(unsigned int _Index, _Values&&... _Vals) noexcept;
	void _Insert_node(node*& _Node, unsigned int _Index);
	node*& _Search(unsigned int _Index);
	constexpr node*& _Search_first_half(unsigned int _Index);
	constexpr node*& _Search_second_half(unsigned int _Index);

	constexpr void _Copy_from(const linkedlist<_Ty>& _List);
	constexpr void _Destroy_data();
};

template <class _Ty>
inline linkedlist<_Ty>::linkedlist()
{
	_Size = 0;
	_Head = NULL;
	_Tail = NULL;
}

template <class _Ty>
inline linkedlist<_Ty>::linkedlist(const linkedlist<_Ty>& _List)
{
	_Copy_from(_List);
}

template <class _Ty>
inline linkedlist<_Ty>::linkedlist(linkedlist<_Ty>&& _List)
{
	_Head = move(_List._Head);
	_Tail = move(_List._Tail);
	_Size = move(_List._Size);

	_List._Head = NULL;
	_List._Tail = NULL;
	_List._Size = 0;
}

template <class _Ty>
inline linkedlist<_Ty>::~linkedlist()
{
	_Destroy_data();
}

template <class _Ty>
inline void linkedlist<_Ty>::append(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template <class _Ty>
inline void linkedlist<_Ty>::append(_Ty&& _Val) noexcept
{
	_Emplace_back(move<_Ty>(_Val));
}

template<class _Ty>
template<class _FwdIt>
inline void linkedlist<_Ty>::append(_FwdIt _First, const _FwdIt _Last)
{
	if (_First == _Last) return;

	_Emplace_back(*_First++);

	for (; _First != _Last; ++_First) {
		auto* _New = new node(*_First);
		_Tail->next = _New;
		_New->prev = _Tail;

		_Tail = _New;
		++_Size;
	}
}

template <class _Ty>
template<class ..._Values>
inline void linkedlist<_Ty>::emplace_back(_Values&& ..._Vals) noexcept
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty>
template<class ..._Values>
inline void linkedlist<_Ty>::emplace(unsigned int _Index, _Values && ..._Vals) noexcept
{
	_Emplace(_Index, forward<_Values>(_Vals)...);
}

template <class _Ty>
inline void linkedlist<_Ty>::insert(unsigned int _Index, const _Ty& _Val)
{
	if (_Index == _Size) return append(_Val);

	auto next = new node(_Val);

	_Insert_node(next, _Index);
}

template <class _Ty>
inline void linkedlist<_Ty>::insert(unsigned int _Index, _Ty&& _Val) noexcept
{
	if (_Index == _Size) return append(move(_Val));

	auto next = new node(move(_Val));

	_Insert_node(next, _Index);
}

template <class _Ty>
inline bool linkedlist<_Ty>::remove_at(unsigned int _Index)
{
	if (_Index == _Size - 1) return pop_back();
	if (_Index == 0) return pop_begin();

	auto node = _Search(_Index);
	auto prev = node->prev;
	auto next = node->next;

	delete node;

	prev->next = next;
	next->prev = prev;

	--_Size;
	return true;
}

template<class _Ty>
inline bool linkedlist<_Ty>::remove(const _Ty& _Val)
{
	auto _Tmp = _Head;
	while (_Tmp) {
		if (_Tmp->value == _Val) {
			auto _To_remove = _Tmp;
			auto _Prev = _Tmp->prev;
			auto _Next = _Tmp->next;
			if (_Prev) _Prev->next = _Next;
			else _Head = _Next;
			if (_Next) _Next->prev = _Prev;
			else _Tail = _Prev;
			delete _To_remove;
			return true;
		}
		_Tmp = _Tmp->next;
	}
	return false;
}

template <class _Ty>
inline bool linkedlist<_Ty>::pop_begin()
{
	if (_Size > 1) {
		auto next = _Head->next;
		auto node = _Head;

		delete node;
		_Head = next;
		--_Size;
		return true;
	}
	return pop_back();
}

template <class _Ty>
inline bool linkedlist<_Ty>::pop_back()
{
	if (_Size > 1) {
		auto prev = _Tail->prev;
		auto node = _Tail;
		delete node;
		_Tail = prev;
	}
	else if (_Size == 1) {
		delete _Head;
		_Head = NULL;
		_Tail = NULL;
	}
	else {
		return false;
	}
	--_Size;
	return true;
}

template<class _Ty>
inline void linkedlist<_Ty>::clear()
{
	_Destroy_data();
	_Size = 0;
}

template<class _Ty>
inline linkedlist_iterator<_Ty> linkedlist<_Ty>::find(const _Ty& _Val)
{
	for (iterator it = begin(); it != end(); ++it) {
		if (*it == _Val) return it;
	}
	return end();
}

template <class _Ty>
inline constexpr linkedlist_iterator<_Ty> linkedlist<_Ty>::begin()
{
	return iterator(_Head);
}

template <class _Ty>
inline constexpr linkedlist_iterator<_Ty> linkedlist<_Ty>::begin() const
{
	return iterator(_Head);
}

template <class _Ty>
inline constexpr linkedlist_iterator<_Ty> linkedlist<_Ty>::end()
{
	return iterator(_Tail->next);
}

template <class _Ty>
inline constexpr linkedlist_iterator<_Ty> linkedlist<_Ty>::end() const
{
	return iterator(_Tail->next);
}

template <class _Ty>
inline constexpr size_t linkedlist<_Ty>::size() const
{
	return _Size;
}

template <class _Ty>
inline _Ty& linkedlist<_Ty>::back()
{
	return _Tail->value;
}

template <class _Ty>
inline const _Ty& linkedlist<_Ty>::back() const
{
	return _Tail->value;
}

template <class _Ty>
inline _Ty& linkedlist<_Ty>::front()
{
	return _Head->value;
}

template <class _Ty>
inline const _Ty& linkedlist<_Ty>::front() const
{
	return _Head->value;
}

template <class _Ty>
inline _Ty& linkedlist<_Ty>::operator[](unsigned int _Index)
{
	return _Search(_Index)->value;
}

template <class _Ty>
inline const _Ty& linkedlist<_Ty>::operator[](unsigned int _Index) const
{
	return _Search(_Index)->value;
}

template <class _Ty>
inline linkedlist<_Ty>& linkedlist<_Ty>::operator=(const linkedlist<_Ty>& _List)
{
	_Destroy_data();
	_Copy_from(_List);
	return *this;
}

template <class _Ty>
inline linkedlist<_Ty>& linkedlist<_Ty>::operator=(linkedlist<_Ty>&& _List)
{
	_Destroy_data();

	_Head = _List._Head;
	_Tail = _List._Tail;
	_Size = _List._Size;

	_List._Head = NULL;
	_List._Tail = NULL;
	_List._Size = 0;
	return *this;
}

template<class _Ty>
inline bool linkedlist<_Ty>::operator<(const linkedlist<_Ty>& _Vec) const
{
	return ::lexicographical_compare(begin(), end(), _Vec.begin(), _Vec.end());
}

template<class _Ty>
inline constexpr bool linkedlist<_Ty>::operator<=(const linkedlist<_Ty>& _Vec) const
{
	return !(*this > _Vec);
}

template<class _Ty>
inline bool linkedlist<_Ty>::operator>(const linkedlist<_Ty>& _Vec) const
{
	return ::lexicographical_compare(_Vec.begin(), _Vec.end(), begin(), end());
}

template<class _Ty>
inline constexpr bool linkedlist<_Ty>::operator>=(const linkedlist<_Ty>& _Vec) const
{
	return !(*this < _Vec);
}

template <class _Ty>
template <class ..._Values>
inline void linkedlist<_Ty>::_Emplace_back(_Values&& ..._Vals) noexcept
{
	node* _New = new node(::forward<_Values>(_Vals)...);

	if (_Size == 0) {
		_Head = _New;
		_Tail = _New;
	}
	else {
		_Tail->next = _New;
		_New->prev = _Tail;

		_Tail = _New;
	}
	++_Size;
}

template<class _Ty>
template<class ..._Values>
inline void linkedlist<_Ty>::_Emplace(unsigned int _Index, _Values && ..._Vals) noexcept
{
	if (_Index == _Size) {
		_Emplace_back(forward<_Values>(_Vals)...);
		return;
	}

	auto* _Tmp = _Search(_Index);

	auto* _Node = new node(forward<_Values>(_Vals)...);
	auto prev = _Tmp->prev;
	_Tmp->prev  = _Node;
	_Node->next = _Tmp;
	_Node->prev = prev;

	if (_Index == 0) _Head = _Node;
	else _Node->prev->next = _Node;

	++_Size;
}


template <class _Ty>
inline void linkedlist<_Ty>::_Insert_node(node*& _Node, unsigned int _Index)
{
	node* _Tmp = _Search(_Index);

	auto prev = _Tmp->prev;
	_Tmp->prev = _Node;
	_Node->next = _Tmp;
	_Node->prev = prev;

	if (_Index == 0) _Head = _Node;
	else _Node->prev->next = _Node;

	++_Size;
}

template <class _Ty>
inline linkedlist_node<_Ty>*& linkedlist<_Ty>::_Search(unsigned int _Index)
{
	if (_Index <= _Size / 2) return _Search_first_half(_Index);
	return _Search_second_half(_Index);
}

template <class _Ty>
inline constexpr linkedlist_node<_Ty>*&linkedlist<_Ty>::_Search_first_half(unsigned int _Index)
{
	node* _Tmp = _Head;
	for (int i = 0; i < _Index; ++i) {
		_Tmp = _Tmp->next;
	}
	return _Tmp;
}

template <class _Ty>
inline constexpr linkedlist_node<_Ty>*& linkedlist<_Ty>::_Search_second_half(unsigned int _Index)
{
	node* _Tmp = _Tail;
	for (int i = _Size; i > _Index + 1; --i) {
		_Tmp = _Tmp->prev;
	}
	return _Tmp;
}

template <class _Ty>
inline constexpr void linkedlist<_Ty>::_Copy_from(const linkedlist<_Ty>& _List)
{
	if (_List._Size == 0) {
		_Size = 0;
		_Head = NULL;
		_Tail = NULL;
		return;
	}

	auto it = _List.begin();
	_Head = new node(*it);

	auto _Tmp = _Head;

	while (++it != _List.end()) {
		auto node = new linkedlist_node<_Ty>(*it);
		node->prev = _Tmp;
		_Tmp->next = node;
		_Tmp = _Tmp->next;
	}
	_Tail = _Tmp;
	_Size = _List._Size;
}

template <class _Ty>
inline constexpr void linkedlist<_Ty>::_Destroy_data()
{
	if (!_Head) return;
	while (_Head != _Tail) {
		auto _Tmp = _Tail->prev;
		_Tmp->next = NULL;
		delete _Tail;
		_Tail = _Tmp;
	}

	delete _Head;
	_Head = NULL;
	_Tail = NULL;
}

///
///  LIST NODE
/// 

template <class _Ty>
struct linkedlist_node
{
public:
	linkedlist_node* next = NULL;
	linkedlist_node* prev = NULL;
	_Ty value;
	linkedlist_node()
	{ }
	linkedlist_node(const _Ty& _Val)
		: value(_Val)
	{ }
	linkedlist_node(_Ty&& _Val)
		: value(move(_Val))
	{ }
	linkedlist_node(const linkedlist_node& other)
		: value(other.value)
	{
		next = other.next;
		prev = other.prev;
	}
	linkedlist_node(linkedlist_node&& other)
		: value(move(other.value))
	{
		next = other.next;
		prev = other.prev;

		other.next = NULL;
		other.prev = NULL;
	}
	linkedlist_node& operator=(const linkedlist_node& other)
	{
		_Copy_from(other);
		return *this;
	}
	linkedlist_node& operator=(linkedlist_node&& other)
	{
		next = other.next;
		prev = other.prev;
		value.~_Ty();
		new (&value) _Ty(move(value));

		other.next = NULL;
		other.prev = NULL;
		return *this;
	}
	constexpr bool operator==(const linkedlist_node& _Right) {
		return value == _Right.value;
	}
	constexpr bool operator!=(const linkedlist_node& _Right) {
		return !(value == _Right.value);
	}
private:
	constexpr void _Copy_from(const linkedlist_node& other)
	{
		next = other.next;
		prev = other.prev;
		value.~_Ty();
		new (&value) _Ty(other.value);
	}
};

//
//	ITERATOR
//

template<class _Ty>
struct linkedlist_iterator
{
	friend class linkedlist<remove_const_t<_Ty>>;
public:
	using node = linkedlist_node<remove_const_t<_Ty>>;
	using category = complex_iterator;
	linkedlist_iterator();
	linkedlist_iterator(node* const& _Ptr);
	linkedlist_iterator(node*&& _Ptr);
	linkedlist_iterator(const linkedlist_iterator& _Other);
	linkedlist_iterator(linkedlist_iterator&& _Other);

	_Ty& operator*();
	linkedlist_iterator<_Ty>& operator++();
	linkedlist_iterator<_Ty> operator++(int);

	bool operator==(const linkedlist_iterator<_Ty>& _Right) const;
	bool operator!=(const linkedlist_iterator<_Ty>& _Right) const;
	
	linkedlist_iterator& operator=(const linkedlist_iterator<_Ty>& _Right);
	linkedlist_iterator& operator=(linkedlist_iterator<_Ty>&& _Right);
private:

	node* _List;
};

template<class _Ty>
inline linkedlist_iterator<_Ty>::linkedlist_iterator()
	: _List(NULL)
{ }

template<class _Ty>
inline linkedlist_iterator<_Ty>::linkedlist_iterator(node* const& _Ptr)
{
	_List = _Ptr;
}

template<class _Ty>
inline linkedlist_iterator<_Ty>::linkedlist_iterator(node*&& _Ptr)
{
	_List = _Ptr;
	_Ptr = NULL;
}

template<class _Ty>
inline linkedlist_iterator<_Ty>::linkedlist_iterator(const linkedlist_iterator& _Other)
{
	_List = _Other._List;
}

template<class _Ty>
inline linkedlist_iterator<_Ty>::linkedlist_iterator(linkedlist_iterator&& _Other)
{
	_List = _Other._List;
	_Other._List = NULL;
}

template<class _Ty>
inline _Ty& linkedlist_iterator<_Ty>::operator*()
{
	return this->_List->value;
}

template<class _Ty>
inline linkedlist_iterator<_Ty>& linkedlist_iterator<_Ty>::operator++()
{
	_List = _List->next;
	return *this;
}

template<class _Ty>
inline linkedlist_iterator<_Ty> linkedlist_iterator<_Ty>::operator++(int)
{
	auto p = *this;
	_List = _List->next;
	return *p;
}

template<class _Ty>
inline bool linkedlist_iterator<_Ty>::operator==(const linkedlist_iterator<_Ty>& _Right) const
{
	return _List == _Right._List;
}

template<class _Ty>
inline bool linkedlist_iterator<_Ty>::operator!=(const linkedlist_iterator<_Ty>& _Right) const
{
	return _List != _Right._List;
}

template<class _Ty>
inline linkedlist_iterator<_Ty>& linkedlist_iterator<_Ty>::operator=(const linkedlist_iterator<_Ty>& _Right)
{
	_List = _Right._List;
	return *this;
}

template<class _Ty>
inline linkedlist_iterator<_Ty>& linkedlist_iterator<_Ty>::operator=(linkedlist_iterator<_Ty>&& _Right)
{
	_List = _Right._List;
	_Right._List = NULL;
	return *this;
}

#pragma pack(pop)