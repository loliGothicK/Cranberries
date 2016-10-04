#ifndef CRANBERRIES_INTEGERS_HPP
#define CRANBERRIES_INTEGERS_HPP
#include <cstdint>


namespace cranberries {


  template < std::size_t N >
  struct int_t_traits;

  template < >
  struct int_t_traits<8> {
    using type = std::int8_t;
  };

  template < >
  struct int_t_traits<16> {
    using type = std::int16_t;
  };

  template < >
  struct int_t_traits<32> {
    using type = std::int32_t;
  };

  template < >
  struct int_t_traits<64> {
    using type = std::int64_t;
  };

  template < std::size_t N >
  using int_t = typename int_t_traits<N>::type;
}


#endif