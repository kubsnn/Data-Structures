 #pragma once

#include "Hash.h"
#include "Utility.h"
#include "LinkedList.h"

template<class _TKey, class _TValue>
struct HashTableIterator;

template<class _TKey, class _TValue>
struct HashPair
{
	_TKey key;
	_TValue value;
};

template<class _TKey, class _TValue>
class HashTable
{
public:
	HashTable();
	HashTable(const HashTable<_TKey, _TValue>& _Table);
	HashTable(HashTable<_TKey, _TValue>&& _Table);
	~HashTable();


	void insert(const _TKey& _Key, const _TValue& _Val);
	void insert(const _TKey& _Key, _TValue&& _Val);
	void insert(_TKey&& _Key, _TValue&& _Val);

	_TValue& find(const _TKey& _Key);

	HashTableIterator<_TKey, _TValue> begin();
	const HashTableIterator<_TKey, _TValue> begin() const;
	HashTableIterator<_TKey, _TValue> end();
	const HashTableIterator<_TKey, _TValue> end() const;

	_TValue& operator[](const _TKey& _Key);
	const _TValue& operator[](const _TKey& _Key) const;

	HashTable<_TKey, _TValue>& operator=(const HashTable<_TKey, _TValue>& _Table);
	HashTable<_TKey, _TValue>& operator=(HashTable<_TKey, _TValue>&& _Table);
private:
	const size_t MAX_SIZE = 997;
	LinkedList<HashPair<_TKey, _TValue>>** data;

	void _insert(HashPair<_TKey, _TValue>& _Pair);
	void _copy_from(const HashTable<_TKey, _TValue>& _Table);
	void _clear();
};

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable()
{
	data = new LinkedList<HashPair<_TKey, _TValue>>* [MAX_SIZE];
	memset(data, 0, sizeof(LinkedList<HashPair<_TKey, _TValue>>*) * MAX_SIZE);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(const HashTable<_TKey, _TValue>& _Table)
{
	_copy_from(_Table);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(HashTable<_TKey, _TValue>&& _Table)
{
	data = move(_Table.data);
	_Table.data = NULL;
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::~HashTable()
{
	_clear();
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::insert(const _TKey& _Key, const _TValue& _Val)
{
	HashPair<_TKey, _TValue> pair;
	pair.key = _Key;
	pair.value = _Val;

	_insert(pair);
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::insert(const _TKey& _Key, _TValue&& _Val)
{
	HashPair<_TKey, _TValue> pair;
	pair.key = _Key;
	pair.value = move(_Val);

	_insert(pair);
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::insert(_TKey&& _Key, _TValue&& _Val)
{
	HashPair<_TKey, _TValue> pair;
	pair.key = move(_Key);
	pair.value = move(_Val);

	_insert(pair);
}

template<class _TKey, class _TValue>
inline _TValue& HashTable<_TKey, _TValue>::find(const _TKey& _Key)
{
	size_t hash = Hash<_TKey>()(_Key) % MAX_SIZE;

	for (auto& e : *data[hash]) {
		if (e.key == _Key) return e.value;
	}
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::begin()
{
	return HashTableIterator<_TKey, _TValue>(data, MAX_SIZE); 
}

template<class _TKey, class _TValue>
inline const HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::begin() const
{
	return HashTableIterator<_TKey, _TValue>(data, MAX_SIZE);
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::end()
{
	return HashTableIterator<_TKey, _TValue>(data + MAX_SIZE, 0);
}

template<class _TKey, class _TValue>
inline const HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::end() const
{
	return HashTableIterator<_TKey, _TValue>(data + MAX_SIZE, 0);
}

template<class _TKey, class _TValue>
inline _TValue& HashTable<_TKey, _TValue>::operator[](const _TKey& _Key)
{
	return find(_Key);
}

template<class _TKey, class _TValue>
inline const _TValue& HashTable<_TKey, _TValue>::operator[](const _TKey& _Key) const
{
	return find(_Key);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>& HashTable<_TKey, _TValue>::operator=(const HashTable<_TKey, _TValue>& _Table)
{
	_clear();
	data = new LinkedList<HashPair<_TKey, _TValue>>* [MAX_SIZE];
	_copy_from(_Table);
	return *this;
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>& HashTable<_TKey, _TValue>::operator=(HashTable<_TKey, _TValue>&& _Table)
{
	_clear();
	data = move(_Table.data);
	_Table.data = NULL;
	return *this;
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_insert(HashPair<_TKey, _TValue>& _Pair)
{
	size_t hash = Hash<_TKey>()(_Pair.key) % MAX_SIZE;

	if (!data[hash]) data[hash] = new LinkedList<HashPair<_TKey, _TValue>>();
		
	data[hash]->append(move(_Pair));
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_copy_from(const HashTable<_TKey, _TValue>& _Table)
{
	for (int i = 0; i < MAX_SIZE; ++i) {
		if (_Table.data[i]) {
			data[i] = new LinkedList<HashPair<_TKey, _TValue>>();
			*(data[i]) = *_Table.data[i];
		}
	}
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_clear()
{
	if (data) delete[] data;
}

//
//	ITERATOR
//

template<class _TKey, class _TValue>
struct HashTableIterator
{
	friend class HashTable<_TKey, _TValue>;
public:
	HashTableIterator(LinkedList<HashPair<_TKey, _TValue>>** _List, size_t _Size);

	HashPair<_TKey, _TValue>& operator*();
	HashTableIterator<_TKey, _TValue>& operator++();
	HashTableIterator<_TKey, _TValue> operator++(int);

	bool operator==(const HashTableIterator<_TKey, _TValue>& _Right);
	bool operator!=(const HashTableIterator<_TKey, _TValue>& _Right);
private:
	LinkedList<HashPair<_TKey, _TValue>>** ptr;
	int left;
	int tmp = 0;

	void find_next_value();
};

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue>::HashTableIterator(LinkedList<HashPair<_TKey, _TValue>>** _List, size_t _Size)
	: ptr(_List)
{
	left = _Size;
	find_next_value();
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>& HashTableIterator<_TKey, _TValue>::operator*()
{
	return (*ptr)->operator[](tmp);
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue>& HashTableIterator<_TKey, _TValue>::operator++()
{
	find_next_value();
	return *this;
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTableIterator<_TKey, _TValue>::operator++(int)
{
	auto p = ptr;
	find_next_value();
	return *p;
}

template<class _TKey, class _TValue>
inline bool HashTableIterator<_TKey, _TValue>::operator==(const HashTableIterator<_TKey, _TValue>& _Right)
{
	return ptr == _Right.ptr;
}

template<class _TKey, class _TValue>
inline bool HashTableIterator<_TKey, _TValue>::operator!=(const HashTableIterator<_TKey, _TValue>& _Right)
{
	return ptr != _Right.ptr;
}
template<class _TKey, class _TValue>
void HashTableIterator<_TKey, _TValue>::find_next_value()
{
	while (left > 0 && *ptr == NULL) {
		++ptr;
		--left;
	}
	if (left <= 0) return;
	if ((*ptr)->size() >= 1) {
		
		if (tmp < (*ptr)->size()) {
			tmp++;
			return;
		} else {
			--left;
		}
	}
	tmp = 0;
	++ptr;

	find_next_value();
}
