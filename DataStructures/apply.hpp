#pragma once

#ifndef __APPLY_HPP__
#define __APPLY_HPP__

#include "utility.h"
namespace pipeline
{
    struct apply_fn
    {
    public:
        template <class _Container>
        constexpr auto operator()(_Container& _C) const {
            return _Pipe_obj_arg<apply_fn, _Container&>{ *this, _C };
        }
        template <class _Container>
        constexpr auto operator()(_Container&& _C) const {
            return _Pipe_obj_arg<apply_fn, _Container>{ *this, move(_C) };
        }
        template <class _Range, class _Container>
        constexpr _Container& operator()(_Range& _Rng, _Container& _C) const {
            _Apply_range(_Rng, _C);
            return _C;
        }
        template <class _Range, class _Container>
        constexpr _Container operator()(_Range& _Rng, _Container&& _C) const {
            _Apply_range(_Rng, _C);
            return _C;
        }
        template <class _Range, class _Container>
        constexpr _Container& operator()(_Range&& _Rng, _Container& _C) const {
            _Apply_range(move(_Rng), _C);
            return _C;
        }
        template <class _Range, class _Container>
        constexpr _Container operator()(_Range&& _Rng, _Container&& _C) const {
            _Apply_range(move(_Rng), _C);
            return _C;
        }
    private:
        template <class _Range, class _Container>
        constexpr void _Apply_range(_Range&& _Rng, _Container& _C) const {
            auto _It = _C.begin();
            for (auto&& e : _Rng) {
                if (_It == _C.end()) return;
                *_It = move(e);
                ++_It;
            }
        }
    };
    constexpr apply_fn apply = {};

    struct apply_push_fn
    {
    public:
        template <class _Container>
        constexpr auto operator()(_Container& _C) const {
            return _Pipe_obj_arg<apply_push_fn, _Container&>{ *this, _C };
        }
        template <class _Container>
        constexpr auto operator()(_Container&& _C) const {
            return _Pipe_obj_arg<apply_push_fn, _Container>{ *this, move(_C) };
        }
        template <class _Range, class _Container>
        constexpr _Container operator()(_Range& _Rng, _Container& _C) const {
            _Apply_range(_Rng, _C);
            return _C;
        }
        template <class _Range, class _Container>
        constexpr _Container operator()(_Range& _Rng, _Container&& _C) const {
            _Apply_range(_Rng, _C);
            return _C;
        }
        template <class _Range, class _Container>
        constexpr _Container operator()(_Range&& _Rng, _Container& _C) const {
            _Apply_range(move(_Rng), _C);
            return _C;
        }
        template <class _Range, class _Container>
        constexpr _Container operator()(_Range&& _Rng, _Container&& _C) const {
            _Apply_range(move(_Rng), _C);
            return _C;
        }
    private:
        template <class _Range, class _Container>
        constexpr void _Apply_range(_Range&& _Rng, _Container& _C) const {
            for (auto&& e : _Rng) {
                _C.push_back(move(e));
            }
        }
    };
    constexpr apply_push_fn apply_push = {};

}

#endif // !__APPLY_HPP__