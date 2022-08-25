#pragma once

#include "memory.h"

#if __CPPVER >= 201703L

#include "cycle.h"
#include "enumerate.h"
#include "remove_if.h"
#include "take.h"
#include "transform.h"

namespace pipeline
{
    template <class _Container, class _Fun>
    constexpr auto operator|(_Container& _C, _Fun _Fn) {
        return _Fn(_C);
    }
    template <class _Container, class _Fun, class _Arg>
    constexpr auto operator|(_Container&& _C, _Fun _Fn) {
        return _Fn(move(_C));
    }
    template <class _Container, class _Fun, class _Arg>
    constexpr auto operator|(_Container& _C,  _Pipe_obj_arg<_Fun, _Arg> _Fn) {
        return _Fn._Fn(_C, _Fn._Arg);
    }
    template <class _Container, class _Fun, class _Arg>
    constexpr auto operator|(_Container&& _C, _Pipe_obj_arg<_Fun, _Arg> _Fn) {
        return _Fn._Fn(move(_C), _Fn._Arg);
    }
}

#endif

template<class _Ty = void>
struct less
{
    template <class _Ty1, class _Ty2>
    constexpr bool operator()(_Ty1&& _Left, _Ty2&& _Right) const
    {
        return static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right);
    }
};

template<class _Ty = void>
struct greater
{
    template <class _Ty1, class _Ty2>
    constexpr bool operator()(_Ty1&& _Left, _Ty2&& _Right) const
    {
        return static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right);
    }
};

//sortowanie przez wstawianie
template <class _Ty, class _Pr>
constexpr void insertion_sort(_Ty* const& _Data, size_t _Size, _Pr _Pred) {
    for (int i = 1, j; i < _Size; ++i) {
        _Ty _Tmp = _Data[i];
        for (j = i - 1; j >= 0 && _Pred(_Data[j], _Tmp); --j) {
            _Data[j + 1] = _Data[j];
        }
        _Data[j + 1] = _Tmp;
    }
}

template<class _Ty>
constexpr int median(_Ty* const& _Data, const _Ty& a, const _Ty& b, const _Ty& c) {
    _Ty x = _Data[a] - _Data[b], y = _Data[b] - _Data[c], z = _Data[a] - _Data[c];
    return x * y > 0 ? b : x * z > 0 ? c : a;
}

template<class _Ty, class _Pr>
constexpr int partition(_Ty* const& _Data, int left, int right, _Pr _Pred) {
    _Ty pivot = _Data[(right + left) / 2];
    int i = left, j = right;
    while (true) {
        while (_Pred(_Data[j], pivot)) j--;
        while (!_Pred(_Data[i], pivot)) i++;

        if (i < j) {
            swap(_Data[i], _Data[j]);
            i++; j--;
        }
        else {
            return j;
        }
    }
}

template <class _Ty, class _Pr>
constexpr void heapify(_Ty* const& _Data, int i, size_t n, _Pr _Pred) {
    int root = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && _Pred(_Data[left], _Data[root])) root = left;
    if (right < n&& _Pred(_Data[right], _Data[root])) root = right;

    if (root != i) {
        swap(_Data[i], _Data[root]);
        heapify(_Data, root, n, _Pred);
    }
}

template <class _Ty, class _Pr>
constexpr void create_heap(_Ty* const& _Data, size_t _Size, _Pr _Pred) {
    for (int i = _Size / 2 - 1; i >= 0; --i) {
        heapify(_Data, i, _Size, _Pred);
    }
}

template <class _Ty, class _Pr>
constexpr void sort_heap(_Ty* const& _Data, size_t _Size, _Pr _Pred) {
    for (int i = _Size - 1; i > 0; --i) {
        swap(_Data[0], _Data[i]);
        heapify(_Data, 0, i, _Pred);
    }
}

