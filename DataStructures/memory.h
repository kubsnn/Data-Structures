#pragma once

#include "Utility.h"

template <class _Ty, size_t _Size = sizeof(_Ty)>
struct allocator
{
	// allocates memory, no constructors are called
	static constexpr _Ty* allocate(size_t _Count) {
		return static_cast<_Ty*>(::operator new(_Size * _Count));
	}

	// realloc for new operator, creates new block, moves data and deletes previous
	static constexpr void reallocate(_Ty*& _Pointer, size_t _Old_size, size_t _New_size) {
		auto _New_block = static_cast<_Ty*>(::operator new(_Size * _New_size));
		_Move_memory_block(_Pointer, _New_block, min(_Old_size, _New_size));
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
		new(_Where) _Ty(forward<_Vals>(_Args)...);
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
inline void _Copy_in_place(_Ty* _First, _Ty* _Last, _Ty* _Dest) {
	for (; _First != _Last; ++_First, ++_Dest) {
		new(_Dest) _Ty(*_First);
	}
}

template <class _Ty>
inline void _Fill_in_place(_Ty* _First, _Ty* _Last, const _Ty& _Val) {
	for (; _First != _Last; ++_First) {
		new(_First) _Ty(_Val);
	}
}


