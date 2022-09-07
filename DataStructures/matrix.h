#pragma once

#include "utility.h"
#include <iostream>

/*

	auto _End = reinterpret_cast<const _Ty2*>(_Data) + _Rows * _Cols;
	auto _Begin = reinterpret_cast<const _Ty*>(_Data);

	while (_Begin != _End) {
		for (size_t i = 0; i < _Cols; ++i, ++_Begin) {
			*_Begin += _Other._Data[0][i];
		}
	}
	
	return *this;

*/
namespace pipeline
{
	template <class _Ty, size_t _Rows, size_t _Cols>
	struct _Transpose_iterator;

	template <class _Ty, size_t _Rows, size_t _Cols>
	struct _Transpose_view;

	struct transpose_fn;
}
template <class _Ty, size_t _Rows, size_t _Cols>
class matrix
{
public:
	template <class _Ty, size_t _Rows, size_t _Cols>
	friend class matrix;
	template <class _Ty, size_t _Rows, size_t _Cols>
	friend struct pipeline::_Transpose_view;
	friend struct pipeline::transpose_fn;

	constexpr matrix() = default;
	constexpr matrix(const _Ty& _Init_val);
	constexpr matrix(const matrix& _Other) = default;
	constexpr matrix(matrix&& _Other) = default;

	constexpr matrix& operator=(const matrix& _Other) = default;
	constexpr matrix& operator=(matrix&& _Other) = default;
	
	template <class _Ty2>
	constexpr matrix& operator+=(const pipeline::_Transpose_view<_Ty2, _Cols, _Rows>& _Other);
	template <class _Ty2>
	constexpr matrix& operator+=(const matrix<_Ty2, _Rows, _Cols>& _Other);
	template <class _NumTy>
	constexpr matrix& operator+=(const _NumTy& _Val);
	template <class _Ty2, size_t _C = _Cols>
	constexpr matrix<_Ty, _Rows, _Cols>& operator+=(const matrix<_Ty2, 1, _C>& _Other);
	
	template <class _Ty2>
	constexpr matrix& operator-=(const pipeline::_Transpose_view<_Ty2, _Cols, _Rows>& _Other);
	template <class _Ty2>
	constexpr matrix& operator-=(const matrix<_Ty2, _Rows, _Cols>& _Other);

	constexpr auto begin();
	constexpr auto begin() const;
	constexpr auto end();
	constexpr auto end() const;

	constexpr size_t rows() const;
	constexpr size_t columns() const;
	constexpr void print() const;
	
	constexpr decltype(auto) operator[](unsigned int _Row);
	constexpr decltype(auto) operator[](unsigned int _Row) const;

	template <class _Ty2, size_t _R>
	constexpr auto operator*(const matrix<_Ty2, _Cols, _R>& _Other) const;
	
	template <class _Ty, size_t _Rows, size_t _Cols>
	inline friend std::ostream& operator<<(std::ostream& _Ostream, const matrix& _Matrix);

private:
	template <class _Ty2, size_t _R>
	constexpr auto dot(const matrix<_Ty2, _Cols, _R>& _Other) const;

