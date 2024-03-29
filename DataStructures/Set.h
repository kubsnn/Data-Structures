#pragma once

#include "utility.h"
#include "Hash.h"
#include "compressed_pair.h"
#include "HashTable.h"


#pragma pack (push, 1)

struct SetEmptyEl{};

template <class _Ty>
struct set_const_iterator;

template <class _Ty>
class set : protected hashtable<_Ty, SetEmptyEl>
{
public:
	using base = hashtable<_Ty, SetEmptyEl>;
	using const_iterator = set_const_iterator<const _Ty>;
	using value_type = _Ty;

	set();
	set(const set& _Set);
	set(set&& _Set) noexcept;

	void insert(const _Ty& _Val);
	void insert(_Ty&& _Val) noexcept;
	template <class _FwdIt>
	void insert(_FwdIt _First, const _FwdIt _Last);
	template <class ..._Values>
	void emplace(_Values&&... _Vals);

	bool remove(const _Ty& _Val);

	const_iterator find(const _Ty& _Key) const;

	constexpr const_iterator begin() const;
	constexpr const_iterator end() const;

	set& operator=(const set& _Table);
	set& operator=(set&& _Table) noexcept;

	bool operator==(const set& _Set) const;

	void clear();
};

template<class _Ty>
inline set<_Ty>::set()
	: base()
{ }

template<class _Ty>
inline set<_Ty>::set(const set& _Set)
	: base(_Set)
{ }

template<class _Ty>
inline set<_Ty>::set(set&& _Set) noexcept
	: base(move(_Set))
{ }

template<class _Ty>
inline constexpr set_const_iterator<const _Ty> set<_Ty>::begin() const
{
	return const_iterator(this->_Buckets, this->_BucketCount);
}

template<class _Ty>
inline constexpr set_const_iterator<const _Ty> set<_Ty>::end() const
{
	return const_iterator(this->_Buckets + this->_BucketCount, 0);
}

template<class _Ty>
inline void set<_Ty>::insert(const _Ty& _Val)
{
	base::insert(_Val, SetEmptyEl{});
}

template<class _Ty>
inline void set<_Ty>::insert(_Ty&& _Val) noexcept
{
	base::insert(move(_Val), SetEmptyEl{});
}

template<class _Ty>
template<class _FwdIt>
inline void set<_Ty>::insert(_FwdIt _First, const _FwdIt _Last)
{
	for (; _First != _Last; ++_First) {
		base::insert(*_First, SetEmptyEl{});
	}
}

template<class _Ty>
template<class ..._Values>
inline void set<_Ty>::emplace(_Values&&... _Vals)
{
	base::insert(_Ty(forward<_Values>(_Vals)...), SetEmptyEl{});
}


template<class _Ty>
inline bool set<_Ty>::remove(const _Ty& _Val)
{
	return base::remove(_Val);
}

template<class _Ty>
inline set_const_iterator<const _Ty> set<_Ty>::find(const _Ty& _Key) const
{
	size_t index = this->_Bucket_index(_Key);

	if (this->_Buckets[index] == NULL) return this->end();

	const auto _End = end();
	for (const_iterator it = begin(); it != _End; ++it) {
		if (*it == _Key) return it;
	}

	return this->end();
}

template<class _Ty>
inline set<_Ty>& set<_Ty>::operator=(const set& _Set)
{
	base::_Clear();
	base::_Buckets = new forward_list<compressed_pair<_Ty, SetEmptyEl>>*[_Set._BucketCount];
	base::_Copy_from(_Set);
	return *this;
}

template<class _Ty>
inline set<_Ty>& set<_Ty>::operator=(set&& _Set) noexcept
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

template<class _Ty>
inline bool set<_Ty>::operator==(const set& _Set) const
{
	return base::operator==(_Set);
}

template<class _Ty>
inline void set<_Ty>::clear()
{
	base::clear();
}

///
///	 ITERATOR
/// 

template <class _Ty>
struct set_const_iterator : public hashtable_iterator<compressed_pair<_Ty, SetEmptyEl>>
{
public:
	using base = hashtable_iterator<compressed_pair<_Ty, SetEmptyEl>>;
	using base::base;
	friend class set<_Ty>;
	const _Ty& operator*();
	set_const_iterator<_Ty>& operator++();
	set_const_iterator<_Ty> operator++(int);
};

template<class _Ty>
inline const _Ty& set_const_iterator<_Ty>::operator*()
{
	return base::operator*().first();
}

template<class _Ty>
inline set_const_iterator<_Ty>& set_const_iterator<_Ty>::operator++()
{
	base::operator++();
	return *this;
}

template<class _Ty>
inline set_const_iterator<_Ty> set_const_iterator<_Ty>::operator++(int)
{
	auto p = *this;
	base::operator++();
	return p;
}

template<class _Ty>
struct Hash<set<_Ty>>
{
	size_t operator()(const set<_Ty>& _Set)
	{
		size_t hash = 0;
		size_t power = 1;
		constexpr int mod = static_cast<int>(1e9) + 7;
		Hash<_Ty> hasher;
		for (const auto& e : _Set) {
			hash = (hash + hasher(e) * power) % mod;

			power = (power * 31) % mod;
		}

		return hash;
	}
};

#pragma pack (pop)