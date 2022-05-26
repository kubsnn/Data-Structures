 #pragma once

#include "Utility.h"
#include "Hash.h"
#include "HashPair.h"
#include "LinkedList.h"


template<class _TKey, class _TValue>
struct HashTableIterator;


template<class _TKey, class _TValue>
class HashTable
{
public:
	using iterator = HashTableIterator<_TKey, _TValue>;

	HashTable();
	HashTable(const HashTable<_TKey, _TValue>& _Table);
	HashTable(HashTable<_TKey, _TValue>&& _Table);
	~HashTable();

	void insert(const _TKey& _Key, const _TValue& _Val);
	void insert(const _TKey& _Key, _TValue&& _Val);
	void insert(_TKey&& _Key, _TValue&& _Val);

	_TValue& find(const _TKey& _Key);
	
	bool remove(const _TKey& _Key);

	size_t bucket_count() const;

	iterator begin();
	const iterator begin() const;
	iterator end();
	const iterator end() const;

	_TValue& operator[](const _TKey& _Key);
	const _TValue& operator[](const _TKey& _Key) const;

	HashTable<_TKey, _TValue>& operator=(const HashTable<_TKey, _TValue>& _Table);
	HashTable<_TKey, _TValue>& operator=(HashTable<_TKey, _TValue>&& _Table) noexcept;

private:
	size_t _Count = 0;
	size_t _BucketCount = 8;
	size_t _Mask;
	LinkedList<HashPair<_TKey, _TValue>>** _Buckets;

	void _Insert(HashPair<_TKey, _TValue>& _Pair);
	size_t _Bucket_index(const _TKey& _Key) const;

	void _Try_resize();
	void _Rehash(size_t _NewSize);

	size_t _Get_expanded_size() const;
	size_t _Get_shrinked_size() const;

