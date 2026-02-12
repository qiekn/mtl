// MySTL
//
// This header defines the iterator infrastructure: iterator tags for
// category dispatching, an iterator base class template, iterator_traits
// for uniform type extraction (including specializations for raw pointers),
// and the distance/advance helper functions.

#pragma once

#include <cstddef>

namespace mystl {

// Iterator category tags.
//
// These empty structs form an inheritance hierarchy that mirrors iterator
// capabilities. Algorithms use tag dispatch to select the most efficient
// implementation for a given iterator category.
//
// Hierarchy:
//   input_iterator_tag
//       |
//   forward_iterator_tag
//       |
//   bidirectional_iterator_tag
//       |
//   random_access_iterator_tag

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// Iterator base class.
//
// Custom iterators can inherit from this to automatically provide the
// five required associated types. This struct holds no data members.
//
// Template parameters:
//   Category  - one of the iterator tag types above
//   T         - the element type the iterator points to
//   Distance  - type for representing distance between iterators
//   Pointer   - pointer-to-element type
//   Reference - reference-to-element type

template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
  typedef Category  iterator_category;
  typedef T         value_type;
  typedef Distance  difference_type;
  typedef Pointer   pointer;
  typedef Reference reference;
};

// Iterator traits.
//
// Provides a uniform interface to access the associated types of any
// iterator, whether it is a class type with nested typedefs or a raw
// pointer.
//
// Primary template: extracts types from Iterator's own typedefs.
// Partial specialization for T*: treats raw pointers as random-access
// iterators.
// Partial specialization for const T*: same as T* but with const-
// qualified pointer and reference. Note that value_type is T (without
// const) by convention.

// Primary template for class-type iterators.
template <class Iterator>
struct iterator_traits {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type        value_type;
  typedef typename Iterator::difference_type   difference_type;
  typedef typename Iterator::pointer           pointer;
  typedef typename Iterator::reference         reference;
};

// Specialization for raw pointers.
template <class T>
struct iterator_traits<T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T                          value_type;
  typedef ptrdiff_t                  difference_type;
  typedef T*                         pointer;
  typedef T&                         reference;
};

// Specialization for const pointers.
template <class T>
struct iterator_traits<const T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T                          value_type;
  typedef ptrdiff_t                  difference_type;
  typedef const T*                   pointer;
  typedef const T&                   reference;
};

// Convenience type aliases for iterator_traits access.

template <class Iterator>
using iter_difference_t = typename iterator_traits<Iterator>::difference_type;

template <class Iterator>
using iter_value_t = typename iterator_traits<Iterator>::value_type;

// distance: returns the number of increments needed to go from first
// to last.
//
// For input iterators, this is O(n). For random-access iterators,
// this is O(1) via pointer subtraction.

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last,
                  input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}

template <class RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
distance_dispatch(RandomIterator first, RandomIterator last,
                  random_access_iterator_tag) {
  return last - first;
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
  typedef typename iterator_traits<InputIterator>::iterator_category category;
  return distance_dispatch(first, last, category());
}

// advance: moves iterator i forward (or backward) by n positions.
//
// For input iterators, only non-negative n is supported (O(n)).
// For bidirectional iterators, n may be negative (O(|n|)).
// For random-access iterators, this is O(1) via +=.

template <class InputIterator, class Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
  while (n--) ++i;
}

template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n,
                      bidirectional_iterator_tag) {
  if (n >= 0)
    while (n--) ++i;
  else
    while (n++) --i;
}

template <class RandomIterator, class Distance>
void advance_dispatch(RandomIterator& i, Distance n,
                      random_access_iterator_tag) {
  i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n) {
  typedef typename iterator_traits<InputIterator>::iterator_category category;
  advance_dispatch(i, n, category());
}

}  // namespace mystl
