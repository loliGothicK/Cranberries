#ifndef CRANBERRIES_FUNCTIONAL_HPP
#define CRANBERRIES_FUNCTIONAL_HPP
#ifdef _MSC_VER
#pragma once
#endif

namespace cranberries {
inline namespace experimental {

	template < class T = void >
	struct less {
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(T const& a, T const& b) {
			return a < b;
		}
	};

	template < class T = void >
	struct greater {
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(T const& a, T const& b) {
			return !(a < b);
		}
	};

	template < class T = void >
	struct less_or_equal {
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(T const& a, T const& b) {
			return a < b || !(a != b);
		}
	};

	template < class T = void >
	struct greater_or_equal {
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(T const& a, T const& b) {
			return !(a < b) || !(a != b);
		}
	};

	template < class T = void >
	struct equal_to {
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(T const& a, T const& b) {
			return !(a != b);
		}
	};

	template < class T = void >
	struct not_equal_to {
		using first_argument_type = T;
		using second_argument_type = T;
		using result_type = bool;

		bool operator()(T const& a, T const& b) {
			return a != b;
		}
	};

	template < >
	struct less<void> {
		using is_transparent = std::nullptr_t;

		template < class T1, class T2 >
		bool operator()(T1&& a, T2&& b) {
			return std::forward<T1>(a) < std::forward<T2>(b);
		}
	};

	template < >
	struct greater<void> {
		using is_transparent = std::nullptr_t;

		template < class T1, class T2 >
		bool operator()(T1&& a, T2&& b) {
			return !(std::forward<T1>(a) < std::forward<T2>(b));
		}
	};

	template < >
	struct less_or_equal<void> {
		using is_transparent = std::nullptr_t;

		template < class T1, class T2 >
		bool operator()(T1&& a, T2&& b) {
			return std::forward<T1>(a) < std::forward<T2>(b) || !(std::forward<T1>(a) != std::forward<T2>(b));
		}
	};

	template < >
	struct greater_or_equal<void> {
		using is_transparent = std::nullptr_t;

		template < class T1, class T2 >
		bool operator()(T1&& a, T2&& b) {
			return !(std::forward<T1>(a) < std::forward<T2>(b)) || !(std::forward<T1>(a) != std::forward<T2>(b));
		}
	};

	template < >
	struct equal_to<void> {
		using is_transparent = std::nullptr_t;

		template < class T1, class T2 >
		bool operator()(T1&& a, T2&& b) {
			return !(std::forward<T1>(a) != std::forward<T2>(b));
		}
	};

	template < >
	struct not_equal_to<void> {
		using is_transparent = std::nullptr_t;

		template < class T1, class T2 >
		bool operator()(T1&& a, T2&& b) {
			return std::forward<T1>(a) != std::forward<T2>(b);
		}
	};

}}
#endif // CRANBERRIES_FUNCTIONAL_HPP