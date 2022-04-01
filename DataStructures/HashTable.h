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
	
	bool remove(const _TKey& _Key);


	HashTableIterator<_TKey, _TValue> begin();
	const HashTableIterator<_TKey, _TValue> begin() const;
	HashTableIterator<_TKey, _TValue> end();
	const HashTableIterator<_TKey, _TValue> end() const;

	_TValue& operator[](const _TKey& _Key);
	const _TValue& operator[](const _TKey& _Key) const;

	HashTable<_TKey, _TValue>& operator=(const HashTable<_TKey, _TValue>& _Table);
	HashTable<_TKey, _TValue>& operator=(HashTable<_TKey, _TValue>&& _Table);
private:
	const size_t MAX_SIZE = 2048*2048;
	size_t _Mask;
	LinkedList<HashPair<_TKey, _TValue>>** _buckets;

	void _insert(HashPair<_TKey, _TValue>& _Pair);
	size_t bucket_index(const _TKey& _Key) const;
	void _copy_from(const HashTable<_TKey, _TValue>& _Table);
	void _clear();
};

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable()
{
	_Mask = MAX_SIZE - 1;
	_buckets = new LinkedList<HashPair<_TKey, _TValue>>* [MAX_SIZE];
	memset(_buckets, 0, sizeof(LinkedList<HashPair<_TKey, _TValue>>*) * MAX_SIZE);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(const HashTable<_TKey, _TValue>& _Table)
{
	_buckets = new LinkedList<HashPair<_TKey, _TValue>>*[MAX_SIZE];
	_copy_from(_Table);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(HashTable<_TKey, _TValue>&& _Table)
{
	_buckets = move(_Table._buckets);
	_Mask = move(_Table._Mask);
	_Table._buckets = NULL;
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
	size_t index = bucket_index(_Key);

	for (auto& e : *_buckets[index]) {
		if (e.key == _Key) return e.value;
	}
}

template<class _TKey, class _TValue>
inline bool HashTable<_TKey, _TValue>::remove(const _TKey& _Key)
{
	size_t index = bucket_index(_Key);

	if (!_buckets[index]) return false;

	auto it = _buckets[index]->begin();
	int i = 0;
	for (; i < _buckets[index]->size(); ++i) {
		if ((*it).key == _Key) break;
		++it;
	}

	_buckets[index]->remove_at(i);

	if (_buckets[index]->size() == 0) {
		delete _buckets[index];
		_buckets[index] = NULL;
	}

	return true;
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::begin()
{
	return HashTableIterator<_TKey, _TValue>(_buckets, MAX_SIZE); 
}

template<class _TKey, class _TValue>
inline const HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::begin() const
{
	return HashTableIterator<_TKey, _TValue>(_buckets, MAX_SIZE);
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::end()
{
	return HashTableIterator<_TKey, _TValue>(_buckets + MAX_SIZE, 0);
}

template<class _TKey, class _TValue>
inline const HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::end() const
{
	return HashTableIterator<_TKey, _TValue>(_buckets + MAX_SIZE, 0);
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
	_buckets = new LinkedList<HashPair<_TKey, _TValue>>* [MAX_SIZE];
	_copy_from(_Table);
	return *this;
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>& HashTable<_TKey, _TValue>::operator=(HashTable<_TKey, _TValue>&& _Table)
{
	_clear();
	_buckets = move(_Table._buckets);
	_Table._buckets = NULL;
	return *this;
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_insert(HashPair<_TKey, _TValue>& _Pair)
{
	size_t index = bucket_index(_Pair.key);

	if (!_buckets[index]) _buckets[index] = new LinkedList<HashPair<_TKey, _TValue>>();
		
	_buckets[index]->append(move(_Pair));
}

template<class _TKey, class _TValue>
inline size_t HashTable<_TKey, _TValue>::bucket_index(const _TKey& _Key) const
{
	return Hash<_TKey>()(_Key) & _Mask;
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_copy_from(const HashTable<_TKey, _TValue>& _Table)
{
	_Mask = _Table._Mask;
	memset(_buckets, 0, sizeof(LinkedList<HashPair<_TKey, _TValue>>*) * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE; ++i) {
		if (_Table._buckets[i]) {
			_buckets[i] = new LinkedList<HashPair<_TKey, _TValue>>();
			*(_buckets[i]) = *_Table._buckets[i];
		}
	}
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_clear()
{
	if (!_buckets) return;

	for (int i = 0; i < MAX_SIZE; ++i) {
		if (_buckets[i]) {
			delete _buckets[i];
			_buckets[i] = NULL;
		}
	}

	delete[] _buckets;
	_buckets = NULL;
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
	left = (int)_Size;
	if (left > 0 && !*ptr) {
		tmp = -1;
		find_next_value();
	}
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
	while (*ptr == NULL) {
		if (left == 0) return;
		--left;
		++ptr;
	}
	if (left <= 0) return;

	int bucket_size = (*ptr)->size();

	if (tmp + 1 < bucket_size) {
		tmp++;
		return;
	}

	left--;
	tmp = -1;
	++ptr;
	find_next_value();
}
