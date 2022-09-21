#pragma once

#include "utility.h"

namespace pipeline
{
	struct sum_fn
	{
		template <range _Range>
		constexpr auto operator()(_Range&& _Rng) const {
			return ::utils::sum(_Rng.begin(), _Rng.end());
		}
	};

	constexpr sum_fn sum = {};

	
	struct sum_if_fn
	{
		template <class _Fun>
		constexpr auto operator()(_Fun _Fn) const {
			return _Pipe_obj_arg{ *this, _Fn };
		}
		
		template <range _Range, class _Fun>
		constexpr auto operator()(_Range&& _Rng, _Fun _Fn) const {
			return ::utils::sum_if(_Rng.begin(), _Rng.end(), _Fn);
		}
	};
	
	constexpr sum_if_fn sum_if = {};

	
	struct average_fn
	{
		template <range _Range>
		constexpr auto operator()(_Range&& _Rng) const {
			return ::utils::average(_Rng.begin(), _Rng.end());
		}
	};
	
	constexpr average_fn average = {};

	
	struct average_if_fn
	{
		template <class _Fun>
		constexpr auto operator()(_Fun _Fn) const {
			return _Pipe_obj_arg{ *this, _Fn };
		}

		template <range _Range, class _Fun>
		constexpr auto operator()(_Range&& _Rng, _Fun _Fn) const {
			return ::utils::average_if(_Rng.begin(), _Rng.end(), _Fn);
		}
	};
	
	constexpr average_if_fn average_if = {};

	
	struct distance_fn
	{
		template <range _Range>
		constexpr auto operator()(_Range&& _Rng) const {
			return ::distance(_Rng.begin(), _Rng.end());
		}
	};

	constexpr distance_fn distance = {};


	struct max_fn
	{
		template <range _Range>
		constexpr auto operator()(_Range&& _Rng) const {
			return ::utils::max(_Rng.begin(), _Rng.end());
		}
	};

	constexpr max_fn max = {};


	struct min_fn
	{
		template <range _Range>
		constexpr auto operator()(_Range&& _Rng) const {
			return ::utils::min(_Rng.begin(), _Rng.end());
		}
	};

	constexpr min_fn min = {};
}