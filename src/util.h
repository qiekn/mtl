// MySTL
//
// This header provides core utility functions: move, forward, swap, and pair.
// These are fundamental building blocks for move semantics, perfect
// forwarding, and paired value storage throughout the library.

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

// pair holds two objects of possibly different types. It is used
// extensively throughout STL: map elements are pair<const Key, Value>,
// and many algorithms return pairs.
//
// T1, T2 are the pair's own type parameters, fixed at creation.
// U1, U2 in the forwarding constructor are deduced from arguments,
// allowing construction from compatible types (e.g., const char* -> string).
template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;

  // Default constructor: value-initializes both members.
  pair() : first(), second() {}

  // Constructs a pair from two values. Kept alongside the forwarding
  // constructor to provide a non-template overload that avoids ambiguity
  // in overload resolution (template constructors can be too greedy).
  pair(const T1& a, const T2& b) : first(a), second(b) {}

  // Forwarding constructor: deduces U1, U2 from arguments and uses
  // forward to preserve value category (lvalue -> copy, rvalue -> move).
  template <class U1, class U2>
  pair(U1&& a, U2&& b)
      : first(mystl::forward<U1>(a)), second(mystl::forward<U2>(b)) {}

  // Copy and move constructors: compiler-generated defaults.
  pair(const pair& other) = default;
  pair(pair&& other) = default;

  // Copy assignment.
  pair& operator=(const pair& other) {
    first = other.first;
    second = other.second;
    return *this;
  }

  // Move assignment.
  pair& operator=(pair&& other) noexcept {
    first = mystl::move(other.first);
    second = mystl::move(other.second);
    return *this;
  }

  void swap(pair& other) {
    mystl::swap(first, other.first);
    mystl::swap(second, other.second);
  }
};

// Comparison operators for pair. Pairs are compared lexicographically:
// first by first, then by second.

template <class T1, class T2>
bool operator==(const pair<T1, T2>& a, const pair<T1, T2>& b) {
  return a.first == b.first && a.second == b.second;
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2>& a, const pair<T1, T2>& b) {
  return !(a == b);
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& a, const pair<T1, T2>& b) {
  return a.first < b.first || (a.first == b.first && a.second < b.second);
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& a, const pair<T1, T2>& b) {
  return b < a;
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2>& a, const pair<T1, T2>& b) {
  return !(b < a);
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2>& a, const pair<T1, T2>& b) {
  return !(a < b);
}

// make_pair creates a pair, deducing T1 and T2 from the arguments.
template <class T1, class T2>
pair<T1, T2> make_pair(T1&& a, T2&& b) {
  return pair<T1, T2>(mystl::forward<T1>(a), mystl::forward<T2>(b));
}

}  // namespace mystl
