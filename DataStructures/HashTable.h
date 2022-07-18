 #pragma once

#include "Utility.h"
#include "Hash.h"
#include "compressed_pair.h"
#include "forward_list.h"

#pragma pack (push, 1)

template<class _Ty>
struct HashTableIterator;


template<class _TKey, class _TValue>
class HashTable
{
public:
	using pair = compressed_pair<const _TKey, _TValue>;
	using bucket_type = forward_list<pair>*;
	using iterator = HashTableIterator<pair>;
	using const_iterator = HashTableIterator<const pair>;

	HashTable();
	HashTable(const HashTable& _Table);
	HashTable(HashTable&& _Table) noexcept;
	~HashTable();

	constexpr void insert(const _TKey& _Key, const _TValue& _Val);
	constexpr void insert(const _TKey& _Key, _TValue&& _Val);
	constexpr void insert(_TKey&& _Key, _TValue&& _Val) noexcept;
	template <class ..._Values>
	constexpr void emplace(const _TKey& _Key, _Values&& ..._Vals) noexcept;
	template <class ..._Values>
	constexpr void emplace(_TKey&& _Key, _Values&&... _Vals) noexcept;

	constexpr _TValue& find(const _TKey& _Key);
	constexpr const _TValue& find(const _TKey& _Key) const;
	
	constexpr bool remove(const _TKey& _Key);

	constexpr size_t size() const;
	constexpr size_t bucket_count() const;

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	constexpr _TValue& operator[](const _TKey& _Key);
	constexpr const _TValue& operator[](const _TKey& _Key) const;

	constexpr HashTable& operator=(const HashTable& _Table);
	constexpr HashTable& operator=(HashTable&& _Table) noexcept;

	constexpr bool operator==(const HashTable& _Table) const;
	
	constexpr void clear();
protected:
	size_t _Count = 0;
	size_t _BucketCount = 8;
	size_t _Mask;
	bucket_type* _Buckets;

	constexpr void _Insert(compressed_pair<const _TKey, _TValue>& _Pair);
	constexpr size_t _Bucket_index(const _TKey& _Key) const;

	constexpr void _Try_resize();
	constexpr void _Rehash(size_t _NewSize);