	_Ty _Data[_Rows][_Cols]{};
};

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr matrix<_Ty, _Rows, _Cols>::matrix(const _Ty& _Init_val)
{
	for (auto&& row : _Data) {
		fill(row, row + _Cols, _Init_val);
	}
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2>
inline constexpr matrix<_Ty, _Rows, _Cols>& matrix<_Ty, _Rows, _Cols>::operator+=(const pipeline::_Transpose_view<_Ty2, _Cols, _Rows>& _Other)
{
	auto _Begin = _Other.begin();
	auto _End = _Other.end();
	auto _It = reinterpret_cast<_Ty*>(_Data);

	for (; _Begin != _End; ++_Begin, ++_It) {
		*_It += static_cast<_Ty>(*_Begin);
	}
	return *this;
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2>
inline constexpr matrix<_Ty, _Rows, _Cols>& matrix<_Ty, _Rows, _Cols>::operator+=(const matrix<_Ty2, _Rows, _Cols>& _Other)
{
	auto _Begin = reinterpret_cast<const _Ty2*>(_Other._Data);
	auto _End = reinterpret_cast<const _Ty2*>(_Other._Data) + _Rows * _Cols;
	auto _It = reinterpret_cast<_Ty*>(_Data);

	for (; _Begin != _End; ++_Begin, ++_It) {
		*_It += static_cast<_Ty>(*_Begin);
	}
	return *this;
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2, size_t _C>
inline constexpr matrix<_Ty, _Rows, _Cols>& matrix<_Ty, _Rows, _Cols>::operator+=(const matrix<_Ty2, 1, _C>& _Other)
{
	auto _End = reinterpret_cast<_Ty*>(_Data) + _Rows * _Cols;
	auto _Begin = reinterpret_cast<_Ty*>(_Data);

	while (_Begin != _End) {
		for (size_t i = 0; i < _Cols; ++i, ++_Begin) {
			*_Begin += static_cast<_Ty>(_Other._Data[0][i]);
		}
	}

	return *this;
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _NumTy>
inline constexpr matrix<_Ty, _Rows, _Cols>& matrix<_Ty, _Rows, _Cols>::operator+=(const _NumTy& _Val)
{
	static_assert(is_numeric<_NumTy>, "Value type must be int/float etc!");

	auto _Begin = _Data;
	auto _End = _Data + _Rows * _Cols;
	for (; _Begin != _End; ++_Begin) {
		*_Begin += static_cast<_Ty>(_Val);
	}

	
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2>
inline constexpr matrix<_Ty, _Rows, _Cols>& matrix<_Ty, _Rows, _Cols>::operator-=(const pipeline::_Transpose_view<_Ty2, _Cols, _Rows>& _Other)
{
	auto _Begin = _Other.begin();
	auto _End = _Other.end();
	auto _It = reinterpret_cast<_Ty*>(_Data);

	for (; _Begin != _End; ++_Begin, ++_It) {
		*_It -= static_cast<_Ty>(*_Begin);
	}
	return *this;
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2>
inline constexpr matrix<_Ty, _Rows, _Cols>& matrix<_Ty, _Rows, _Cols>::operator-=(const matrix<_Ty2, _Rows, _Cols>& _Other)
{
	auto _Begin = reinterpret_cast<const _Ty2*>(_Other._Data);
	auto _End = reinterpret_cast<const _Ty2*>(_Other._Data) + _Rows * _Cols;
	auto _It = reinterpret_cast<_Ty*>(_Data);

	for (; _Begin != _End; ++_Begin, ++_It) {
		*_It -= static_cast<_Ty>(*_Begin);
	}
	return *this;
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr auto matrix<_Ty, _Rows, _Cols>::begin()
{
	return pointer_iterator(reinterpret_cast<_Ty*>(_Data));
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr auto matrix<_Ty, _Rows, _Cols>::begin() const
{
	return pointer_iterator(reinterpret_cast<_Ty*>(_Data));
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr auto matrix<_Ty, _Rows, _Cols>::end()
{
	return pointer_iterator(reinterpret_cast<_Ty*>(_Data) + _Rows * _Cols);
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr auto matrix<_Ty, _Rows, _Cols>::end() const
{
	return pointer_iterator(reinterpret_cast<_Ty*>(_Data) + _Rows * _Cols);
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr size_t matrix<_Ty, _Rows, _Cols>::rows() const
{
	return _Rows;
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr size_t matrix<_Ty, _Rows, _Cols>::columns() const
{
	return _Cols;
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr void matrix<_Ty, _Rows, _Cols>::print() const
{
	std::cout << "{\n";
	for (auto&& row : _Data) {
		std::cout << "    ";
		for (auto&& e : row) {
			std::cout << e << ' ';
		}
		std::cout << '\n';
	}
	std::cout << "}" << std::endl;
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr decltype(auto) matrix<_Ty, _Rows, _Cols>::operator[](unsigned int _Row)
{
	return _Data[_Row];
}

template<class _Ty, size_t _Rows, size_t _Cols>
inline constexpr decltype(auto) matrix<_Ty, _Rows, _Cols>::operator[](unsigned int _Row) const
{
	return _Data[_Row];
}

template <class _Ty, size_t _Rows, size_t _Cols>
inline std::ostream& operator<<(std::ostream& _Ostream, const matrix<_Ty, _Rows, _Cols>& _Matrix)
{
	_Matrix.print();
	return _Ostream;
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2, size_t _R>
inline constexpr auto matrix<_Ty, _Rows, _Cols>::operator*(const matrix<_Ty2, _Cols, _R>& _Other) const
{
	return dot(_Other);
}

template<class _Ty, size_t _Rows, size_t _Cols>
template<class _Ty2, size_t _C>
inline constexpr auto matrix<_Ty, _Rows, _Cols>::dot(const matrix<_Ty2, _Cols, _C>& _Other) const
{
	matrix<_Ty, _Rows, _C> _Res(0);

	for (size_t i = 0; i < _Rows; ++i) {
		for (size_t j = 0; j < _C; ++j) {
			for (size_t k = 0; k < _Cols; ++k) {
				_Res._Data[i][j] += _Data[i][k] * _Other._Data[k][j];
			}
		}
	}

	return _Res;
}


namespace pipeline 
{
	template <class _Ty, size_t _Rows, size_t _Cols>
	struct _Transpose_view
	{
		using iterator = _Transpose_iterator<_Ty, _Rows, _Cols>;
		using const_iterator = _Transpose_iterator<const _Ty, _Rows, _Cols>;
		using value_type = _Ty;

		constexpr _Transpose_view(matrix<_Ty, _Rows, _Cols>& _Matrix)
			: _Data(reinterpret_cast<_Ty**>(_Matrix._Data))
		{ }
		constexpr auto begin() {
			return iterator(_Data);
		}
		constexpr auto begin() const {
			return iterator(_Data);
		}
		constexpr auto end() const {
			return pipeline::sentinel{};
		}

		_Ty** _Data;
	};

	template <class _Ty, size_t _Rows, size_t _Cols>
	struct _Transpose_iterator
	{
		using category = forward_iterator;

		constexpr _Transpose_iterator(_Ty** _Iter)
			: _Curr(reinterpret_cast<_Ty*>(_Iter))
			, _End(reinterpret_cast<_Ty*>(_Iter) + _Rows * _Cols)
		{ }
		constexpr _Ty& operator*() {
			return *_Curr;
		}
		constexpr const _Ty& operator*() const {
			return *_Curr;
		}
		constexpr _Transpose_iterator& operator++() {
			_Curr += _Cols;
			if (_Curr >= _End) {
				_Curr -= _Cols * _Rows - 1;
				++_Col_id;
			}
			return *this;
		}
		constexpr _Transpose_iterator operator++(int) {
			auto _Tmp = *this;
			++*this;
			return _Tmp;
		}
		constexpr friend bool operator==(const _Transpose_iterator& _It, pipeline::sentinel) {
			return _It._Col_id == _Cols;
		}
		constexpr friend bool operator!=(const _Transpose_iterator& _It, pipeline::sentinel) {
			return _It._Col_id != _Cols;
		}
	private:
		_Ty* _Curr;
		size_t _Col_id = 0;
		_Ty* const _End;
	};

	struct transpose_fn
	{
		template <class _Ty, size_t _Rows, size_t _Cols>
		constexpr auto operator()(matrix<_Ty, _Rows, _Cols>& _M) const {
			return _Transpose_view<_Ty, _Rows, _Cols>(_M);
		}
	};
	
	constexpr transpose_fn transpose = {};
}

namespace utils
{
	template <class _Ty, size_t _Rows, size_t _Cols>
	constexpr auto transpose(const matrix<_Ty, _Rows, _Cols>& _M)
	{
		matrix<_Ty, _Cols, _Rows> _Res(0);

		for (size_t i = 0; i < _Rows; ++i) {
			for (size_t j = 0; j < _Cols; ++j) {
				_Res[j][i] = _M[i][j];
			}
		}

		return _Res;
	}
}