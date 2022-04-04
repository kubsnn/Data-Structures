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

	_Ty& back();
	const _Ty& back() const;
	_Ty& front();
	const _Ty& front() const;

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	LinkedList<_Ty>& operator=(const LinkedList<_Ty>& _List);
	LinkedList<_Ty>& operator=(LinkedList<_Ty>&& _List);

private:
	Node<_Ty>* _Begin;
	Node<_Ty>* _End;
	size_t _Size;

	void _Append_node(Node<_Ty>*& _Node);
	void _Insert_node(Node<_Ty>*& _Node, unsigned int _Index);
	Node<_Ty>*& _Search(unsigned int _Index);
	Node<_Ty>*& _Search_first_half(unsigned int _Index);
	Node<_Ty>*& _Search_second_half(unsigned int _Index);

	void _Copy_from(const LinkedList<_Ty>& _List);
	void _Destroy_data();
};

template<typename _Ty>
inline LinkedList<_Ty>::LinkedList()
{
	_Size = 0;
	_Begin = NULL;
	_End = NULL;
}

template<typename _Ty>
inline LinkedList<_Ty>::LinkedList(const LinkedList<_Ty>& _List)
{
	_Copy_from(_List);
}

template<typename _Ty>
inline LinkedList<_Ty>::LinkedList(LinkedList<_Ty>&& _List)
{
	_Begin = move(_List._Begin);
	_End = move(_List._End);
	_Size = move(_List._Size);

	_List._Begin = NULL;
	_List._End = NULL;
	_List._Size = 0;
}

template<typename _Ty>
inline LinkedList<_Ty>::~LinkedList()
{
	_Destroy_data();
}

template<typename _Ty>
inline void LinkedList<_Ty>::append(const _Ty& _Val)
{
	auto node = new Node<_Ty>();
	node->value = _Val;

	_Append_node(node);
}

template<typename _Ty>
inline void LinkedList<_Ty>::append(_Ty&& _Val)
{
	auto node = new Node<_Ty>();
	node->value = move(_Val);

	_Append_node(node);
}

template<typename _Ty>
inline void LinkedList<_Ty>::insert(const _Ty& _Val, unsigned int _Index)
{
	if (_Index == _Size) return append(_Val);

	auto next = new Node<_Ty>();
	next->value = const_cast<_Ty&>(_Val);

	_Insert_node(next, _Index);
}

template<typename _Ty>
inline void LinkedList<_Ty>::insert(_Ty&& _Val, unsigned int _Index)
{
	if (_Index == _Size) return append(move(_Val));

	auto next = new Node<_Ty>();
	next->value = move(_Val);

	_Insert_node(next, _Index);
}

template<typename _Ty>
inline bool LinkedList<_Ty>::remove_at(unsigned int _Index)
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

template<typename _Ty>
inline bool LinkedList<_Ty>::pop_begin()
{
	if (_Size > 1) {
		auto next = _Begin->next;
		auto node = _Begin;

		delete node;
		_Begin = next;
		--_Size;
		return true;
	}
	return pop_back();
}

template<typename _Ty>
inline bool LinkedList<_Ty>::pop_back()
{
	if (_Size > 1) {
		auto prev = _End->prev;
		auto node = _End;
		delete node;
		_End = prev;
	}
	else if (_Size == 1) {
		delete _Begin;
		_Begin = NULL;
		_End = NULL;
	}
	else {
		return false;
	}
	--_Size;
	return true;
}

template<typename _Ty>
inline LinkedListIterator<_Ty> LinkedList<_Ty>::begin()
{
	return LinkedListIterator<_Ty>(_Begin);
}

template<typename _Ty>
inline const LinkedListIterator<_Ty> LinkedList<_Ty>::begin() const
{
	return LinkedListIterator<_Ty>(_Begin);
}

template<typename _Ty>
inline LinkedListIterator<_Ty> LinkedList<_Ty>::end()
{
	return LinkedListIterator<_Ty>(_End->next);
}

template<typename _Ty>
inline const LinkedListIterator<_Ty> LinkedList<_Ty>::end() const
{
	return LinkedListIterator<_Ty>(_End->next);
}

template<typename _Ty>
inline size_t LinkedList<_Ty>::size() const
{
	return _Size;
}

template<typename _Ty>
inline _Ty& LinkedList<_Ty>::back()
{
	return _End->value;
}

template<typename _Ty>
inline const _Ty& LinkedList<_Ty>::back() const
{
	return _End->value;
}

template<typename _Ty>
inline _Ty& LinkedList<_Ty>::front()
{
	return _Begin->value;
}

template<typename _Ty>
inline const _Ty& LinkedList<_Ty>::front() const
{
	return _Begin->value;
}

