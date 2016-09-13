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
		template < class STREAM >
		class ImplicitStreamConverter
		{
			STREAM stream_;
		public:
			ImplicitStreamConverter(STREAM stream) : stream_{ std::forward<STREAM>(stream) } {}

			template < typename Target >
			inline operator Target() const {
				return stream_. template convert<Target>();
			}
		};

		// Implicit convert traits
		struct ImplicitStreamConvertInvoker {
			template < typename STREAM >
			static constexpr auto invoke(STREAM&& stream) {
				return ImplicitStreamConverter<STREAM>(std::forward<STREAM>(stream));
			}
		};

		// Explicit convert traits
		template < class Target >
		struct ExplicitStreamConverter {
			template < typename STREAM >
			static constexpr auto convert(STREAM&& stream) {
				return Target{ stream.begin(), stream.end() };
			}
		};


		struct ConvertAny {};

		template < template<class T, class Allocator = std::allocator<T>> class Cont >
		struct ConvertTo {
			template < typename T >
			using type = Cont<T>;
		};


		// Convert Terminate Operation
		template <
			typename STREAM,
			template<class, class> class Target
		>
			inline constexpr decltype(auto) operator || (STREAM&& stream, ConvertTo<Target>) noexcept {
			return ExplicitStreamConverter<
				typename ConvertTo<Target>::template type<
				typename std::decay_t<STREAM>::element_type
				>
			>::convert(std::forward<STREAM>(stream.eval()));
		}



} // ! namespace detail

	constexpr detail::ConvertAny convert{};

	template < template<class, class> class Target >
	constexpr auto convert_to = detail::ConvertTo<Target>{};

	inline constexpr detail::ConvertAny converter() noexcept { return{}; }

	template < template<class, class> class Target >
	inline constexpr detail::ConvertTo<Target> converter() noexcept { return{}; }

} // ! namespace stream
} // ! namespace cranberries

#endif