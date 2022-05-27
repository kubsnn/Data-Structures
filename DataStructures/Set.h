#pragma once

#include "Utility.h"
#include "Hash.h"
#include "HashPair.h"
#include "HashTable.h"

struct SetEmptyEl{};

template <class _TValue>
struct ConstSetIterator;

template <class _TValue>
class Set : protected HashTable<_TValue, SetEmptyEl>
{
public:
	using const_iterator = ConstSetIterator<_TValue>;

	Set();
	Set(const Set& _Set);
	Set(Set&& _Set) noexcept;

	void insert(const _TValue& _Val);
	void insert(_TValue&& _Val) noexcept;
	template <class _FwdIt>
	void insert(_FwdIt _First, const _FwdIt _Last);
	template <class ..._Values>
	void emplace(_Values&&... _Vals);

	bool remove(const _TValue& _Val);

	const_iterator find(const _TValue& _Key) const;

	constexpr const_iterator begin() const;
	constexpr const_iterator end() const;

	Set& operator=(const Set& _Table);
	Set& operator=(Set&& _Table) noexcept;

	bool operator==(const Set& _Set) const;

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
template<class _FwdIt>
inline void Set<_TValue>::insert(_FwdIt _First, const _FwdIt _Last)
{
	for (; _First != _Last; ++_First) {
		HashTable<_TValue, SetEmptyEl>::insert(*_First, SetEmptyEl{});
	}
}

template<class _TValue>
template<class ..._Values>
inline void Set<_TValue>::emplace(_Values&&... _Vals)
{
	HashTable<_TValue, SetEmptyEl>::insert(_TValue(forward<_Values>(_Vals)...), SetEmptyEl{});
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

	const auto _End = end();
	for (const_iterator it = begin(); it != _End; ++it) {
		if (*it == _Key) return it;
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
inline bool Set<_TValue>::operator==(const Set& _Set) const
{
	return HashTable<_TValue, SetEmptyEl>::operator==(_Set);
}

///
///	 ITERATOR
/// 

template <class _TValue>
struct ConstSetIterator : public HashTableIterator<_TValue, SetEmptyEl>
{
public:
	using HashTableIterator<_TValue, SetEmptyEl>::HashTableIterator;
	friend class Set<_TValue>;
	const _TValue& operator*();
	ConstSetIterator<_TValue>& operator++();
	ConstSetIterator<_TValue> operator++(int);
};

template<class _TValue>
inline const _TValue& ConstSetIterator<_TValue>::operator*()
{
	return HashTableIterator<_TValue, SetEmptyEl>::operator*().key;
}

template<class _TValue>
inline ConstSetIterator<_TValue>& ConstSetIterator<_TValue>::operator++()
{
	HashTableIterator<_TValue, SetEmptyEl>::operator++();
	return *this;
}

template<class _TValue>
inline ConstSetIterator<_TValue> ConstSetIterator<_TValue>::operator++(int)
{
	auto p = *this;
	HashTableIterator<_TValue, SetEmptyEl>::operator++();
	return p;
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

