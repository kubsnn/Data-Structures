#pragma once
#include "Utility.h"

#pragma pack (push, 1)

template  <class _Ty>
struct ListNode;

template  <class _Ty>
struct LinkedListIterator;

template  <class _Ty>
class LinkedList
{
public:
	using iterator = LinkedListIterator<_Ty>;

	LinkedList();
	LinkedList(const LinkedList& _List);
	LinkedList(LinkedList&& _List);
	~LinkedList();
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
	bool pop_begin();
	bool pop_back();
	iterator find(const _Ty& _Val);


	constexpr iterator begin();
	constexpr const iterator begin() const;
	constexpr iterator end();
	constexpr const iterator end() const;

	size_t size() const;

	_Ty& back();
	const _Ty& back() const;
	_Ty& front();
	const _Ty& front() const;

	_Ty& operator[](unsigned int _Index);
	const _Ty& operator[](unsigned int _Index) const;

	LinkedList& operator=(const LinkedList& _List);
	LinkedList& operator=(LinkedList&& _List);


	bool operator<(const LinkedList<_Ty>& _Vec) const;
	constexpr bool operator<=(const LinkedList& _Vec) const;
	bool operator>(const LinkedList<_Ty>& _Vec) const;
	constexpr bool operator>=(const LinkedList& _Vec) const;

private:
	ListNode<_Ty>* _Begin;
	ListNode<_Ty>* _End;
	size_t _Size;

	void _Append_node(ListNode<_Ty>*& _Node);
	template <class ..._Values>
	void _Emplace_back(_Values&& ..._Vals) noexcept;
	template <class ..._Values>
	void _Emplace(unsigned int _Index, _Values&&... _Vals) noexcept;
	void _Insert_node(ListNode<_Ty>*& _Node, unsigned int _Index);
	ListNode<_Ty>*& _Search(unsigned int _Index);
	ListNode<_Ty>*& _Search_first_half(unsigned int _Index);
	ListNode<_Ty>*& _Search_second_half(unsigned int _Index);

	void _Copy_from(const LinkedList<_Ty>& _List);
	void _Destroy_data();
};

template <class _Ty>
inline LinkedList<_Ty>::LinkedList()
{
	_Size = 0;
	_Begin = NULL;
	_End = NULL;
}

template <class _Ty>
inline LinkedList<_Ty>::LinkedList(const LinkedList<_Ty>& _List)
{
	_Copy_from(_List);
}

template <class _Ty>
inline LinkedList<_Ty>::LinkedList(LinkedList<_Ty>&& _List)
{
	_Begin = move(_List._Begin);
	_End = move(_List._End);
	_Size = move(_List._Size);

	_List._Begin = NULL;
	_List._End = NULL;
	_List._Size = 0;
}

template <class _Ty>
inline LinkedList<_Ty>::~LinkedList()
{
	_Destroy_data();
}

template <class _Ty>
inline void LinkedList<_Ty>::append(const _Ty& _Val)
{
	_Emplace_back(_Val);
}

template <class _Ty>
inline void LinkedList<_Ty>::append(_Ty&& _Val) noexcept
{
	_Emplace_back(move<_Ty>(_Val));
}

template<class _Ty>
template<class _FwdIt>
inline void LinkedList<_Ty>::append(_FwdIt _First, const _FwdIt _Last)
{
	if (_First == _Last) return;

	_Emplace_back(*_First++);

	for (; _First != _Last; ++_First) {
		auto* node = new ListNode<_Ty>(*_First);
		_End->next = node;
		node->prev = _End;

		_End = node;
		++_Size;
	}
}

template <class _Ty>
template<class ..._Values>
inline void LinkedList<_Ty>::emplace_back(_Values&& ..._Vals) noexcept
{
	_Emplace_back(forward<_Values>(_Vals)...);
}

template<class _Ty>
template<class ..._Values>
inline void LinkedList<_Ty>::emplace(unsigned int _Index, _Values && ..._Vals) noexcept
{
	_Emplace(_Index, forward<_Values>(_Vals)...);
}

template <class _Ty>
inline void LinkedList<_Ty>::insert(unsigned int _Index, const _Ty& _Val)
{
	if (_Index == _Size) return append(_Val);

	auto next = new ListNode<_Ty>(_Val);

	_Insert_node(next, _Index);
}

