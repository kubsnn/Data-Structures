#pragma once
#include "Utility.h"

template<typename _Ty>
struct Node
{
	Node* next;
	Node* prev;
	_Ty value;
	Node()
		: next(NULL)
		, prev(NULL)
	{ }
};

template<typename _Ty>
struct LinkedListIterator;

template<typename _Ty>
class LinkedList
{
public:
	LinkedList();
	LinkedList(const LinkedList<_Ty>& _List);
	LinkedList(LinkedList<_Ty>&& _List);
	~LinkedList();
	void append(const _Ty& _Val);
	void append(_Ty&& _Val);
	void insert(const _Ty& _Val, unsigned int _Index);
	void insert(_Ty&& _Val, unsigned int _Index);

	bool remove_at(unsigned int _Index);
	bool pop_begin();
	bool pop_back();

	LinkedListIterator<_Ty> begin();
	const LinkedListIterator<_Ty> begin() const;
	LinkedListIterator<_Ty> end();
	const LinkedListIterator<_Ty> end() const;

	size_t size() const;

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	LinkedList<_Ty>& operator=(const LinkedList<_Ty>& _List);
	LinkedList<_Ty>& operator=(LinkedList<_Ty>&& _List);

private:
	Node<_Ty>* _begin;
	Node<_Ty>* _end;
	size_t _size;

	void _append_node(Node<_Ty>*& _Node);
	void _insert_node(Node<_Ty>*& _Node, unsigned int _Index);
	Node<_Ty>*& _search(unsigned int _Index);
	Node<_Ty>*& _search_first_half(unsigned int _Index);
	Node<_Ty>*& _search_second_half(unsigned int _Index);

	void _copy_from(const LinkedList<_Ty>& _List);
	void _destroy_data();
};

template<typename _Ty>
inline LinkedList<_Ty>::LinkedList()
{
	_size = 0;
	_begin = NULL;
	_end = NULL;
}

template<typename _Ty>
inline LinkedList<_Ty>::LinkedList(const LinkedList<_Ty>& _List)
{
	_copy_from(_List);
}

template<typename _Ty>
inline LinkedList<_Ty>::LinkedList(LinkedList<_Ty>&& _List)
{
	_begin = move(_List._begin);
	_end = move(_List._end);
	_size = move(_List._size);

	_List._begin = NULL;
	_List._end = NULL;
	_List._size = 0;
}

template<typename _Ty>
inline LinkedList<_Ty>::~LinkedList()
{
	_destroy_data();
}

template<typename _Ty>
inline void LinkedList<_Ty>::append(const _Ty& _Val)
{
	auto node = new Node<_Ty>();
	node->value = _Val;

	_append_node(node);
}

template<typename _Ty>
inline void LinkedList<_Ty>::append(_Ty&& _Val)
{
	auto node = new Node<_Ty>();
	node->value = move(_Val);

	_append_node(node);
}

template<typename _Ty>
inline void LinkedList<_Ty>::insert(const _Ty& _Val, unsigned int _Index)
{
	if (_Index == _size) return append(_Val);

	auto next = new Node<_Ty>();
	next->value = const_cast<_Ty&>(_Val);

	_insert_node(next, _Index);
}

template<typename _Ty>
inline void LinkedList<_Ty>::insert(_Ty&& _Val, unsigned int _Index)
{
	if (_Index == _size) return append(move(_Val));

	auto next = new Node<_Ty>();
	next->value = move(_Val);

	_insert_node(next, _Index);
}

template<typename _Ty>
inline bool LinkedList<_Ty>::remove_at(unsigned int _Index)
{
	if (_Index == _size - 1) return pop_back();
	if (_Index == 0) return pop_begin();

	auto node = _search(_Index);
	auto prev = node->prev;
	auto next = node->next;

	delete node;

	prev->next = next;
	next->prev = prev;

	--_size;
	return true;
}

template<typename _Ty>
inline bool LinkedList<_Ty>::pop_begin()
{
	if (_size > 1) {
		auto next = _begin->next;
		auto node = _begin;
		node->value = NULL;
		delete node;
		_begin = next;
		--_size;
		return true;
	}
	return pop_back();
}

template<typename _Ty>
inline bool LinkedList<_Ty>::pop_back()
{
	if (_size > 1) {
		auto prev = _end->prev;
		auto node = _end;
		delete node;
		_end = prev;
	}
	else if (_size == 1) {
		delete _begin;
		_begin = NULL;
		_end = NULL;
	}
	else {
		return false;
	}
	--_size;
	return true;
}

template<typename _Ty>
inline LinkedListIterator<_Ty> LinkedList<_Ty>::begin()
{
	return LinkedListIterator<_Ty>(_begin);
}

template<typename _Ty>
inline const LinkedListIterator<_Ty> LinkedList<_Ty>::begin() const
{
	return LinkedListIterator<_Ty>(_begin);
}

template<typename _Ty>
inline LinkedListIterator<_Ty> LinkedList<_Ty>::end()
{
	return LinkedListIterator<_Ty>(_end->next);
}

template<typename _Ty>
inline const LinkedListIterator<_Ty> LinkedList<_Ty>::end() const
{
	return LinkedListIterator<_Ty>(_end->next);
}

