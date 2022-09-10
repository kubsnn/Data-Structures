#pragma once

#include "utility.h"
#include <fstream>
#include <iostream>

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

	struct from_file_fn
	{
		constexpr auto operator()(const char* _File_path) const {
			return _Pipe_obj_arg{ *this, _File_path };
		}

		template <class _Range>
		constexpr decltype(auto) operator()(_Range&& _Rng, const char* _Path) const {
			using _Ty = remove_reference_t<decltype(*_Rng.begin())>;
			
			std::ifstream _File(_Path);
			while (!_File.eof()) {
				_Ty _Tmp;
				_File >> _Tmp;
				if constexpr (pushbackable<_Range, _Ty>) {
					_Rng.push_back(move(_Tmp));
				} else if constexpr (pushfrontable<_Range, _Ty>) {
					_Rng.push_front(move(_Tmp));
				} else {
					static_assert(_Always_false<_Range>, "container not able to push new elements");
				}
			}
			_File.close();

			return forward<_Range>(_Rng);
		}
	};

	constexpr from_file_fn from_file = {};
}