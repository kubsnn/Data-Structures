#pragma once

#include "utility.h"
#include <fstream>

namespace pipeline
{
	
	struct to_file_fn
	{
		constexpr auto operator()(const char* _File_path) const {
			return _Pipe_obj_arg<to_file_fn, const char*>{ *this, _File_path };
		}

		template <class _Range>
		constexpr decltype(auto) operator()(_Range&& _Rng, const char* _Path) const {
			std::ofstream _File(_Path);
			for (auto&& e : _Rng) {
				_File << e << std::endl;
			}
			_File.close();

			return forward<_Range>(_Rng);
		}
	};

	constexpr to_file_fn to_file = {};
}