#pragma once

#include "memory.h"

#if __CPPVER >= 201703L

#include "pointer_iterator.h"
#include "cycle.hpp"
#include "enumerate.hpp"
#include "remove_if.hpp"
#include "take.hpp"
#include "transform.hpp"
#include "zip.hpp"
#include "unique.hpp"
#include "to_file.hpp"

#include "apply.hpp"

namespace pipeline
{
    template <class _Container, class _Fun>
    constexpr decltype(auto) operator|(_Container& _C, _Fun _Fn) {
        return _Fn(_C);
    }
    template <class _Container, class _Fun>
    constexpr decltype(auto) operator|(_Container&& _C, _Fun _Fn) {
        return _Fn(move(_C));
    }
    template <class _Container, class _Fun, class _Arg>
    constexpr decltype(auto) operator|(_Container& _C,  _Pipe_obj_arg<_Fun, _Arg> _Fn) {
        return _Fn._Fn(_C, _Fn._Arg);
    }
    template <class _Container, class _Fun, class _Arg>
    constexpr decltype(auto) operator|(_Container&& _C, _Pipe_obj_arg<_Fun, _Arg> _Fn) {
        return _Fn._Fn(move(_C), _Fn._Arg);
    }

    namespace utils {
        template <class _Ty, size_t _Size>
        struct _Array_wrapper 
        {
            using iterator = pointer_iterator<_Ty>;
            using const_iterator = pointer_iterator<const _Ty>;
            using value_type = _Ty;

            constexpr auto begin() {
                return iterator(_Ptr);
            }
            constexpr auto begin() const {
                return const_iterator(_Ptr);
            }
            constexpr auto end() {
                return iterator(_Ptr + _Size);
            }
            constexpr auto end() const {
                return const_iterator(_Ptr + _Size);
            }
            _Ty* _Ptr;
        };
    }

    template <size_t _Size, class _Ty, class _Fun>
    constexpr decltype(auto) operator|(_Ty (&_C)[_Size], _Fun _Fn) {
        return _Fn(utils::_Array_wrapper<_Ty, _Size>{ _C });
    }
    template <class _Ty, class _Fun, class _Arg, size_t _Size>
    constexpr decltype(auto) operator|(_Ty(&_C)[_Size], _Pipe_obj_arg<_Fun, _Arg> _Fn) {
        return _Fn._Fn(utils::_Array_wrapper<_Ty, _Size>{ _C }, _Fn._Arg);
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
template <class _It, class _Pr>
constexpr void insertion_sort(_It _First, size_t _Size, _Pr _Pred) {
    for (int i = 1, j; i < _Size; ++i) {
        auto _Tmp = _First[i];
        for (j = i - 1; j >= 0 && _Pred(_First[j], _Tmp); --j) {
            _First[j + 1] = _First[j];
        }
        _First[j + 1] = _Tmp;
    }
}

template<class _It, class _Ty>
constexpr int median(_It _First, const int& a, const int& b, const int& c) {
    auto x = _First[a] - _First[b], y = _First[b] - _First[c], z = _First[a] - _First[c];
    return x * y > 0 ? b : x * z > 0 ? c : a;
}

template<class _It, class _Pr>
constexpr int partition(_It _First, int left, int right, _Pr _Pred) {
    auto pivot = _First[(right + left) / 2];
    int i = left, j = right;
    while (true) {
        while (_Pred(_First[j], pivot)) j--;
        while (!_Pred(_First[i], pivot)) i++;

        if (i < j) {
            swap(_First[i], _First[j]);
            i++; j--;
        }
        else {
            return j;
        }
    }
}

template <class _It, class _Pr>
constexpr void heapify(_It _First, int i, size_t n, _Pr _Pred) {
    int root = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && _Pred(_First[left], _First[root])) root = left;
    if (right < n&& _Pred(_First[right], _First[root])) root = right;

    if (root != i) {
        swap(_First[i], _First[root]);
        heapify(_First, root, n, _Pred);
    }
}

template <class _It, class _Pr>
constexpr void create_heap(_It _First, size_t _Size, _Pr _Pred) {
    for (int i = _Size / 2 - 1; i >= 0; --i) {
        heapify(_First, i, _Size, _Pred);
    }
}

template <class _It, class _Pr>
constexpr void sort_heap(_It _First, size_t _Size, _Pr _Pred) {
    for (int i = _Size - 1; i > 0; --i) {
        swap(_First[0], _First[i]);
        heapify(_First, 0, i, _Pred);
    }
}

//sortowanie introspektywne
template <class _It, class _Pr>
inline constexpr void intro_sort(_It _First, int left, int right, int depth, _Pr _Pred) {
    if (left >= right) return;

    const size_t size = right - left + 1;
    if (size < 32) {
        insertion_sort(_First + left, size, _Pred);
        return;
    }

    if (depth == 0) {
        create_heap(_First + left, size, _Pred);
        sort_heap(_First + left, size, _Pred);
        return;
    }

    int piv = partition(_First, left, right, _Pred);
    intro_sort(_First, left, piv, depth - 1, _Pred);
    intro_sort(_First, piv + 1, right, depth - 1, _Pred);
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
        _Data.push_back(next);
    }
    return _Data;
}

template <class _Ty, template<class _T = _Ty> class _Container>
inline constexpr _Container<_Ty> range(size_t _Count)
{
    _Container<_Ty> _Data;
    for (size_t i = 0; i < _Count; ++i) {
        _Data.push_back(i);
    }
    return _Data;
}

#if __CPPVER < 201703L

template <class _Ty, template<class _T = _Ty, class _Al = allocator<_Ty>> class _Container>
inline constexpr _Container<_Ty, allocator<_Ty>> range(size_t _Count)
{
    _Container<_Ty> _Data;
    for (size_t i = 0; i < _Count; ++i) {
        _Data.push_back(i);
    }
    return _Data;
}

template <class _Ty, template<class _T = _Ty, class _Al = allocator<_Ty>> class _Container>
inline constexpr _Container<_Ty, allocator<_Ty>> range(size_t _Count, int _Step)
{
    _Container<_Ty> _Data;
    size_t next = static_cast<size_t>(_Step > 0 ? 0 : (int)(1 - _Count) * _Step);
    for (size_t i = 0; i < _Count; ++i, next += _Step) {
        _Data.push_back(next);
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


namespace pipeline
{
    struct sorted_fn
    {
        template <class _Ty>
        constexpr _Ty& operator()(_Ty& _Val) const {
            sort(_Val.begin(), _Val.end());
            return _Val;
        }
    };

    constexpr sorted_fn sorted = {};

    template <class _Ty>
    _Ty& operator|(_Ty& _Val, const sorted_fn& s) {
        return s(_Val);
    }

    template <class _Ty>
    _Ty operator|(_Ty&& _Val, const sorted_fn& s) {
        return s(_Val);
    }
}
#endif