template <class _Ty>
inline void LinkedList<_Ty>::insert(unsigned int _Index, _Ty&& _Val) noexcept
{
	if (_Index == _Size) return append(move(_Val));

	auto next = new ListNode<_Ty>(move(_Val));

	_Insert_node(next, _Index);
}

template <class _Ty>
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

template <class _Ty>
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

template <class _Ty>
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

template<class _Ty>
inline LinkedListIterator<_Ty> LinkedList<_Ty>::find(const _Ty& _Val)
{
	for (iterator it = begin(); it != end(); ++it) {
		if (*it == _Val) return it;
	}
	return end();
}

template <class _Ty>
inline constexpr LinkedListIterator<_Ty> LinkedList<_Ty>::begin()
{
	return LinkedListIterator<_Ty>(_Begin);
}

template <class _Ty>
inline constexpr const LinkedListIterator<_Ty> LinkedList<_Ty>::begin() const
{
	return LinkedListIterator<_Ty>(_Begin);
}

template <class _Ty>
inline constexpr LinkedListIterator<_Ty> LinkedList<_Ty>::end()
{
	return LinkedListIterator<_Ty>(_End->next);
}

template <class _Ty>
inline constexpr const LinkedListIterator<_Ty> LinkedList<_Ty>::end() const
{
	return LinkedListIterator<_Ty>(_End->next);
}

template <class _Ty>
inline size_t LinkedList<_Ty>::size() const
{
	return _Size;
}

template <class _Ty>
inline _Ty& LinkedList<_Ty>::back()
{
	return _End->value;
}

template <class _Ty>
inline const _Ty& LinkedList<_Ty>::back() const
{
	return _End->value;
}

template <class _Ty>
inline _Ty& LinkedList<_Ty>::front()
{
	return _Begin->value;
}

template <class _Ty>
inline const _Ty& LinkedList<_Ty>::front() const
{
	return _Begin->value;
}

template <class _Ty>
inline _Ty& LinkedList<_Ty>::operator[](unsigned int _Index)
{
	return _Search(_Index)->value;
}

template <class _Ty>
inline const _Ty& LinkedList<_Ty>::operator[](unsigned int _Index) const
{
	return _Search(_Index)->value;
}

template <class _Ty>
inline LinkedList<_Ty>& LinkedList<_Ty>::operator=(const LinkedList<_Ty>& _List)
{
	_Destroy_data();
	_Copy_from(_List);
	return *this;
}

template <class _Ty>
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

template<class _Ty>
inline bool LinkedList<_Ty>::operator<(const LinkedList<_Ty>& _Vec) const
{
	return ::lexicographical_compare(begin(), end(), _Vec.begin(), _Vec.end());
}

template<class _Ty>
inline constexpr bool LinkedList<_Ty>::operator<=(const LinkedList<_Ty>& _Vec) const
{
	return !(*this > _Vec);
}

template<class _Ty>
inline bool LinkedList<_Ty>::operator>(const LinkedList<_Ty>& _Vec) const
{
	return ::lexicographical_compare(_Vec.begin(), _Vec.end(), begin(), end());
}

template<class _Ty>
inline constexpr bool LinkedList<_Ty>::operator>=(const LinkedList<_Ty>& _Vec) const
{
	return !(*this < _Vec);
}

template <class _Ty>
inline void LinkedList<_Ty>::_Append_node(ListNode<_Ty>*& _Node)
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

template <class _Ty>
template <class ..._Values>
inline void LinkedList<_Ty>::_Emplace_back(_Values&& ..._Vals) noexcept
{
	ListNode<_Ty>* node = new ListNode<_Ty>(forward<_Values>(_Vals)...);

	if (_Size == 0) {
		_Begin = node;
		_End = node;
	}
	else {
		_End->next = node;
		node->prev = _End;

		_End = node;
	}
	++_Size;
}