template<typename _Ty>
inline size_t LinkedList<_Ty>::size() const
{
	return _size;
}

template<typename _Ty>
inline _Ty& LinkedList<_Ty>::operator[](unsigned int _Index)
{
	return _search(_Index)->value;
}

template<typename _Ty>
inline const _Ty& LinkedList<_Ty>::operator[](unsigned int _Index) const
{
	return _search(_Index)->value;
}

template<typename _Ty>
inline LinkedList<_Ty>& LinkedList<_Ty>::operator=(const LinkedList<_Ty>& _List)
{
	_destroy_data();
	_copy_from(_List);
	return *this;
}

template<typename _Ty>
inline LinkedList<_Ty>& LinkedList<_Ty>::operator=(LinkedList<_Ty>&& _List)
{
	_destroy_data();

	_begin = move(_List._begin);
	_end = move(_List._end);
	_size = move(_List._size);

	_List._begin = NULL;
	_List._end = NULL;
	_List._size = 0;
	return *this;
}


template<typename _Ty>
inline void LinkedList<_Ty>::_append_node(Node<_Ty>*& _Node)
{
	if (_size == 0) {
		_begin = _Node;
		_end = _Node;
	}
	else {
		_end->next = _Node;
		_Node->prev = _end;

		_end = _Node;
	}
	++_size;
}

template<typename _Ty>
inline void LinkedList<_Ty>::_insert_node(Node<_Ty>*& _Node, unsigned int _Index)
{
	Node<_Ty>* node = _search(_Index);

	auto prev = node->prev;
	node->prev = _Node;
	_Node->next = node;
	_Node->prev = prev;

	if (_Index == 0) _begin = _Node;
	else _Node->prev->next = _Node;

	++_size;
}

template<typename _Ty>
inline Node<_Ty>*& LinkedList<_Ty>::_search(unsigned int _Index)
{
	if (_Index <= _size / 2) return _search_first_half(_Index);
	return _search_second_half(_Index);
}

template<typename _Ty>
inline Node<_Ty>*& LinkedList<_Ty>::_search_first_half(unsigned int _Index)
{
	Node<_Ty>* tmp = _begin;
	for (int i = 0; i < _Index; ++i) {
		tmp = tmp->next;
	}
	return tmp;
}

template<typename _Ty>
inline Node<_Ty>*& LinkedList<_Ty>::_search_second_half(unsigned int _Index)
{
	Node<_Ty>* tmp = _end;
	for (int i = _size; i > _Index + 1; --i) {
		tmp = tmp->prev;
	}
	return tmp;
}

template<typename _Ty>
inline void LinkedList<_Ty>::_copy_from(const LinkedList<_Ty>& _List)
{
	if (_List._size == 0) {
		_size = 0;
		_begin = NULL;
		_end = NULL;
		return;
	}

	auto it = _List.begin();
	_begin = new Node<_Ty>();
	_begin->value = *it;
	auto tmp = _begin;

	while (++it != _List.end()) {
		auto node = new Node<_Ty>();
		node->value = *it;
		node->prev = tmp;
		tmp->next = node;
		tmp = tmp->next;
	}
	_end = tmp;
	_size = _List._size;
}

template<typename _Ty>
inline void LinkedList<_Ty>::_destroy_data()
{
	if (!_begin) return;
	while (_begin != _end) {
		auto tmp = _end->prev;
		tmp->next = NULL;
		delete _end;
		_end = tmp;
	}
	delete _begin;
	_begin = NULL;
	_end = NULL;
}



//
//	ITERATOR
//

template<class _Ty>
struct LinkedListIterator
{
	friend class LinkedList<_Ty>;
public:
	LinkedListIterator(Node<_Ty>* _Ptr);

	_Ty& operator*();
	LinkedListIterator<_Ty>& operator++();
	LinkedListIterator<_Ty> operator++(int);

	bool operator==(const LinkedListIterator<_Ty>& _Right);
	bool operator!=(const LinkedListIterator<_Ty>& _Right);
private:

	Node<_Ty>* ptr;
};

template<class _Ty>
inline LinkedListIterator<_Ty>::LinkedListIterator(Node<_Ty>* _Ptr)
	: ptr(_Ptr)
{ }

template<class _Ty>
inline _Ty& LinkedListIterator<_Ty>::operator*()
{
	return this->ptr->value;
}

template<class _Ty>
inline LinkedListIterator<_Ty>& LinkedListIterator<_Ty>::operator++()
{
	ptr = ptr->next;
	return *this;
}

template<class _Ty>
inline LinkedListIterator<_Ty> LinkedListIterator<_Ty>::operator++(int)
{
	auto p = ptr;
	ptr = ptr->next;
	return *p;
}

template<class _Ty>
inline bool LinkedListIterator<_Ty>::operator==(const LinkedListIterator<_Ty>& _Right)
{
	return ptr == _Right.ptr;
}

template<class _Ty>
inline bool LinkedListIterator<_Ty>::operator!=(const LinkedListIterator<_Ty>& _Right)
{
	return ptr != _Right.ptr;
}