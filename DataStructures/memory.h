#pragma once

#include "utility.h"

template <class _Ty, size_t _Size = sizeof(_Ty)>
struct allocator
{
	// allocates memory, no constructors are called
	static constexpr _Ty* allocate(size_t _Count) {
		return static_cast<_Ty*>(::operator new(_Size * _Count));
	}

	// realloc for new operator, creates new block, moves data and deletes previous
	static constexpr void reallocate(_Ty*& _Pointer, size_t _Old_size, size_t _New_size) {
		reallocate(_Pointer, _Old_size, _New_size, 0);
	}
	static constexpr void reallocate(_Ty*& _Pointer, size_t _Old_size, size_t _New_size, size_t _Offset) {
		auto _New_block = static_cast<_Ty*>(::operator new(_Size * _New_size));
		_Move_memory_block(_Pointer, _New_block + _Offset, min(_Old_size, _New_size - _Offset));
		::operator delete(_Pointer);
		_Pointer = _New_block;
	}
	// deallocates without calling destructors
	static constexpr void deallocate(_Ty*& _Pointer) {
		::operator delete(_Pointer);
	}

	// calls destructor
	static constexpr void destroy(_Ty& _Obj) {
		_Obj.~_Ty();
	}

	// calls destructor for every object in range
	template <class FwdIt>
	static constexpr void destroy_range(FwdIt _First, const FwdIt _Last) {
		for (; _First != _Last; ++_First) {
			(*_First).~_Ty();
		}
	}

	// creates object in place and forwards _Args to constructor
	template <class ..._Vals>
	static constexpr void construct(_Ty* _Where, _Vals&&... _Args) {
		new (_Where) _Ty(forward<_Vals>(_Args)...);
	}

	// creates objects (in place) in range and forwards _Args to constructor
	template <class ..._Vals>
	static constexpr void construct_range(_Ty* _First, _Ty* _Last, _Vals&&... _Args) {
		for (; _First != _Last; ++_First) {
			new (_First) _Ty(forward<_Vals>(_Args)...);
		}
	}
private:
	static constexpr void _Move_memory_block(_Ty* _Src, _Ty* _Dst, size_t _Count) {
		for (size_t i = 0; i < _Count; ++i) {
			new (_Dst) _Ty(move(*_Src));
			++_Src; ++_Dst;
		}
	}
};

template <class _Ty>
inline constexpr void _Move_in_place(_Ty* _Src, _Ty* _Dst, size_t _Count) {
	if (_Src == _Dst) return;
	if (_Src > _Dst) {
		for (int i = 0; i < _Count; ++i) {
			new(_Dst) _Ty(move(*_Src));
			++_Dst; ++_Src;
		}
	}
	else {
		_Ty* _RSrc = _Src + _Count - 1;
		_Ty* _RDst = _Dst + _Count - 1;
		for (int i = 0; i < _Count; ++i) {
			new(_RDst) _Ty(move(*_RSrc));
			--_RDst; --_RSrc;
		}
	}
}

template <class _Ty>
inline constexpr void _Copy_in_place(_Ty* _First, _Ty* _Last, _Ty* _Dest) {
	for (; _First != _Last; ++_First, ++_Dest) {
		new(_Dest) _Ty(*_First);
	}
}

template <class _Ty, class ...Args>
inline constexpr void _Fill_in_place(_Ty* _First, _Ty* _Last, Args&&... _Args) {
	for (; _First != _Last; ++_First) {
		new(_First) _Ty(forward<Args>(_Args)...);
	}
}


