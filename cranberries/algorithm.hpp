/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_ALGORITHM_HPP
#define CRANBERRIES_ALGORITHM_HPP

#include <iterator>
#include <cstdint>
#include <algorithm>
#include "type_traits.hpp"
#include "utility/utility.hpp"
#include "common/defaulted_type.hpp"
#include "integers.hpp"

namespace cranberries {

namespace cranberries_magic {

template<class BidirectionalIterator> 
  inline constexpr bool next_combination(BidirectionalIterator first1,
                        BidirectionalIterator last1,
                        BidirectionalIterator first2,
                        BidirectionalIterator last2)
  {
      if ((first1 == last1) || (first2 == last2)) {
          return false;
      }
      
      BidirectionalIterator m1 = last1;
      BidirectionalIterator m2 = last2; --m2;
      
      // Find first m1 not less than *m2 (i.e., lower_bound(first1, last1, *m2)).
      // Actually, this loop stops at one position before that, except perhaps
      // if m1 == first1 (in which case one must compare *first1 with *m2).
      while (--m1 != first1 && !(*m1 < *m2)) {
      }
      
      // Test if all elements in [first1, last1) not less than *m2.
      bool result = (m1 == first1) && !(*first1 < *m2);
      
      if (!result) {

          // Find first first2 greater than *m1 (since *m1 < *m2, we know it
          // can't pass m2 and there's no need to test m2).
          while (first2 != m2 && !(*m1 < *first2)) {
              ++first2;
          }
          
          first1 = m1;
          std::iter_swap (first1, first2);
          ++first1;
          ++first2;
      }
      
      // Merge [first1, last1) with [first2, last2), given that the rest of the
      // original ranges are sorted and compare appropriately.
      if ((first1 != last1) && (first2 != last2)) {      
          for (m1 = last1, m2 = first2;  (m1 != first1) && (m2 != last2); ++m2) {
              std::iter_swap (--m1, m2);
          }
          
          std::reverse (first1, m1);
          std::reverse (first1, last1);
          
          std::reverse (m2, last2);
          std::reverse (first2, last2);
      }
      
      return !result;
  }
    
template<class BidirectionalIterator, class Compare> 
  inline constexpr bool next_combination(BidirectionalIterator first1,
                        BidirectionalIterator last1,
                        BidirectionalIterator first2,
                        BidirectionalIterator last2, Compare comp)
  {
      if ((first1 == last1) || (first2 == last2)) {
          return false;
      }
      
      BidirectionalIterator m1 = last1;
      BidirectionalIterator m2 = last2; --m2;
      
      while (--m1 != first1 && !comp(*m1, *m2)) {
      }
      
      bool result = (m1 == first1) && !comp(*first1, *m2);
      
      if (!result) {
          
          while (first2 != m2 && !comp(*m1, *first2)) {
              ++first2;
          }

          first1 = m1; 
          std::iter_swap (first1, first2);
          ++first1;
          ++first2;
      }
      
      if ((first1 != last1) && (first2 != last2)) {      
          for (m1 = last1, m2 = first2;  (m1 != first1) && (m2 != last2); ++m2) {
              std::iter_swap (--m1, m2);
          }
          
          std::reverse (first1, m1);
          std::reverse (first1, last1);
          
          std::reverse (m2, last2);
          std::reverse (first2, last2);
      }
      
      return !result;
  }
  
}  // namespace cranberries_magic

/* PROPOSED STANDARD EXTENSIONS:
 *
 * template<class BidirectionalIterator> 
 *   bool next_partial_permutation(BidirectionalIterator first,
 *                                 BidirectionalIterator middle,
 *                                 BidirectionalIterator last); 
 *
 * template<class BidirectionalIterator, class Compare> 
 *   bool next_partial_permutation(BidirectionalIterator first,
 *                                 BidirectionalIterator middle,
 *                                 BidirectionalIterator last, Compare comp); 
 */
 
template <class BidirectionalIterator>
  inline constexpr bool next_partial_permutation(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last)
{
  using std::reverse;
  reverse(middle, last);
  return std::next_permutation(first, last);
}

template<class BidirectionalIterator, class Compare> 
  inline constexpr bool next_partial_permutation(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last, Compare comp)
{
  using std::reverse;
  reverse(middle, last);
  return std::next_permutation(first, last, comp);
}

/* PROPOSED STANDARD EXTENSIONS:
 *
 * template<class BidirectionalIterator> 
 *   bool prev_partial_permutation(BidirectionalIterator first,
 *                                 BidirectionalIterator middle,
 *                                 BidirectionalIterator last); 
 *
 * template<class BidirectionalIterator, class Compare> 
 *   bool prev_partial_permutation(BidirectionalIterator first,
 *                                 BidirectionalIterator middle,
 *                                 BidirectionalIterator last, Compare comp); 
 */
 
template<class BidirectionalIterator> 
  inline constexpr bool prev_partial_permutation(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last)
{
  bool result = prev_permutation(first, last);
  reverse (middle, last);
  return result;
}


template<class BidirectionalIterator, class Compare> 
  inline constexpr bool prev_partial_permutation(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last, Compare comp)
{
  bool result = prev_permutation(first, last, comp);
  reverse (middle, last);
  return result;
}

/* PROPOSED STANDARD EXTENSIONS:
 *
 * template<class BidirectionalIterator> 
 *   bool next_combination(BidirectionalIterator first,
 *                         BidirectionalIterator middle,
 *                         BidirectionalIterator last); 
 *
 * template<class BidirectionalIterator, class Compare> 
 *   bool next_combination(BidirectionalIterator first,
 *                         BidirectionalIterator middle,
 *                         BidirectionalIterator last, Compare comp); 
 */

template<class BidirectionalIterator> 
  inline constexpr bool next_combination(BidirectionalIterator first,
                        BidirectionalIterator middle,
                        BidirectionalIterator last)
  {
    return cranberries_magic::next_combination(first, middle, middle, last);
  }
    
template<class BidirectionalIterator, class Compare> 
  inline constexpr bool next_combination(BidirectionalIterator first,
                        BidirectionalIterator middle,
                        BidirectionalIterator last, Compare comp)
  {
      return cranberries_magic::next_combination(first, middle, middle, last, comp);
  }
 
/* PROPOSED STANDARD EXTENSIONS:
 *
 * template<class BidirectionalIterator> 
 *   bool prev_combination(BidirectionalIterator first,
 *                         BidirectionalIterator middle,
 *                         BidirectionalIterator last); 
 *
 * template<class BidirectionalIterator, class Compare> 
 *   bool prev_combination(BidirectionalIterator first,
 *                         BidirectionalIterator middle,
 *                         BidirectionalIterator last, Compare comp); 
 */
 
template<class BidirectionalIterator>
  inline
  constexpr bool prev_combination(BidirectionalIterator first,
                        BidirectionalIterator middle,
                        BidirectionalIterator last)
  {
    return cranberries_magic::next_combination(middle, last, first, middle);
  }
  
template<class BidirectionalIterator, class Compare> 
  inline
  constexpr bool prev_combination(BidirectionalIterator first,
                        BidirectionalIterator middle,
                        BidirectionalIterator last, Compare comp)
  {
    return cranberries_magic::next_combination(middle, last, first, middle, comp);
  }
  
/* PROPOSED STANDARD EXTENSION:
 *
 * template<class BidirectionalIterator, class T> 
 *   bool next_mapping(BidirectionalIterator first,
 *                     BidirectionalIterator last,
 *                     T first_value, T last_value); 
 *
 * template<class BidirectionalIterator, class T, class Incrementor> 
 *   bool next_mapping(BidirectionalIterator first,
 *                     BidirectionalIterator last,
 *                     T first_value, T last_value, Incrementor increment); 
*/
 
template <class BidirectionalIterator, class T>
  inline constexpr bool
  next_mapping(BidirectionalIterator first,
               BidirectionalIterator last,
               T first_value, T last_value)
{
    if (last == first ) {
        return false;
    }
    do {
        if (++(*(--last)) != last_value) {
            return true;
        }
        *last = first_value;
    } while (last != first);
    return false;
}

template <class BidirectionalIterator, class T, class Incrementer>
  inline constexpr bool
  next_mapping(BidirectionalIterator first,
               BidirectionalIterator last, 
               T first_value, T last_value, Incrementer increment)
{
    if (last == first ) {
        return false;
    }
    do {
        if (increment(*(--last)) != last_value) {
            return true;
        }
        *last = first_value;
    } while (last != first);
    return false;
}

/* PROPOSED STANDARD EXTENSION:
 *
 * template<class BidirectionalIterator, class T> 
 *   bool prev_mapping(BidirectionalIterator first,
 *                     BidirectionalIterator last,
 *                     T first_value, T last_value); 
 *
 * template<class BidirectionalIterator, class T, class Decrementor> 
 *   bool prev_mapping(BidirectionalIterator first,
 *                     BidirectionalIterator last,
 *                     T first_value, T last_value, Decrementor decrement); 
 */

template <class BidirectionalIterator, class T>
  inline constexpr bool
  prev_mapping(BidirectionalIterator first,
    	       BidirectionalIterator last,
               T first_value, T last_value)
{
    if (last == first) {
        return false;
    }
    --last_value;
    do {
        if (*(--last) != first_value) {
            --(*last);
            return true;
        }
        *last = last_value;
    } while (last != first);
    return true;
}

template <class BidirectionalIterator, class T, class Decrementer>
  inline constexpr bool
  prev_mapping(BidirectionalIterator first,
               BidirectionalIterator last, 
               T first_value, T last_value, Decrementer decrement)
{
    if (last == first) {
        return false;
    }
    decrement(last_value);
    do {
        if (*(--last) != first_value) {
            decrement(*last);
            return true;
        }
        *last = last_value;
    } while (last != first);
    return true;
}

/* PROPOSED STANDARD EXTENSION:
 *
 * template<class BidirectionalIterator, class T> 
 *   bool next_combination_counts(BidirectionalIterator first,
 *                                BidirectionalIterator last);
 */
 
template <class BidirectionalIterator>
  inline constexpr bool
  next_combination_counts(BidirectionalIterator first,
    	                    BidirectionalIterator last)
{
    BidirectionalIterator current = last;
    while (current != first && *(--current) == 0) {
    }
    if (current == first) {
        if (first != last && *first != 0)
            std::iter_swap(--last, first);
        return false;
    }
    --(*current);
    std::iter_swap(--last, current);
    ++(*(--current));
    return true;
}

/* PROPOSED STANDARD EXTENSION:
 *
 * template<class BidirectionalIterator> 
 *   bool prev_combination_counts(BidirectionalIterator first,
 *                                BidirectionalIterator last); 
 */

template <class BidirectionalIterator>
inline constexpr bool
prev_combination_counts(BidirectionalIterator first,
                        BidirectionalIterator last)
{
    if (first == last)
        return false;
    BidirectionalIterator current = --last;
    while (current != first && *(--current) == 0) {
    }
    if (current == last || (current == first && *current == 0)) {
        if (first != last)
            std::iter_swap(first, last);
        return false; 
    }
    --(*current);
    ++current;
    if (0 != *last) {
        std::iter_swap(current, last);
    }
    ++(*current);
    return true;
}

} // namespace cranberries

