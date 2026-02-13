#include "util.h"

#include <gtest/gtest.h>

#include <string>
#include <type_traits>

// -- move tests --

TEST(MoveTest, ConvertsLvalueToRvalue) {
  int x = 42;
  static_assert(std::is_rvalue_reference<decltype(mystl::move(x))>::value);
  EXPECT_EQ(mystl::move(x), 42);
}

TEST(MoveTest, StringResourceTransfer) {
  std::string a = "hello";
  std::string b = mystl::move(a);
  EXPECT_EQ(b, "hello");
  // After move, a is in a valid but unspecified state.
  // Typically empty for std::string.
  EXPECT_TRUE(a.empty());
}

// -- forward tests --

// Helper to detect value category.
enum class ValueCat { Lvalue, Rvalue };

template <class T>
ValueCat detect(T&&) {
  return std::is_lvalue_reference<T>::value ? ValueCat::Lvalue
                                            : ValueCat::Rvalue;
}

template <class T>
ValueCat forward_and_detect(T&& arg) {
  return detect(mystl::forward<T>(arg));
}

TEST(ForwardTest, PreservesLvalue) {
  int x = 1;
  EXPECT_EQ(forward_and_detect(x), ValueCat::Lvalue);
}

TEST(ForwardTest, PreservesRvalue) {
  EXPECT_EQ(forward_and_detect(42), ValueCat::Rvalue);
}

// -- swap tests --

TEST(SwapTest, SwapInts) {
  int a = 1, b = 2;
  mystl::swap(a, b);
  EXPECT_EQ(a, 2);
  EXPECT_EQ(b, 1);
}

TEST(SwapTest, SwapStrings) {
  std::string a = "hello", b = "world";
  mystl::swap(a, b);
  EXPECT_EQ(a, "world");
  EXPECT_EQ(b, "hello");
}

TEST(SwapTest, SwapArrays) {
  int a[] = {1, 2, 3};
  int b[] = {4, 5, 6};
  mystl::swap(a, b);
  EXPECT_EQ(a[0], 4);
  EXPECT_EQ(a[2], 6);
  EXPECT_EQ(b[0], 1);
  EXPECT_EQ(b[2], 3);
}

// -- pair tests --

TEST(PairTest, DefaultConstruct) {
  mystl::pair<int, int> p;
  EXPECT_EQ(p.first, 0);
  EXPECT_EQ(p.second, 0);
}

TEST(PairTest, ValueConstruct) {
  mystl::pair<int, std::string> p(1, "hello");
  EXPECT_EQ(p.first, 1);
  EXPECT_EQ(p.second, "hello");
}

TEST(PairTest, CopyConstruct) {
  mystl::pair<int, int> a(3, 4);
  mystl::pair<int, int> b(a);
  EXPECT_EQ(b.first, 3);
  EXPECT_EQ(b.second, 4);
}

TEST(PairTest, MoveConstruct) {
  mystl::pair<int, std::string> a(1, "hello");
  mystl::pair<int, std::string> b(mystl::move(a));
  EXPECT_EQ(b.first, 1);
  EXPECT_EQ(b.second, "hello");
}

TEST(PairTest, CopyAssign) {
  mystl::pair<int, int> a(1, 2);
  mystl::pair<int, int> b;
  b = a;
  EXPECT_EQ(b.first, 1);
  EXPECT_EQ(b.second, 2);
}

TEST(PairTest, MoveAssign) {
  mystl::pair<int, std::string> a(1, "hello");
  mystl::pair<int, std::string> b;
  b = mystl::move(a);
  EXPECT_EQ(b.first, 1);
  EXPECT_EQ(b.second, "hello");
}

TEST(PairTest, Swap) {
  mystl::pair<int, int> a(1, 2), b(3, 4);
  a.swap(b);
  EXPECT_EQ(a.first, 3);
  EXPECT_EQ(a.second, 4);
  EXPECT_EQ(b.first, 1);
  EXPECT_EQ(b.second, 2);
}

TEST(PairTest, EqualAndNotEqual) {
  mystl::pair<int, int> a(1, 2), b(1, 2), c(1, 3);
  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a != c);
}

TEST(PairTest, LessThan) {
  mystl::pair<int, int> a(1, 2), b(1, 3), c(2, 0);
  EXPECT_TRUE(a < b);   // same first, compare second
  EXPECT_TRUE(a < c);   // first differs
  EXPECT_FALSE(b < a);
}

TEST(PairTest, AllComparisons) {
  mystl::pair<int, int> a(1, 2), b(1, 3);
  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(b >= a);
  EXPECT_TRUE(b > a);
  EXPECT_FALSE(a > b);
}

TEST(PairTest, MakePair) {
  auto p = mystl::make_pair(1, 2);
  EXPECT_EQ(p.first, 1);
  EXPECT_EQ(p.second, 2);
}