//sortowanie introspektywne
template <class _Ty, class _Pr>
inline constexpr void intro_sort(_Ty* const& _Data, int left, int right, int depth, _Pr _Pred) {
    if (left >= right) return;

    const size_t size = right - left + 1;
    if (size < 32) {
        insertion_sort(_Data + left, size, _Pred);
        return;
    }

    if (depth == 0) {
        create_heap(_Data + left, size, _Pred);
        sort_heap(_Data + left, size, _Pred);
        return;
    }

    int piv = partition(_Data, left, right, _Pred);
    intro_sort(_Data, left, piv, depth - 1, _Pred);
    intro_sort(_Data, piv + 1, right, depth - 1, _Pred);
}

template <class _Iter>
inline constexpr void sort(_Iter _First, _Iter _Last) {
    sort(_First, _Last, greater<>{});
}

template <class _Iter, class _Pr>
inline constexpr void sort(_Iter _First, _Iter _Last, _Pr _Pred) {

    static_assert(is_random_access<_Iter>, "Memory data must be contiguous! (random access)");

    auto _UFirst = unwrap(_First);
    auto _ULast = unwrap(_Last);
    
    const size_t size = _ULast - _UFirst;
    int max_depth = 2 * static_cast<int>(log2(size));
    intro_sort(_UFirst, 0, size - 1, max_depth, _Pred);
}

template <class _Ty, template<class _T = _Ty> class _Container>
inline constexpr _Container<_Ty> range(size_t _Count, int _Step)
{
    _Container<_Ty> _Data;
    size_t next = static_cast<size_t>(_Step > 0 ? 0 : (int)(1 - _Count) * _Step);
    for (size_t i = 0; i < _Count; ++i, next += _Step) {
        _Data.append(next);
    }
    return _Data;
}

template <class _Ty, template<class _T = _Ty> class _Container>
inline constexpr _Container<_Ty> range(size_t _Count)
{
    _Container<_Ty> _Data;
    for (size_t i = 0; i < _Count; ++i) {
        _Data.append(i);
    }
    return _Data;
}

#if __CPPVER < 201703L

template <class _Ty, template<class _T = _Ty, class _Al = allocator<_Ty>> class _Container>
inline constexpr _Container<_Ty, allocator<_Ty>> range(size_t _Count)
{
    _Container<_Ty> _Data;
    for (size_t i = 0; i < _Count; ++i) {
        _Data.append(i);
    }
    return _Data;
}

template <class _Ty, template<class _T = _Ty, class _Al = allocator<_Ty>> class _Container>
inline constexpr _Container<_Ty, allocator<_Ty>> range(size_t _Count, int _Step)
{
    _Container<_Ty> _Data;
    size_t next = static_cast<size_t>(_Step > 0 ? 0 : (int)(1 - _Count) * _Step);
    for (size_t i = 0; i < _Count; ++i, next += _Step) {
        _Data.append(next);
    }
    return _Data;
}

#endif

template <class _Array>
inline constexpr _Array range(int _Step)
{
    _Array _Data{};
    size_t next = _Step > 0 ? 0 : (int)(1 - _Data.size()) * _Step;
    for (size_t i = 0; i < _Data.size(); ++i, next += _Step) {
        _Data[i] = next;
    }
    return _Data;
}

template <class _Array>
inline constexpr _Array range()
{
    _Array _Data{};
    for (size_t i = 0; i < _Data.size(); ++i) {
        _Data[i] = i;
    }
    return _Data;
}

template <class _Iter>
inline constexpr void reverse(_Iter _First, _Iter _Last)
{
    while (_First < --_Last) {
        iter_swap(_First++, _Last);
    }
}

#if __CPPVER >= 201703L

struct sorted_fn
{
    template <class _Ty>
    constexpr _Ty& operator()(_Ty& _Val) const {
        sort(_Val.begin(), _Val.end());
        return _Val;
    }
};

constexpr const sorted_fn sorted = {};

template <class _Ty>
_Ty& operator|(_Ty& _Val, const sorted_fn& s) {
    return s(_Val);
}

template <class _Ty>
_Ty operator|(_Ty&& _Val, const sorted_fn& s) {
    return s(_Val);
}

#endif