namespace cranberries {

  enum class sort_type {
    ascending,
    descending,
  };


  class default_get_key
  {
    template < class T >
    union to_radix_key {
      T value;
      cranberries::uint_fast_t<sizeof(T) * 8> key;
    };
  public:
    default_get_key() = default;

    template < class T, std::enable_if_t<!std::is_floating_point<std::decay_t<T>>::value, std::nullptr_t> = nullptr >
    auto operator()(T const& a) const noexcept
    {
      return endian::native == endian::little 
        ? byte_swap( to_radix_key<T>{ a }.key)
        : to_radix_key<T>{ a }.key;
    }
    template < class T, std::enable_if_t<std::is_floating_point<std::decay_t<T>>::value, std::nullptr_t> = nullptr >
    auto operator()(T const& a) const noexcept
    {
      return *reinterpret_cast<const int_t<sizeof(T)*8>*>(&a);
    }

  };

  template < class T >
  class reverse_key
  {
    union to_radix_key {
      cranberries::uint_fast_t<sizeof(T) * 8> key;
      T value;
    };
  public:
    reverse_key() = default;

    T operator()(cranberries::int_fast_t<sizeof(T) * 8> const& a) const noexcept
    {
      return endian::native == endian::little
        ? (to_radix_key{ byte_swap(a) }.value)
        : to_radix_key{ a }.value;
    }
  };