	constexpr void _Copy_from(const HashTable<_TKey, _TValue>& _Table);
	constexpr void _Clear();
};

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable()
{
	_Mask = _BucketCount - 1;
	_Buckets = new bucket_type [_BucketCount];
	memset(_Buckets, 0, sizeof(bucket_type) * _BucketCount);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(const HashTable<_TKey, _TValue>& _Table)
{
	_Buckets = new bucket_type[_Table._BucketCount];
	_Copy_from(_Table);
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::HashTable(HashTable<_TKey, _TValue>&& _Table) noexcept
{
	_BucketCount = _Table._BucketCount;
	_Count = _Table._Count;
	_Mask = _Table._Mask;
	_Buckets = _Table._Buckets;
	_Table._Buckets = NULL;
}

template<class _TKey, class _TValue>
inline HashTable<_TKey, _TValue>::~HashTable()
{
	_Clear();
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::insert(const _TKey& _Key, const _TValue& _Val)
{
	compressed_pair<const _TKey, _TValue> pair(_Key, _Val);

	_Insert(pair);
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::insert(const _TKey& _Key, _TValue&& _Val)
{
	compressed_pair<const _TKey, _TValue> pair(_Key, move(_Val));

	_Insert(pair);
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::insert(_TKey&& _Key, _TValue&& _Val) noexcept
{
	compressed_pair<const _TKey, _TValue> pair(move(_Key), move(_Val));

	_Insert(pair);
}

template<class _TKey, class _TValue>
template<class ..._Values>
inline constexpr void HashTable<_TKey, _TValue>::emplace(const _TKey& _Key, _Values&&... _Vals) noexcept
{
	compressed_pair<const _TKey, _TValue> pair(_Key, forward<_Values>(_Vals)...);

	_Insert(pair);
}

template<class _TKey, class _TValue>
template<class ..._Values>
inline constexpr void HashTable<_TKey, _TValue>::emplace(_TKey&& _Key, _Values&&... _Vals) noexcept
{
	compressed_pair<const _TKey, _TValue> pair(move(_Key), forward<_Values>(_Vals)...);

	_Insert(pair);
}

template<class _TKey, class _TValue>
inline constexpr _TValue& HashTable<_TKey, _TValue>::find(const _TKey& _Key)
{
	size_t index = _Bucket_index(_Key);

	for (auto& e : *_Buckets[index]) {
		if (e._First == _Key) return e._Second;
	}
}

template<class _TKey, class _TValue>
inline constexpr const _TValue& HashTable<_TKey, _TValue>::find(const _TKey& _Key) const
{
	size_t index = _Bucket_index(_Key);

	for (auto& e : *_Buckets[index]) {
		if (e._First == _Key) return e._Second;
	}
}

template<class _TKey, class _TValue>
inline constexpr bool HashTable<_TKey, _TValue>::remove(const _TKey& _Key)
{
	size_t index = _Bucket_index(_Key);

	if (!_Buckets[index]) return false;

	auto it = _Buckets[index]->begin();
	int i = 0;
	for (; i < _Buckets[index]->size(); ++i) {
		if ((*it)._First == _Key) break;
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
inline constexpr size_t HashTable<_TKey, _TValue>::size() const
{
	return _Count;
}

template<class _TKey, class _TValue>
inline constexpr size_t HashTable<_TKey, _TValue>::bucket_count() const
{
	return _BucketCount;
}

template<class _TKey, class _TValue>
inline constexpr HashTable<_TKey, _TValue>::iterator HashTable<_TKey, _TValue>::begin()
{
	return iterator(_Buckets, _BucketCount);
}

template<class _TKey, class _TValue>
inline constexpr HashTable<_TKey, _TValue>::const_iterator HashTable<_TKey, _TValue>::begin() const
{
	return const_iterator(_Buckets, _BucketCount);
}

template<class _TKey, class _TValue>
inline constexpr HashTable<_TKey, _TValue>::iterator HashTable<_TKey, _TValue>::end()
{
	return iterator(_Buckets + _BucketCount, 0);
}

template<class _TKey, class _TValue>
inline constexpr HashTable<_TKey, _TValue>::const_iterator HashTable<_TKey, _TValue>::end() const
{
	return const_iterator(_Buckets + _BucketCount, 0);
}

template<class _TKey, class _TValue>
inline constexpr _TValue& HashTable<_TKey, _TValue>::operator[](const _TKey& _Key)
{
	return find(_Key);
}

template<class _TKey, class _TValue>
inline constexpr const _TValue& HashTable<_TKey, _TValue>::operator[](const _TKey& _Key) const
{
	return find(_Key);
}

template<class _TKey, class _TValue>
inline constexpr HashTable<_TKey, _TValue>& HashTable<_TKey, _TValue>::operator=(const HashTable<_TKey, _TValue>& _Table)
{
	_Clear();
	_Buckets = new forward_list<compressed_pair<_TKey, _TValue>>* [_Table._BucketCount];
	_Copy_from(_Table);
	return *this;
}

template<class _TKey, class _TValue>
inline constexpr HashTable<_TKey, _TValue>& HashTable<_TKey, _TValue>::operator=(HashTable<_TKey, _TValue>&& _Table) noexcept
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
inline constexpr bool HashTable<_TKey, _TValue>::operator==(const HashTable& _Table) const
{
	if (_Count != _Table._Count) return false;

	auto _KeysIdx1 = new _TKey[_Count];
	auto _KeysIdx2 = new _TKey[_Count];

	for (size_t i = 0, j = 0; j < _Count; ++i) {
		if (_Buckets[i]) for (const auto& e : *_Buckets[i]) {
			_KeysIdx1[j++] = e._First;
		}
	}
	for (size_t i = 0, j = 0; j < _Table._Count; ++i) {
		if (_Table._Buckets[i]) for (const auto& e : *_Table._Buckets[i]) {
			_KeysIdx2[j++] = e._First;
		}
	}
	if (_BucketCount != _Table._BucketCount) {
		sort(_KeysIdx1, _KeysIdx1 + _Count);
		sort(_KeysIdx2, _KeysIdx2 + _Count);
	}

	bool result = arrays_compare(_KeysIdx1, _KeysIdx1 + _Count, _KeysIdx2, _KeysIdx2 + _Count);

	delete[] _KeysIdx1;
	delete[] _KeysIdx2;

	return result;
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::clear()
{
	_Clear();
	_BucketCount = 8;
	_Mask = _BucketCount - 1;
	_Count = 0;
	_Buckets = new bucket_type [_BucketCount];
	memset(_Buckets, 0, sizeof(bucket_type) * _BucketCount);
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::_Insert(compressed_pair<const _TKey, _TValue>& _Pair)
{
	_Try_resize();
	size_t index = _Bucket_index(_Pair.first());

	if (!_Buckets[index]) _Buckets[index] = new forward_list<compressed_pair<const _TKey, _TValue>>();
	else if (_Buckets[index]->find(_Pair) != _Buckets[index]->end()) return;

	_Buckets[index]->push_front(move(_Pair));
	++_Count;
}

template<class _TKey, class _TValue>
inline constexpr size_t HashTable<_TKey, _TValue>::_Bucket_index(const _TKey& _Key) const
{
	return Hash<_TKey>()(_Key) & _Mask;
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::_Try_resize()
{
	if (_Count > _BucketCount << 2) _Rehash(_BucketCount << 2);
	else if (_Count > 8 && _Count < _BucketCount / 3) _Rehash(_BucketCount >> 2);
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::_Rehash(size_t _NewSize)
{
	auto _Prev_buckets = _Buckets;

	size_t _OldSize = _BucketCount;

	_BucketCount = _NewSize;
	_Mask = _BucketCount - 1;

	_Buckets = new bucket_type [_BucketCount];
	::fill(_Buckets, _Buckets + _BucketCount, (bucket_type)NULL);

	for (int i = 0; i < _OldSize; ++i) {
		if (_Prev_buckets[i]) {
			for (auto& _El : *_Prev_buckets[i]) {
				size_t index = _Bucket_index(_El.first());
				if (!_Buckets[index]) _Buckets[index] = new forward_list<compressed_pair<const _TKey, _TValue>>();
				_Buckets[index]->push_front(move(_El));
			}
			delete _Prev_buckets[i];
		}
	}
	delete[] _Prev_buckets;
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::_Copy_from(const HashTable<_TKey, _TValue>& _Table)
{
	_Mask = _Table._Mask;
	_BucketCount = _Table._BucketCount;
	_Count = _Table._Count;

	memset(_Buckets, 0, sizeof(bucket_type) * _BucketCount);
	for (int i = 0; i < _BucketCount; ++i) {
		if (_Table._Buckets[i]) {
			_Buckets[i] = new forward_list<compressed_pair<const _TKey, _TValue>>(*_Table._Buckets[i]);
		}
	}
}

template<class _TKey, class _TValue>
inline constexpr void HashTable<_TKey, _TValue>::_Clear()
{
	if (!_Buckets) return;

	for (int i = 0; i < _BucketCount; ++i) {
		if (_Buckets[i]) {
			delete _Buckets[i];
		}
	}

	delete[] _Buckets;
	_Buckets = NULL;
}




//
//	ITERATOR
//

template <class _Ty>
struct HashTableIterator
{
public:
	using bucket_type = forward_list<remove_const_t<_Ty>>*;
	using first_type = typename _Ty::first_type;
	using second_type = typename _Ty::second_type;
	using iterator = HashTableIterator<_Ty>;
	friend class HashTable<remove_const_t<first_type>, remove_const_t<second_type>>;
	HashTableIterator(bucket_type* _List, size_t _Size);

	_Ty& operator*();
	iterator& operator++();
	iterator operator++(int);

	constexpr bool operator==(const iterator& _Right) const;
	constexpr bool operator!=(const iterator& _Right) const;
private:
	bucket_type* _List;
	int _BucketsLeft;
	int _CurrentBucketIndex = 0;

	constexpr void find_next_value();
};

template <class _Ty>
inline HashTableIterator<_Ty>::HashTableIterator(bucket_type* _List, size_t _Size)
	: _List(_List)
{
	_BucketsLeft = (int)_Size;
	if (_BucketsLeft > 0 && !*_List) {
		_CurrentBucketIndex = -1;
		find_next_value();
	}
}

template <class _Ty>
inline _Ty& HashTableIterator<_Ty>::operator*()
{
	return (*_List)->at(_CurrentBucketIndex);
}

template <class _Ty>
inline HashTableIterator<_Ty>& HashTableIterator<_Ty>::operator++()
{
	find_next_value();
	return *this;
}

template <class _Ty>
inline HashTableIterator<_Ty> HashTableIterator<_Ty>::operator++(int)
{
	auto p = *this;
	find_next_value();
	return p;
}

template <class _Ty>
inline constexpr bool HashTableIterator<_Ty>::operator==(const iterator& _Right) const
{
	return _List == _Right._List;
}

template <class _Ty>
inline constexpr bool HashTableIterator<_Ty>::operator!=(const iterator& _Right) const
{
	return _List != _Right._List;
}

template<class _Ty>
constexpr void HashTableIterator<_Ty>::find_next_value()
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
		for (const auto& [key, value] : _Table) {
			hash = (hash + Hash<_TKey>()(key) * power) % mod;
			hash = (hash + Hash<_TValue>()(value) * power) % mod;

			power = (power * 31) % mod;
		}

		return hash;
	}
};

#pragma pack (pop)