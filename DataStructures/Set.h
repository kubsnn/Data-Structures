#pragma once

#include "Utility.h"
#include "Hash.h"
#include "HashPair.h"
#include "HashTable.h"

struct SetEmptyEl{};

template <class _TValue>
struct SetIterator;

template <class _TValue>
class Set : protected HashTable<_TValue, SetEmptyEl>
{
public:
	using const_iterator = SetIterator<_TValue>;

	Set();
	Set(const Set& _Set);
	Set(Set&& _Set) noexcept;

	constexpr const_iterator begin() const;
	constexpr const_iterator end() const;

	void insert(const _TValue& _Val);
	void insert(_TValue&& _Val) noexcept;

	bool remove(const _TValue& _Val);

	const_iterator find(const _TValue& _Key) const;

	Set& operator=(const Set& _Table);
	Set& operator=(Set&& _Table) noexcept;

	bool operator==(const Set& _Set);

};

template<class _TValue>
inline Set<_TValue>::Set()
	: HashTable<_TValue, SetEmptyEl>()
{ }

template<class _TValue>
inline Set<_TValue>::Set(const Set& _Set)
	: HashTable<_TValue, SetEmptyEl>(_Set)
{ }

template<class _TValue>
inline Set<_TValue>::Set(Set&& _Set) noexcept
	: HashTable<_TValue, SetEmptyEl>(move(_Set))
{ }

template<class _TValue>
inline constexpr Set<_TValue>::const_iterator Set<_TValue>::begin() const
{
	return const_iterator(this->_Buckets, this->_BucketCount);
}

template<class _TValue>
inline constexpr Set<_TValue>::const_iterator Set<_TValue>::end() const
{
	return const_iterator(this->_Buckets + this->_BucketCount, 0);
}

template<class _TValue>
inline void Set<_TValue>::insert(const _TValue & _Val)
{
	HashTable<_TValue, SetEmptyEl>::insert(_Val, SetEmptyEl{});
}

template<class _TValue>
inline void Set<_TValue>::insert(_TValue&& _Val) noexcept
{
	HashTable<_TValue, SetEmptyEl>::insert(move(_Val), SetEmptyEl{});
}

template<class _TValue>
inline bool Set<_TValue>::remove(const _TValue& _Val)
{
	return HashTable<_TValue, SetEmptyEl>::remove(_Val);
}

template<class _TValue>
inline Set<_TValue>::const_iterator Set<_TValue>::find(const _TValue& _Key) const
{
	size_t index = this->_Bucket_index(_Key);

	if (this->_Buckets[index] == NULL) return this->end();

	size_t counter = 0;
	for (auto& e : *this->_Buckets[index]) {
		if (e.key == _Key) {
			const_iterator it(&this->_Buckets[index], counter);
			while (counter > 0) ++it;
			return it;
		}
		++counter;
	}
	return this->end();
}

template<class _TValue>
inline Set<_TValue>& Set<_TValue>::operator=(const Set& _Set)
{
	this->_Clear();
	this->_Buckets = new LinkedList<HashPair<_TValue, SetEmptyEl>>*[_Set._BucketCount];
	this->_Copy_from(_Set);
	return *this;
}

template<class _TValue>
inline Set<_TValue>& Set<_TValue>::operator=(Set&& _Set) noexcept
{
	this->_Clear();
	this->_BucketCount = move(_Set._BucketCount);
	this->_Count = move(_Set._Count);
	this->_Mask = move(_Set._Mask);
	this->_Buckets = move(_Set._Buckets);
	_Set._Buckets = NULL;
	_Set._Count = 0;
	_Set._BucketCount = 0;
	return *this;
}

template<class _TValue>
inline bool Set<_TValue>::operator==(const Set& _Set)
{
	return HashTable<_TValue, SetEmptyEl>::operator==(_Set);
}

///
///	 ITERATOR
/// 

template <class _TValue>
struct SetIterator : public HashTableIterator<_TValue, SetEmptyEl>
{
public:
	using HashTableIterator<_TValue, SetEmptyEl>::HashTableIterator;
	friend class Set<_TValue>;
	const _TValue& operator*();
};

template<class _TValue>
inline const _TValue& SetIterator<_TValue>::operator*()
{
	return HashTableIterator<_TValue, SetEmptyEl>::operator*().key;
}

template<class _TValue>
struct Hash<Set<_TValue>>
{
	size_t operator()(const Set<_TValue>& _Table)
	{
		size_t hash = 0;
		size_t power = 1;
		const int mod = 1e9 + 7;
		for (const auto& e : _Table) {
			hash = (hash + Hash<_TValue>()(e) * power) % mod;

			power = (power * 31) % mod;
		}

		return hash;
	}
};