  template < sort_type, class F = cranberries_magic::defaulted_t >
  class radix_key;

  // Radix sort comparator for n-bit two's complement integers
  template < >
  class radix_key<sort_type::ascending>
  {
    size_t BITS;
    size_t bit;
  public:
    constexpr radix_key(size_t bits) noexcept
      : BITS{bits}, bit{} {}

    constexpr radix_key(size_t bits, size_t offset) noexcept
      : BITS{bits}, bit(offset) {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {

      if (bit == BITS-1)
        return value < 0; // negative value to left partition
      else
        return !(value & (1 << bit)); // 0 bit to left partition
    }
    operator bool() {
      return bit != BITS;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };




  // Radix sort comparator for n-bit two's complement integers
  template < >
  class radix_key<sort_type::descending>
  {
    size_t BITS;
    size_t bit;
  public:
    constexpr radix_key(size_t bits) noexcept
      : BITS{ bits }, bit{} {}

    constexpr radix_key(size_t bits, size_t offset) noexcept
      : BITS{ bits }, bit(offset) {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {
      
      if (bit == BITS-1)
        return !(value < 0); // negative value to left partition
      else
        return value & (1 << bit); // 0 bit to left partition
    }
    operator bool() {
      return bit != BITS;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };

  // Radix sort comparator for n-bit two's complement integers
  template < class F >
  class radix_key<sort_type::ascending, F>
  {
    size_t BITS;
    size_t bit;
    F get_key;
  public:
    constexpr radix_key(size_t bits, size_t e, F f) noexcept
      : BITS{bits}, bit{e}, get_key{f} {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {      
      if (bit == BITS - 1)
        return get_key(value) < 0; // negative value to left partition
      else
        return !(get_key(value) & (1 << bit)); // 0 bit to left partition
    }
    operator bool() {
      return bit != BITS;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };

  // Radix sort comparator for n-bit two's complement integers
  template < class F >
  class radix_key<sort_type::descending, F>
  {
    size_t BITS;
    size_t bit;
    F get_key;
  public:
    constexpr radix_key(size_t bits, size_t e, F f) noexcept
      : BITS{ bits }, bit{ e }, get_key{ f } {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {
      if (bit == BITS - 1)
        return !(get_key(value) < 0); // negative value to left partition
      else
        return get_key(value) & (1 << bit); // 0 bit to left partition
    }
    operator bool() {
      return bit != BITS;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };



  radix_key<sort_type::ascending> make_ascending_radix_key(size_t bits, size_t e = 0) {
    return { bits, e };
  }
  radix_key<sort_type::descending> make_descending_radix_key(size_t bits, size_t e = 0) {
    return { bits, e };
  }
  template < class F = default_get_key >
  radix_key<sort_type::ascending, F> make_ascending_radix_get_key(size_t bits, size_t e = 0, F f = default_get_key{}) {
    return { bits, e, f };
  }
  template < class F = default_get_key>
  radix_key<sort_type::descending, F> make_descending_radix_get_key(size_t bits, size_t e = 0, F f = default_get_key{}) {
    return { bits, e, f };
  }



  // Radix sort
  template <
    class ForwardIterator,
    enabler_t<std::is_integral<element_type_of_t<ForwardIterator>>::value> = nullptr
  >
  inline constexpr void ascending_radix_sort(ForwardIterator first, ForwardIterator last)
  {
    size_t BIT = sizeof(element_type_of_t<ForwardIterator>) * 8;

    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return v < 0; });

    auto radix_test = make_ascending_radix_key(BIT);

    // 
    for (; radix_test; radix_test.next())
      std::stable_partition(first, mid, radix_test);
    radix_test.reset();
    for (; radix_test; radix_test.next())
      std::stable_partition(mid, last, radix_test);
  }

  // Radix sort
  template <
    class ForwardIterator,
    enabler_t<std::is_integral<element_type_of_t<ForwardIterator>>::value> = nullptr
  >
  inline constexpr void descending_radix_sort(ForwardIterator first, ForwardIterator last)
  {
    size_t BIT = sizeof(element_type_of_t<ForwardIterator>) * 8;

    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return !(v < 0); });

    auto radix_test = make_descending_radix_key(BIT);

    // 
    for (; radix_test; radix_test.next())
      std::stable_partition(first, mid, radix_test);
    radix_test.reset();
    for (; radix_test; radix_test.next())
      std::stable_partition(mid, last, radix_test);
  }

  // Radix sort
  template <
    class ForwardIterator,
    class F,
    enabler_t<std::is_integral<std::result_of_t<F(element_type_of_t<ForwardIterator>)>>::value> = nullptr
  >
  inline constexpr void ascending_radix_sort(ForwardIterator first, ForwardIterator last, F&& get_key)
  {
    size_t BIT = sizeof(std::result_of_t<F(element_type_of_t<ForwardIterator>)>) * 8;

    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return v < 0; });

    auto radix_test = make_ascending_radix_get_key(BIT, 0, std::move(get_key));

    // 
    for (; radix_test; radix_test.next())
      std::stable_partition(first, mid, radix_test);
    radix_test.reset();
    for (; radix_test; radix_test.next())
      std::stable_partition(mid, last, radix_test);
  }

  // Radix sort
  template <
    class ForwardIterator,
    class F,
    enabler_t<std::is_integral<std::result_of_t<F(element_type_of_t<ForwardIterator>)>>::value> = nullptr
  >
  inline constexpr void descending_radix_sort(ForwardIterator first, ForwardIterator last, F&& get_key)
  {
    size_t BIT = sizeof(std::result_of_t<F(element_type_of_t<ForwardIterator>)>) * 8;

    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return !(v < 0); });

    auto radix_test = make_descending_radix_get_key(BIT, 0, std::move(get_key));

    // 
    for (; radix_test; radix_test.next())
      std::stable_partition(first, mid, radix_test);
    radix_test.reset();
    for (; radix_test; radix_test.next())
      std::stable_partition(mid, last, radix_test);
  }

  // Radix sort
  template <
    class ForwardIterator,
    size_t BIT = sizeof(element_type_of_t<ForwardIterator>) * 8,
    enabler_t<!std::is_integral<element_type_of_t<ForwardIterator>>::value> = nullptr
  >
  void ascending_radix_sort(ForwardIterator first, ForwardIterator last) = delete;

  // Radix sort
  template <
    class ForwardIterator,
    size_t BIT = sizeof(element_type_of_t<ForwardIterator>) * 8,
    enabler_t<!std::is_integral<element_type_of_t<ForwardIterator>>::value> = nullptr
  >
  void descending_radix_sort(ForwardIterator first, ForwardIterator last) = delete;


} // ! namespace cranberries

#endif