template<typename _Ty>
inline _Ty& LinkedList<_Ty>::operator[](unsigned int _Index)
{
	return _Search(_Index)->value;
}

template<typename _Ty>
inline const _Ty& LinkedList<_Ty>::operator[](unsigned int _Index) const
{
	return _Search(_Index)->value;
}

template<typename _Ty>
inline LinkedList<_Ty>& LinkedList<_Ty>::operator=(const LinkedList<_Ty>& _List)
{
	_Destroy_data();
	_Copy_from(_List);
	return *this;
}

template<typename _Ty>
inline LinkedList<_Ty>& LinkedList<_Ty>::operator=(LinkedList<_Ty>&& _List)
{
	_Destroy_data();

	_Begin = move(_List._Begin);
	_End = move(_List._End);
	_Size = move(_List._Size);

	_List._Begin = NULL;
	_List._End = NULL;
	_List._Size = 0;
	return *this;
}


template<typename _Ty>
inline void LinkedList<_Ty>::_Append_node(Node<_Ty>*& _Node)
{
	if (_Size == 0) {
		_Begin = _Node;
		_End = _Node;
	}
	else {
		_End->next = _Node;
		_Node->prev = _End;

		_End = _Node;
	}
	++_Size;
}

template<typename _Ty>
inline void LinkedList<_Ty>::_Insert_node(Node<_Ty>*& _Node, unsigned int _Index)
{
	Node<_Ty>* _Tmp = _Search(_Index);

	auto prev = _Tmp->prev;
	_Tmp->prev = _Node;
	_Node->next = _Tmp;
	_Node->prev = prev;

	if (_Index == 0) _Begin = _Node;
	else _Node->prev->next = _Node;

	++_Size;
}

template<typename _Ty>
inline Node<_Ty>*& LinkedList<_Ty>::_Search(unsigned int _Index)
{
	if (_Index <= _Size / 2) return _Search_first_half(_Index);
	return _Search_second_half(_Index);
}

template<typename _Ty>
inline Node<_Ty>*& LinkedList<_Ty>::_Search_first_half(unsigned int _Index)
{
	Node<_Ty>* _Tmp = _Begin;
	for (int i = 0; i < _Index; ++i) {
		_Tmp = _Tmp->next;
	}
	return _Tmp;
}

template<typename _Ty>
inline Node<_Ty>*& LinkedList<_Ty>::_Search_second_half(unsigned int _Index)
{
	Node<_Ty>* _Tmp = _End;
	for (int i = _Size; i > _Index + 1; --i) {
		_Tmp = _Tmp->prev;
	}
	return _Tmp;
}

template<typename _Ty>
inline void LinkedList<_Ty>::_Copy_from(const LinkedList<_Ty>& _List)
{
	if (_List._Size == 0) {
		_Size = 0;
		_Begin = NULL;
		_End = NULL;
		return;
	}

	auto it = _List.begin();
	_Begin = new Node<_Ty>();
	_Begin->value = *it;
	auto _Tmp = _Begin;

	while (++it != _List.end()) {
		auto node = new Node<_Ty>();
		node->value = *it;
		node->prev = _Tmp;
		_Tmp->next = node;
		_Tmp = _Tmp->next;
	}
	_End = _Tmp;
	_Size = _List._Size;
}

template<typename _Ty>
inline void LinkedList<_Ty>::_Destroy_data()
{
	if (!_Begin) return;
	while (_Begin != _End) {
		auto _Tmp = _End->prev;
		_Tmp->next = NULL;
		delete _End;
		_End = _Tmp;
	}

	delete _Begin;
	_Begin = NULL;
	_End = NULL;
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

	Node<_Ty>* _List;
};

template<class _Ty>
inline LinkedListIterator<_Ty>::LinkedListIterator(Node<_Ty>* _Ptr)
	: _List(_Ptr)
{ }

template<class _Ty>
inline _Ty& LinkedListIterator<_Ty>::operator*()
{
	return this->_List->value;
}

template<class _Ty>
inline LinkedListIterator<_Ty>& LinkedListIterator<_Ty>::operator++()
{
	_List = _List->next;
	return *this;
}

template<class _Ty>
inline LinkedListIterator<_Ty> LinkedListIterator<_Ty>::operator++(int)
{
	auto p = _List;
	_List = _List->next;
	return *p;
}

template<class _Ty>
inline bool LinkedListIterator<_Ty>::operator==(const LinkedListIterator<_Ty>& _Right)
{
	return _List == _Right._List;
}

template<class _Ty>
inline bool LinkedListIterator<_Ty>::operator!=(const LinkedListIterator<_Ty>& _Right)
{
	return _List != _Right._List;
}