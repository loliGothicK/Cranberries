#ifndef CRANBERRIES_STREAMS_STREAM_CONVERTER_HPP
#define CRANBERRIES_STREAMS_STREAM_CONVERTER_HPP
#include <utility>
#include "detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace detail {

    //----------------------------//
    // stream to Range Converters //
    //----------------------------//


    // Converter temporary proxy object
    // Convert stream to any container
    template < class Stream >
    class ImplicitStreamConverter
    {
      Stream stream_;
    public:
      ImplicitStreamConverter(Stream stream) noexcept
        : stream_{ std::forward<Stream>(stream) }
      {}

      template < typename Target >
      inline operator Target() const noexcept(false) {
        return stream_. template convert<Target>();
      }
    };

    // Implicit convert traits
    struct ImplicitStreamConvertInvoker {
      template < typename Stream >
      static constexpr auto invoke(Stream&& stream) noexcept {
        return ImplicitStreamConverter<Stream>(std::forward<Stream>(stream));
      }
    };

    // Explicit convert traits
    template < class Target >
    struct ExplicitStreamConverter {
      template < typename Stream >
      static constexpr auto convert(Stream&& stream) {
        return Target{ stream.begin(), stream.end() };
      }
    };


    struct ConvertAny {};

    template <
      template<
        class T,
        class Allocator = std::allocator<T>
      >
      class Cont
    >
    struct ConvertTo
    {
      template < typename T >
      using type = Cont<T>;
    };




} // ! namespace detail

  inline constexpr detail::ConvertAny convert() noexcept { return{}; }

  template < template<class, class> class Target >
  inline constexpr detail::ConvertTo<Target> convert_to() noexcept { return{}; }

} // ! namespace stream
} // ! namespace cranberries

#endif