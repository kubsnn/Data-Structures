#pragma once

#include "utility.h"
#include "rbtree.h"

template <class _TKey, class _TValue>
struct map_pair;

template <class _TKey, class _TValue>
class map : public rbtree<map_pair<const _TKey, _TValue>>
{
public:
	using pair = map_pair<const _TKey, _TValue>;
	using base = rbtree<pair>;
	using value_type = pair;

	_TValue& operator[](const _TKey& _Key);
	const _TValue& operator[](const _TKey& _Key) const;

	void insert(const _TKey& _Key, const _TValue& _Val);
	void insert(const _TKey& _Key, _TValue&& _Val);
	void insert(_TKey&& _Key, _TValue&& _Val);
	template <class ..._Values>
	void emplace(const _TKey& _Key, _Values&&... _Vals);
};

template <class _TKey, class _TValue>
struct map_pair
{
	friend class map<_TKey, _TValue>;
	map_pair() : key(), value() { }
	map_pair(const _TKey& _Key, const _TValue& _Val)
		: key(_Key), value(_Val)
	{ }
	map_pair(const _TKey& _Key, _TValue&& _Val)
		: key(_Key), value(move(_Val))
	{ }
	map_pair(_TKey&& _Key, const _TValue& _Val)
		: key(move(_Key)), value(_Val)
	{ }
	map_pair(_TKey&& _Key, _TValue&& _Val)
		: key(move(_Key)), value(move(_Val))
	{ }
	template <class ...Args>
	map_pair(const _TKey& _Key, Args&&... _Args) 
		: key(_Key), value(forward<Args>(_Args)...)
	{ }
	map_pair(map_pair&& _Other)
		: key(move(_Other.key)), value(move(_Other.value))
	{ }

	bool operator==(const map_pair& _Other) const {
		return key == _Other.key;
	}
	bool operator!=(const map_pair& _Other) const {
		return key != _Other.key;
	}
	bool operator>(const map_pair& _Other) const {
		return key > _Other.key;
	}
	bool operator<(const map_pair& _Other) const {
		return key < _Other.key;
	}

	map_pair& operator=(const map_pair& _Other) {
		key = _Other.key;
		value = _Other.value;
		return *this;
	}
	map_pair& operator=(map_pair&& _Other) {
		key = move(_Other.key);
		value = move(_Other.value);
		return *this;
	}

	_TKey key;
	_TValue value;
};

template<class _TKey, class _TValue>
inline _TValue& map<_TKey, _TValue>::operator[](const _TKey& _Key)
{
	return base::find(pair(_Key, {} ))->value;
}

template<class _TKey, class _TValue>
inline const _TValue& map<_TKey, _TValue>::operator[](const _TKey& _Key) const
{
	return base::find(pair(_Key, {} ))->value;
}

template<class _TKey, class _TValue>
inline void map<_TKey, _TValue>::insert(const _TKey& _Key, const _TValue& _Val)
{
	base::_Emplace(pair({ _Key, _Val }));
}

template<class _TKey, class _TValue>
inline void map<_TKey, _TValue>::insert(const _TKey& _Key, _TValue&& _Val)
{
	base::_Emplace(pair({ _Key, move(_Val) }));
}

template<class _TKey, class _TValue>
inline void map<_TKey, _TValue>::insert(_TKey&& _Key, _TValue&& _Val)
{
	base::_Emplace(pair({ move(_Key), move(_Val) }));
}

template<class _TKey, class _TValue>
template<class ..._Values>
inline void map<_TKey, _TValue>::emplace(const _TKey& _Key, _Values&& ..._Vals)
{
	base::_Emplace(pair({ _Key, forward<_Values>(_Vals)... }));
}
