#ifndef CRANBERRIES_RADIX_SORT_HPP
#define CRANBERRIES_RADIX_SORT_HPP

#include <iterator>
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include "../../type_traits.hpp"
#include "../integers.hpp"

namespace cranberries {

  enum class sort_type {
    ascending,
    descending,
  };

  template < typename T >
  auto byte_swap(T&& v) {
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&v);
    std::reverse(bytes, bytes + sizeof(T));
    return *reinterpret_cast<cranberries::uint_t<sizeof(T) * 8>*>(bytes);
  }

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

  template < sort_type, size_t, class F = cranberries_magic::defaulted_t >
  class radix_key;

  // Radix sort comparator for n-bit two's complement integers
  template < size_t N >
  class radix_key<sort_type::ascending, N>
  {
    size_t bit;
  public:
    constexpr radix_key() noexcept
      : bit{} {}

    constexpr radix_key(size_t offset) noexcept
      : bit(offset) {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {

      if (bit == N-1)
        return value < 0; // negative value to left partition
      else
        return !(value & (1 << bit)); // 0 bit to left partition
    }
    operator bool() {
      return bit != N;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };




  // Radix sort comparator for n-bit two's complement integers
  template < size_t N >
  class radix_key<sort_type::descending, N>
  {
    size_t bit;
  public:
    constexpr radix_key() noexcept
      : bit{} {}

    constexpr radix_key(size_t offset) noexcept
      : bit(offset) {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {
      
      if (bit == N-1)
        return !(value < 0); // negative value to left partition
      else
        return value & (1 << bit); // 0 bit to left partition
    }
    operator bool() {
      return bit != N;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };

  // Radix sort comparator for n-bit two's complement integers
  template < size_t N, class F >
  class radix_key<sort_type::ascending, N, F>
  {
    size_t bit;
    F get_key;
  public:
    constexpr radix_key(size_t e, F f) noexcept
      : bit{e}, get_key{f} {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {      
      if (bit == N - 1)
        return get_key(value) < 0; // negative value to left partition
      else
        return !(get_key(value) & (1 << bit)); // 0 bit to left partition
    }
    operator bool() {
      return bit != N;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };

  // Radix sort comparator for n-bit two's complement integers
  template < size_t N, class F >
  class radix_key<sort_type::descending, N, F>
  {
    size_t bit;
    F get_key;
  public:
    constexpr radix_key(size_t e, F f) noexcept
      : bit{ e }, get_key{ f } {}

    template < typename T >
    constexpr bool operator()(T value) const // function call operator
    {
      if (bit == N - 1)
        return !(get_key(value) < 0); // negative value to left partition
      else
        return get_key(value) & (1 << bit); // 0 bit to left partition
    }
    operator bool() {
      return bit != N;
    }
    void next() {
      ++bit;
    }
    void reset() {
      bit = 0;
    }
  };



  template < size_t N >
  radix_key<sort_type::ascending, N> make_ascending_radix_key(size_t e = 0) {
    return { e };
  }
  template < size_t N >
  radix_key<sort_type::descending, N> make_descending_radix_key(size_t e = 0) {
    return { e };
  }
  template < size_t N, class F = default_get_key>
  radix_key<sort_type::ascending, N, F> make_ascending_radix_get_key(size_t e = 0, F f = default_get_key{}) {
    return { e, f };
  }
  template < size_t N, class F = default_get_key>
  radix_key<sort_type::descending, N, F> make_descending_radix_get_key(size_t e = 0, F f = default_get_key{}) {
    return { e, f };
  }



  // Radix sort
  template <
    class ForwardIterator,
    size_t BIT = sizeof(element_type_of_t<ForwardIterator>) * 8,
    enabler_t<std::is_integral<element_type_of_t<ForwardIterator>>::value> = nullptr
  >
  void ascending_radix_sort(ForwardIterator first, ForwardIterator last)
  {
    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return v < 0; });

    auto radix_test = make_ascending_radix_key<BIT>();

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
    enabler_t<std::is_integral<element_type_of_t<ForwardIterator>>::value> = nullptr
  >
  void descending_radix_sort(ForwardIterator first, ForwardIterator last)
  {
    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return !(v < 0); });

    auto radix_test = make_descending_radix_key<BIT>();

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
    size_t BIT = sizeof(std::result_of_t<F(element_type_of_t<ForwardIterator>)>) * 8,
    enabler_t<std::is_integral<std::result_of_t<F(element_type_of_t<ForwardIterator>)>>::value> = nullptr
  >
  void ascending_radix_sort(ForwardIterator first, ForwardIterator last, F&& get_key)
  {
    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return v < 0; });

    auto radix_test = make_ascending_radix_get_key<BIT>(0, std::move(get_key));

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
    size_t BIT = sizeof(std::result_of_t<F(element_type_of_t<ForwardIterator>)>) * 8,
    enabler_t<std::is_integral<std::result_of_t<F(element_type_of_t<ForwardIterator>)>>::value> = nullptr
  >
  void descending_radix_sort(ForwardIterator first, ForwardIterator last)
  {
    // partition negative number to left
    auto mid = std::stable_partition(first, last, [](auto v) { return !(v < 0); });

    auto radix_test = make_descending_radix_get_key<BIT>(0, std::move(get_key));

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