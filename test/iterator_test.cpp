#include "iterator.h"

#include <gtest/gtest.h>

// =============================================================================
// Iterator Traits
// =============================================================================

TEST(IteratorTraitsTest, PointerTraits) {
  using traits = mystl::iterator_traits<int*>;
  static_assert(
      std::is_same<traits::iterator_category,
                   mystl::random_access_iterator_tag>::value);
  static_assert(std::is_same<traits::value_type, int>::value);
  static_assert(std::is_same<traits::pointer, int*>::value);
  static_assert(std::is_same<traits::reference, int&>::value);
}

TEST(IteratorTraitsTest, ConstPointerTraits) {
  using traits = mystl::iterator_traits<const int*>;
  static_assert(
      std::is_same<traits::iterator_category,
                   mystl::random_access_iterator_tag>::value);
  static_assert(std::is_same<traits::value_type, int>::value);
  static_assert(std::is_same<traits::pointer, const int*>::value);
  static_assert(std::is_same<traits::reference, const int&>::value);
}

// =============================================================================
// Distance (原生指针走 random_access 特化)
// =============================================================================

TEST(DistanceTest, WithPointer) {
  int arr[] = {1, 2, 3, 4, 5};
  EXPECT_EQ(mystl::distance(arr, arr + 5), 5);
  EXPECT_EQ(mystl::distance(arr, arr), 0);
  EXPECT_EQ(mystl::distance(arr + 1, arr + 3), 2);
}

// =============================================================================
// Advance (原生指针走 random_access 特化)
// =============================================================================

TEST(AdvanceTest, ForwardWithPointer) {
  int arr[] = {1, 2, 3, 4, 5};
  int* p = arr;
  mystl::advance(p, 3);
  EXPECT_EQ(*p, 4);
}

TEST(AdvanceTest, BackwardWithPointer) {
  int arr[] = {1, 2, 3, 4, 5};
  int* p = arr + 4;
  mystl::advance(p, -2);
  EXPECT_EQ(*p, 3);
}

TEST(AdvanceTest, ZeroAdvance) {
  int arr[] = {1, 2, 3};
  int* p = arr + 1;
  mystl::advance(p, 0);
  EXPECT_EQ(*p, 2);
}