template<class _Ty>
template<class ..._Values>
inline void LinkedList<_Ty>::_Emplace(unsigned int _Index, _Values && ..._Vals) noexcept
{
	if (_Index == _Size) {
		_Emplace_back(forward<_Values>(_Vals)...);
		return;
	}

	auto* _Tmp = _Search(_Index);

	auto* _Node = new ListNode<_Ty>(forward<_Values>(_Vals)...);
	auto prev = _Tmp->prev;
	_Tmp->prev = _Node;
	_Node->next = _Tmp;
	_Node->prev = prev;

	if (_Index == 0) _Begin = _Node;
	else _Node->prev->next = _Node;

	++_Size;
}


template <class _Ty>
inline void LinkedList<_Ty>::_Insert_node(ListNode<_Ty>*& _Node, unsigned int _Index)
{
	ListNode<_Ty>* _Tmp = _Search(_Index);

	auto prev = _Tmp->prev;
	_Tmp->prev = _Node;
	_Node->next = _Tmp;
	_Node->prev = prev;

	if (_Index == 0) _Begin = _Node;
	else _Node->prev->next = _Node;

	++_Size;
}

template <class _Ty>
inline ListNode<_Ty>*& LinkedList<_Ty>::_Search(unsigned int _Index)
{
	if (_Index <= _Size / 2) return _Search_first_half(_Index);
	return _Search_second_half(_Index);
}

template <class _Ty>
inline ListNode<_Ty>*& LinkedList<_Ty>::_Search_first_half(unsigned int _Index)
{
	ListNode<_Ty>* _Tmp = _Begin;
	for (int i = 0; i < _Index; ++i) {
		_Tmp = _Tmp->next;
	}
	return _Tmp;
}

template <class _Ty>
inline ListNode<_Ty>*& LinkedList<_Ty>::_Search_second_half(unsigned int _Index)
{
	ListNode<_Ty>* _Tmp = _End;
	for (int i = _Size; i > _Index + 1; --i) {
		_Tmp = _Tmp->prev;
	}
	return _Tmp;
}

template <class _Ty>
inline void LinkedList<_Ty>::_Copy_from(const LinkedList<_Ty>& _List)
{
	if (_List._Size == 0) {
		_Size = 0;
		_Begin = NULL;
		_End = NULL;
		return;
	}

	auto it = _List.begin();
	_Begin = new ListNode<_Ty>(*it);

	auto _Tmp = _Begin;

	while (++it != _List.end()) {
		auto node = new ListNode<_Ty>(*it);
		node->prev = _Tmp;
		_Tmp->next = node;
		_Tmp = _Tmp->next;
	}
	_End = _Tmp;
	_Size = _List._Size;
}

template <class _Ty>
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

///
///  LIST NODE
/// 

template  <class _Ty>
struct ListNode
{
public:
	ListNode* next = NULL;
	ListNode* prev = NULL;
	_Ty value;
	ListNode()
	{ }
	ListNode(const _Ty& _Val)
		: value(_Val)
	{ }
	ListNode(_Ty&& _Val)
		: value(move(_Val))
	{ }
	ListNode(const ListNode& other)
		: value(other.value)
	{
		next = other.next;
		prev = other.prev;
	}
	ListNode(ListNode&& other)
		: value(move(other.value))
	{
		next = other.next;
		prev = other.prev;

		other.next = NULL;
		other.prev = NULL;
	}
	ListNode& operator=(const ListNode& other)
	{
		_Copy_from(other);
		return *this;
	}
	ListNode& operator=(ListNode&& other)
	{
		next = other.next;
		prev = other.prev;
		value.~_Ty();
		new (&value) _Ty(move(value));

		other.next = NULL;
		other.prev = NULL;
		return *this;
	}
private:
	constexpr void _Copy_from(const ListNode& other)
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
struct LinkedListIterator
{
	friend class LinkedList<_Ty>;
public:
	LinkedListIterator(ListNode<_Ty>* _Ptr);

	_Ty& operator*();
	LinkedListIterator<_Ty>& operator++();
	LinkedListIterator<_Ty> operator++(int);

	bool operator==(const LinkedListIterator<_Ty>& _Right);
	bool operator!=(const LinkedListIterator<_Ty>& _Right);
private:

	ListNode<_Ty>* _List;
};

template<class _Ty>
inline LinkedListIterator<_Ty>::LinkedListIterator(ListNode<_Ty>* _Ptr)
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

#pragma pack(pop)