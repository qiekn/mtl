#include "type_traits.h"

#include <gtest/gtest.h>

TEST(TypeTraitsTest, IntegralConstant) {
  // 测试 integral_constant 可以包装不同类型的编译期常量
  using five = mystl::integral_constant<int, 5>;
  EXPECT_EQ(five::value, 5);

  using char_a = mystl::integral_constant<char, 'a'>;
  EXPECT_EQ(char_a::value, 'a');
}

TEST(TypeTraitsTest, BoolConstant) {
  // 测试 bool_constant 是 integral_constant<bool, B> 的别名
  using my_true = mystl::bool_constant<true>;
  using my_false = mystl::bool_constant<false>;

  EXPECT_TRUE(my_true::value);
  EXPECT_FALSE(my_false::value);
}

TEST(TypeTraitsTest, TrueTypeAndFalseType) {
  // 测试 true_type 和 false_type
  EXPECT_TRUE(mystl::true_type::value);
  EXPECT_FALSE(mystl::false_type::value);
}
