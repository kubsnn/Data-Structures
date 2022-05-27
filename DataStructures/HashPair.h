#pragma once

template<class _TKey, class _TValue>
struct HashPair
{
	_TKey key;
	_TValue value;
	HashPair();
	HashPair(const HashPair<_TKey, _TValue>& _Pair);
	HashPair(HashPair<_TKey, _TValue>&& _Pair);
	HashPair(const _TKey& _Key, const _TValue& _Value);
	HashPair(const _TKey& _Key, _TValue&& _Value);
	HashPair(_TKey&& _Key, const _TValue& _Value);
	HashPair(_TKey&& _Key, _TValue&& _Value);
	HashPair<_TKey, _TValue>& operator=(const HashPair<_TKey, _TValue>& _Right);
	HashPair<_TKey, _TValue>& operator=(HashPair<_TKey, _TValue>&& _Right) noexcept;
	bool operator==(const HashPair& _Right);
	bool operator!=(const HashPair& _Right);
};

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair()
	: key(_TKey{}), value(_TValue{})
{
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair(const HashPair<_TKey, _TValue>& _Pair)
{
	key = _Pair.key;
	value = _Pair.value;
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair(const _TKey& _Key, const _TValue& _Value)
{
	key = _Key;
	value = _Value;
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair(const _TKey& _Key, _TValue&& _Value)
{
	key = _Key;
	value = move(_Value);
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair(_TKey&& _Key, const _TValue& _Value)
{
	key = move(_Key);
	value = _Value;
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair(HashPair<_TKey, _TValue>&& _Pair)
{
	key = move(_Pair.key);
	value = move(_Pair.value);
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>::HashPair(_TKey&& _Key, _TValue&& _Value)
{
	key = move(_Key);
	value = move(_Value);
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>& HashPair<_TKey, _TValue>::operator=(const HashPair<_TKey, _TValue>& _Right)
{
	key = _Right.key;
	value = _Right.value;
	return *this;
}

template<class _TKey, class _TValue>
inline HashPair<_TKey, _TValue>& HashPair<_TKey, _TValue>::operator=(HashPair<_TKey, _TValue>&& _Right) noexcept
{
	key = move(_Right.key);
	value = move(_Right.value);
	return *this;
}

template<class _TKey, class _TValue>
inline bool HashPair<_TKey, _TValue>::operator==(const HashPair& _Right)
{
	return key == _Right.key;
}

template<class _TKey, class _TValue>
inline bool HashPair<_TKey, _TValue>::operator!=(const HashPair& _Right)
{
	return key != _Right.key;
}