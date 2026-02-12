// MySTL
//
// This header provides basic type traits utilities for compile-time
// type introspection. It defines integral_constant as the foundation
// for type-level boolean logic, along with true_type and false_type
// convenience aliases.

#pragma once

namespace mystl {

// integral_constant wraps a static constant of type T with value v.
// It is the base class for type traits that represent compile-time
// boolean or integral values.
//
// Example:
//   using two = mystl::integral_constant<int, 2>;
//   static_assert(two::value == 2);
template <class T, T v>
struct integral_constant {
  static constexpr T value = v;
};

// bool_constant is a convenience alias for integral_constant<bool, B>.
template <bool B>
using bool_constant = integral_constant<bool, B>;

// true_type and false_type are the two most commonly used
// integral_constants. They serve as base classes for binary type
// traits (e.g., is_same, is_pointer).
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

// remove_reference strips lvalue and rvalue references from T.
//
// Primary template: T is not a reference, so type is T itself.
// Partial specialization for T&: strips lvalue reference.
// Partial specialization for T&&: strips rvalue reference.
template <class T>
struct remove_reference { typedef T type; };

template <class T>
struct remove_reference<T&> { typedef T type; };

template <class T>
struct remove_reference<T&&> { typedef T type; };

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

// is_lvalue_reference determines whether T is an lvalue reference type.
// Used by forward() to guard against incorrect usage.
template <class T>
struct is_lvalue_reference : false_type {};

template <class T>
struct is_lvalue_reference<T&> : true_type {};

}  // namespace mystl
