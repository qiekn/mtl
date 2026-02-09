#pragma once

// 这个头文件用于提取类型信息

namespace mystl {

// =============================================================================
// integral_constant
// 将编译期常量包装为类型，使模板可以根据值进行重载
// =============================================================================

template <class T, T v>
struct integral_constant {
  static constexpr T value = v;
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

}  // namespace mystl
