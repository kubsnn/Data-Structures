#pragma once

#pragma pack (push, 1)

template <class _Ty1, class _Ty2, bool = __is_empty(_Ty2)>
struct compressed_pair : private _Ty2
{
public:
	compressed_pair() 
	{ }
	compressed_pair(const compressed_pair& _Other)
		: _First(_Other._First)
	{ }
	compressed_pair(compressed_pair&& _Other) noexcept
		: _First(move(_Other._First))
	{ }
	compressed_pair(const _Ty1& _X, const _Ty2& _Y) 
		: _First(_X) 
	{ }
	compressed_pair(const _Ty1& _X, _Ty2&& _Y)
		: _First(_X)
	{ }
	compressed_pair(_Ty1&& _X, const _Ty2& _Y)
		: _First(move(_X))
	{ }
	compressed_pair(_Ty1&& _X, _Ty2&& _Y)
		: _First(move(_X))
	{ }
	constexpr _Ty1& first() 
	{ 
		return _First; 
	}
	constexpr const _Ty1& first() const
	{
		return _First;
	}
	constexpr _Ty2& second() 
	{ 
		return *this;
	}
	constexpr const _Ty2& second() const
	{
		return *this;
	}
	constexpr bool operator==(const compressed_pair& _Right) const
	{
		return _First == _Right._First;
	}
	constexpr bool operator!=(const compressed_pair& _Right) const
	{
		return _First != _Right._First;
	}
	constexpr bool operator>(const compressed_pair& _Right) const
	{
		return _First > _Right._First;
	}
	constexpr bool operator<(const compressed_pair& _Right) const
	{
		return _First < _Right._First;
	}
	constexpr bool operator>=(const compressed_pair& _Right) const
	{
		return _First >= _Right._First;
	}
	constexpr bool operator<=(const compressed_pair& _Right) const
	{
		return _First <= _Right._First;
	}
	constexpr compressed_pair& operator=(const compressed_pair& _Right)
	{
		_First = _Right._First;
		return *this;
	}
	constexpr compressed_pair& operator=(compressed_pair&& _Right) noexcept
	{
		_First = move(_Right._First);
		return *this;
	}

	_Ty1 _First;
};

template <class _Ty1, class _Ty2>
struct compressed_pair<_Ty1, _Ty2, false>
{
public:
	compressed_pair()
	{ }
	compressed_pair(const compressed_pair& _Other)
		: _First(_Other._First)
		, _Second(_Other._Second)
	{ }
	compressed_pair(compressed_pair&& _Other) noexcept
		: _First(move(_Other._First))
		, _Second(move(_Other._Second))
	{ }
	compressed_pair(const _Ty1& _X, const _Ty2& _Y) 
		: _First(_X)
		, _Second(_Y) 
	{ }
	compressed_pair(const _Ty1& _X, _Ty2&& _Y) 
		: _First(_X)
		, _Second(move(_Y))
	{ }
	compressed_pair(_Ty1&& _X, const _Ty2& _Y)
		: _First(move(_X))
		, _Second(_Y)
	{ }
	compressed_pair(_Ty1&& _X, _Ty2&& _Y)
		: _First(move(_X))
		, _Second(move(_Y))
	{ }
	constexpr _Ty1& first() 
	{ 
		return _First; 
	}
	constexpr const _Ty1& first() const
	{
		return _First;
	}
	constexpr _Ty2& second() 
	{
		return _Second; 
	}
	constexpr const _Ty2& second() const
	{
		return _Second;
	}
	constexpr bool operator==(const compressed_pair& _Right) const
	{
		return _First == _Right._First;
	}
	constexpr bool operator!=(const compressed_pair& _Right) const
	{
		return _First != _Right._First;
	}
	constexpr bool operator>(const compressed_pair& _Right) const
	{
		return _First > _Right._First;
	}
	constexpr bool operator<(const compressed_pair& _Right) const
	{
		return _First < _Right._First;
	}
	constexpr bool operator>=(const compressed_pair& _Right) const
	{
		return _First >= _Right._First;
	}
	constexpr bool operator<=(const compressed_pair& _Right) const
	{
		return _First <= _Right._First;
	}
	constexpr compressed_pair& operator=(const compressed_pair& _Right)
	{
		_First = _Right._First;
		_Second = _Right._Second;
		return *this;
	}
	constexpr compressed_pair& operator=(compressed_pair&& _Right) noexcept
	{
		_First = move(_Right._First);
		_Second = move(_Right._Second);
		return *this;
	}

	_Ty1 _First;
	_Ty2 _Second;
};

#pragma pack (pop)