// MySTL
//
// This header provides core utility functions: move, forward, and swap.
// These are fundamental building blocks for move semantics and perfect
// forwarding throughout the library.

#pragma once

#include <cstddef>

#include "type_traits.h"

namespace mystl {

// move casts its argument to an rvalue reference, enabling move semantics.
// It does not move anything by itself; it merely signals that the caller
// no longer needs the value and its resources may be transferred.
template <class T>
remove_reference_t<T>&& move(T&& arg) noexcept {
  return static_cast<remove_reference_t<T>&&>(arg);
}

// forward preserves the value category (lvalue or rvalue) of a forwarded
// argument. It is used inside templates that take universal references
// (T&&) to pass arguments to another function without losing their
// original lvalue/rvalue nature.
//
// Overload 1: accepts an lvalue and forwards it according to T.
//   - If T = int&, reference collapsing makes T&& = int& (lvalue).
//   - If T = int,  T&& = int&& (rvalue).
template <class T>
T&& forward(remove_reference_t<T>& arg) noexcept {
  return static_cast<T&&>(arg);
}

// Overload 2: accepts an rvalue. The static_assert prevents forwarding
// an rvalue as an lvalue, which would be a dangling reference bug.
template <class T>
T&& forward(remove_reference_t<T>&& arg) noexcept {
  static_assert(!is_lvalue_reference<T>::value,
                "cannot forward an rvalue as an lvalue");
  return static_cast<T&&>(arg);
}

// swap exchanges the values of a and b using move semantics.
// Three moves instead of three copies: O(1) for types with move support.
template <class T>
void swap(T& a, T& b) noexcept {
  T tmp = mystl::move(a);
  a = mystl::move(b);
  b = mystl::move(tmp);
}

// swap overload for fixed-size arrays.
template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept {
  for (size_t i = 0; i < N; ++i)
    swap(a[i], b[i]);
}

}  // namespace mystl