	void _Copy_from(const HashTable<_TKey, _TValue>& _Table);
	void _Clear();
};

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable()
{
	_Mask = _BucketCount - 1;
	_Buckets = new LinkedList<HashPair<_TKey, _TValue>>* [_BucketCount];
	memset(_Buckets, 0, sizeof(LinkedList<HashPair<_TKey, _TValue>>*) * _BucketCount);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(const HashTable<_TKey, _TValue>& _Table)
{
	_Buckets = new LinkedList<HashPair<_TKey, _TValue>>*[_BucketCount];
	_Copy_from(_Table);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(HashTable<_TKey, _TValue>&& _Table)
{
	_BucketCount = move(_Table._BucketCount);
	_Count = move(_Table._Count);
	_Mask = move(_Table._Mask);
	_Buckets = move(_Table._Buckets);
	_Table._Buckets = NULL;
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::~HashTable()
{
	_Clear();
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::insert(const _TKey& _Key, const _TValue& _Val)
{
	HashPair<_TKey, _TValue> pair(_Key, _Val);
	++_Count;

	_Insert(pair);
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::insert(const _TKey& _Key, _TValue&& _Val)
{
	HashPair<_TKey, _TValue> pair(_Key, move(_Val));
	++_Count;

	_Insert(pair);
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::insert(_TKey&& _Key, _TValue&& _Val)
{
	HashPair<_TKey, _TValue> pair(move(_Key), move(_Val));
	++_Count;

	_Insert(pair);
}

template<class _TKey, class _TValue>
inline _TValue& HashTable<_TKey, _TValue>::find(const _TKey& _Key)
{
	size_t index = _Bucket_index(_Key);

	for (auto& e : *_Buckets[index]) {
		if (e.key == _Key) return e.value;
	}
}

template<class _TKey, class _TValue>
inline bool HashTable<_TKey, _TValue>::remove(const _TKey& _Key)
{
	size_t index = _Bucket_index(_Key);

	if (!_Buckets[index]) return false;

	auto it = _Buckets[index]->begin();
	int i = 0;
	for (; i < _Buckets[index]->size(); ++i) {
		if ((*it).key == _Key) break;
		++it;
	}

	_Buckets[index]->remove_at(i);

	if (_Buckets[index]->size() == 0) {
		delete _Buckets[index];
		_Buckets[index] = NULL;
	}

	--_Count;
	return true;
}

template<class _TKey, class _TValue>
inline size_t HashTable<_TKey, _TValue>::bucket_count() const
{
	return _BucketCount;
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::begin()
{
	return iterator(_Buckets, _BucketCount);
}

template<class _TKey, class _TValue>
inline const HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::begin() const
{
	return iterator(_Buckets, _BucketCount);
}

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::end()
{
	return iterator(_Buckets + _BucketCount, 0);
}

template<class _TKey, class _TValue>
inline const HashTableIterator<_TKey, _TValue> HashTable<_TKey, _TValue>::end() const
{
	return iterator(_Buckets + _BucketCount, 0);
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
	_Clear();
	_Buckets = new LinkedList<HashPair<_TKey, _TValue>>* [_Table._BucketCount];
	_Copy_from(_Table);
	return *this;
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>& HashTable<_TKey, _TValue>::operator=(HashTable<_TKey, _TValue>&& _Table) noexcept
{
	_Clear();
	_BucketCount = move(_Table._BucketCount);
	_Count = move(_Table._Count);
	_Mask = move(_Table._Mask);
	_Buckets = move(_Table._Buckets);
	_Table._Buckets = NULL;
	return *this;
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_Insert(HashPair<_TKey, _TValue>& _Pair)
{
	_Try_resize();
	size_t index = _Bucket_index(_Pair.key);

	if (!_Buckets[index]) _Buckets[index] = new LinkedList<HashPair<_TKey, _TValue>>();
		
	_Buckets[index]->append(move(_Pair));
}

template<class _TKey, class _TValue>
inline size_t HashTable<_TKey, _TValue>::_Bucket_index(const _TKey& _Key) const
{
	return Hash<_TKey>()(_Key) & _Mask;
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_Try_resize()
{
	auto _Bigger_size = _Get_expanded_size();
	if (_Count > 2 * _BucketCount) _Rehash(_Bigger_size);

	auto _Smaller_size = _Get_shrinked_size();
	if (_Count > 8 && _Count < _Smaller_size) _Rehash(_Smaller_size);
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_Rehash(size_t _NewSize)
{
	auto _Prev_buckets = _Buckets;

	size_t _OldSize = _BucketCount;

	_BucketCount = _NewSize;
	_Mask = _BucketCount - 1;

	_Buckets = new LinkedList<HashPair<_TKey, _TValue>>* [_BucketCount];
	memset(_Buckets, 0, _BucketCount * sizeof(LinkedList<HashPair<_TKey, _TValue>>*));

	for (int i = 0; i < _OldSize; ++i) {
		if (_Prev_buckets[i]) {
			for (auto& _El : *_Prev_buckets[i]) {
				auto _New_pair = move(_El);
				_Insert(_New_pair);
			}
			delete _Prev_buckets[i];
			_Prev_buckets[i] = NULL;
		}
	}
	delete[] _Prev_buckets;
}

template<class _TKey, class _TValue>
inline size_t HashTable<_TKey, _TValue>::_Get_expanded_size() const
{
	return _BucketCount < 512 ? _BucketCount << 3 : _BucketCount << 1;
}

template<class _TKey, class _TValue>
inline size_t HashTable<_TKey, _TValue>::_Get_shrinked_size() const
{
	return _BucketCount <= 512 ? _BucketCount >> 3 : _BucketCount >> 1;
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_Copy_from(const HashTable<_TKey, _TValue>& _Table)
{
	_Mask = _Table._Mask;
	_BucketCount = _Table._BucketCount;
	_Count = _Table._Count;

	memset(_Buckets, 0, sizeof(LinkedList<HashPair<_TKey, _TValue>>*) * _BucketCount);
	for (int i = 0; i < _BucketCount; ++i) {
		if (_Table._Buckets[i]) {
			_Buckets[i] = new LinkedList<HashPair<_TKey, _TValue>>();
			*(_Buckets[i]) = *_Table._Buckets[i];
		}
	}
}

template<class _TKey, class _TValue>
inline void HashTable<_TKey, _TValue>::_Clear()
{
	if (!_Buckets) return;

	for (int i = 0; i < _BucketCount; ++i) {
		if (_Buckets[i]) {
			delete _Buckets[i];
			_Buckets[i] = NULL;
		}
	}

	delete[] _Buckets;
	_Buckets = NULL;
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
	LinkedList<HashPair<_TKey, _TValue>>** _List;
	int _BucketsLeft;
	int _CurrentBucketIndex = 0;

	void find_next_value();
};

template<class _TKey, class _TValue>
inline HashTableIterator<_TKey, _TValue>::HashTableIterator(LinkedList<HashPair<_TKey, _TValue>>** _List, size_t _Size)
	: _List(_List)
{
	_BucketsLeft = (int)_Size;
	if (_BucketsLeft > 0 && !*_List) {
		_CurrentBucketIndex = -1;
		find_next_value();
	}
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>& HashTableIterator<_TKey, _TValue>::operator*()
{
	return (*_List)->operator[](_CurrentBucketIndex);
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
	auto p = _List;
	find_next_value();
	return *p;
}

template<class _TKey, class _TValue>
inline bool HashTableIterator<_TKey, _TValue>::operator==(const HashTableIterator<_TKey, _TValue>& _Right)
{
	return _List == _Right._List;
}

template<class _TKey, class _TValue>
inline bool HashTableIterator<_TKey, _TValue>::operator!=(const HashTableIterator<_TKey, _TValue>& _Right)
{
	return _List != _Right._List;
}

template<class _TKey, class _TValue>
void HashTableIterator<_TKey, _TValue>::find_next_value()
{
	while (*_List == NULL) {
		if (_BucketsLeft == 0) return;
		--_BucketsLeft;
		++_List;
	}
	if (_BucketsLeft <= 0) return;

	int bucket_size = (*_List)->size();

	if (_CurrentBucketIndex + 1 < bucket_size) {
		_CurrentBucketIndex++;
		return;
	}

	--_BucketsLeft;
	_CurrentBucketIndex = -1;
	++_List;
	find_next_value();
}

template<class _TKey, class _TValue>
struct Hash<HashTable<_TKey, _TValue>>
{
	size_t operator()(const HashTable<_TKey, _TValue>& _Table) 
	{
		size_t hash = 0;
		size_t power = 1;
		const int mod = 1e9 + 7;
		for (const auto& e : _Table) {
			hash = (hash + Hash<_TKey>()(e.key) * power) % mod;
			hash = (hash + Hash<_TValue>()(e.value) * power) % mod;

			power = (power * 31) % mod;
		}

		return hash;
	}
};

