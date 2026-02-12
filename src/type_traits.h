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

}  // namespace mystl
