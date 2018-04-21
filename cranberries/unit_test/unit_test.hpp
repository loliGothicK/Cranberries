#ifndef CRANBERRIES_UNIT_TEST_HPP
#define CRANBERRIES_UNIT_TEST_HPP
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <mutex>
#include <future>
#include <thread>
#include <chrono>
#include <numeric>
#include <sstream>
#include <utility>
#include <tuple>
#include <string>
#include <initializer_list>
#include "../algorithm.hpp"
#include "../common/concepts.hpp"
#include "../utility/utility.hpp"
#include "../traversals/views/zip_with.hpp"
#ifdef _WIN32
#include <Windows.h>
#endif

namespace cranberries {

	namespace printable_extensons {
		template < class ...Ts >
		std::ostream& operator<<(std::ostream& ss, std::tuple<Ts...> const& tup) {
			cranberries::apply([&](const auto & head, const auto&... tail) {
				ss << "(" << head;
				(void)cranberries::SwallowsNest{ (void(ss << "," << tail), 0)... };
				ss << ")";
			}, tup);
			return ss;
		}
	}

	struct ConsoleTestColor {
#ifndef _WIN32
		static void Red() {
			std::cout << "\x1b[31m";
		}
		static void Green() {
			std::cout << "\x1b[32m";
		}
		static void Yellow() {
			std::cout << "\x1b[33m";
		}
		static void Blue() {
			std::cout << "\x1b[34m";
		}
		static void Reset() {
			std::cout << "\x1b[0m";
		}
#else
		static void Reset()
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		}

		static void Red()
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		}

		static void Green()
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		}

		static void White()
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		}
		static void Yellow() {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		static void Blue() {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		}
#endif
	};


namespace unit_test_framework {

	struct result_collect {};

	constexpr result_collect collect{};

	template < class F = void >
	struct logger {
		F f_;

		template < class T >
		logger& operator << (T&& val) {
			f_( std::forward<T>(val) );
			return *this;
		}
	};

	template < >
	struct logger <void> {
		template < class T >
		logger& operator << (T&& val) {
			std::cout << std::forward<T>(val);
			return *this;
		}
	};
	template < class F >
	logger<F> make_logger(F&& f) { return { std::forward<F>(f) }; }
	
	constexpr auto default_logger = logger<>{};

namespace detail_ {

	struct assert_except
		: public std::runtime_error
	{
		assert_except(std::string info)
			: std::runtime_error{info}
		{}
	};


	class test_method_tag {};
}
	enum class test_status : unsigned {
		failed = 0,
		passed = 1,
		skipped = 2,
	};

	test_status operator & (test_status a, test_status b) {
		if (a == test_status::failed || b == test_status::failed)
			return test_status::failed;
		else if (a == test_status::skipped || b == test_status::skipped)
			return test_status::skipped;
		else return test_status::passed;
	}

	test_status operator ^ (test_status a, test_status b) {
		return static_cast<test_status>(static_cast<unsigned>(a) ^ static_cast<unsigned>(b) & 1);
	}


	test_status& operator &= (test_status& a, test_status b) {
		return a = a&b;
	}

	test_status& operator &= (test_status& a, bool b) {
		return a = a&static_cast<test_status>(b);
	}

	bool operator ! (test_status s) {
		return s == test_status::failed ? true : false;
	}

	std::ostream& operator << (std::ostream& os, test_status stat) {
		switch (stat)
		{
		case cranberries::unit_test_framework::test_status::failed:
			return os << "failed";
		case cranberries::unit_test_framework::test_status::passed:
			return os << "passed";
		case cranberries::unit_test_framework::test_status::skipped:
			return os << "skipped";
		}
	}



	class test_result_t {
		test_status status_;
		std::string what_;
	public:
		test_result_t(test_status stat, std::string msg = "default msg")
			: status_{ stat }, what_{ msg }
		{}
		test_result_t(std::string msg)
			: status_{ test_status::failed }, what_{ msg }
		{}
		test_result_t(const char* msg)
			: status_{ test_status::failed }, what_{ msg }
		{}


		test_result_t() : test_result_t{ test_status::skipped } {}
		test_result_t(test_result_t const&) = default;
		test_result_t(test_result_t&&) = default;
		test_result_t& operator=(test_result_t const&) = default;
		test_result_t& operator=(test_result_t&&) = default;


		test_status status() const { return status_; }
		std::string what() const { return what_; }
	};
namespace detail_{

	namespace info_ {
		template < class Range, class Elem = typename std::decay_t<Range>::value_type,
			concepts::concept_required<Range, concepts::iterable> = nullptr,
			concepts::concept_required<Elem, concepts::printable> = nullptr >
		std::string to_string(Range&& range) {
			std::stringstream ss;
			auto iter = ::cranberries::back_magic::begin(range);
			auto last = ::cranberries::back_magic::end(range);
			ss << "(" << *iter; ++iter;
			for (; iter != last; ++iter) ss << "," << *iter;
			ss << ")";
			return ss.str();
		}

		template < class Range, class Elem = typename std::decay_t<Range>::value_type,
			std::enable_if_t<is_tuple_v<Elem>, std::nullptr_t> = nullptr,
			concepts::concept_required<Range, concepts::iterable> = nullptr
		>
		std::string to_string(Range&& range) {
			std::stringstream ss;
			auto iter = ::cranberries::back_magic::begin(range);
			auto last = ::cranberries::back_magic::end(range);
			ss << "(";
			tuple_print(*iter, ss); ++iter;
			for (; iter != last; ++iter)
			{
				ss << ",";
				tuple_print(*iter, ss);
			}
			ss << ")";
			return ss.str();
		}


	}



	template < class F >
	class TestMethod {
		std::shared_ptr<F> f_;
		std::string label_;

		std::chrono::duration<double, std::milli> elapsed_;
		test_result_t result_;
	public:
		TestMethod(std::shared_ptr<F> f) : f_{ std::move(f) }, label_{} {}
		TestMethod(std::shared_ptr<F> f, std::string _) : f_{ std::move(f) }, label_{ _ } {}
		TestMethod(TestMethod&&) = default;
		TestMethod(TestMethod const&) = default;
		TestMethod& operator=(TestMethod&&) = default;
		TestMethod& operator=(TestMethod const&) = default;

		decltype(auto) index(size_t index) {
			label_ = std::string{ "#" } + std::to_string(index) + ": " + label_;
			return *this;
		}
		void exe() {
			auto first = std::chrono::high_resolution_clock::now();
			result_ = (*f_)();
			auto last = std::chrono::high_resolution_clock::now();
			elapsed_ = last - first;
		}
		template < class F >
		void print(logger<F>& logger) {
			switch (result_.status())
			{
			case test_status::failed:
				ConsoleTestColor::Red();
				logger << label_ << ": => fail! " << elapsed_.count() << "[ms]\n" << result_.what() << "\n";
				ConsoleTestColor::Reset();
				break;
			case test_status::passed:
				ConsoleTestColor::Green();
				logger << label_ << ": => passed. " << elapsed_.count() << "[ms]\n";
				ConsoleTestColor::Reset();
				break;
			case test_status::skipped:
				ConsoleTestColor::Yellow();
				logger << label_ << ": => skipped. " << elapsed_.count() << "[ms]\n";
				ConsoleTestColor::Reset();
				break;
			}
		}

		auto status() { return result_.status(); }
	};

	template < class F >
	struct labeled_method {
		F f_;
		std::string label_;
	};


	template < class F, enabler_t<!std::is_base_of<detail_::test_method_tag, std::decay_t<F>>::value> = nullptr >
	TestMethod<F> make_test_method(F&& f) {
		return { std::make_shared<F>(std::forward<F>(f)), "test method" };
	}

	template < class F, enabler_t<std::is_base_of<detail_::test_method_tag, std::decay_t<F>>::value> =nullptr >
	TestMethod<F> make_test_method(F&& f) {
		return { std::make_shared<F>(std::forward<F>(f)), std::decay_t<F>::label() };
	}


	template < class F >
	TestMethod<F> make_test_method(F&& f, std::string label) {
		return { std::make_shared<F>(std::forward<F>(f)), label };
	}
}

	template < class LOGGER_ >
	class UnitTestContainer {
		// members
		LOGGER_ logger; // for output
		std::string name; // unit test label
		decltype(std::chrono::high_resolution_clock::now()) start; // time point of first push

		std::once_flag once; // flag for initialize time point first push
		std::mutex mtx_; // for lock the ostream at logging
		std::vector<std::future<test_status>> methods{}; // future container
		size_t index{}; // for labeling n-th push

	public:
		// The following four are valid constructor calls.
		// default logger is std::cout and default label is "Unit Test"

		// UnitTestContainer( logger, "label" )
		// UnitTestContainer( logger ) <=> UnitTestContainer( logger, "Unit Test" )
		// UnitTestContainer( "label" ) <=> UnitTestContainer( std::cout, "label" )
		// UnitTestContainer( ) <=> UnitTestContainer( std::cout, "Unit Test" )

		// constructor [ logger, label ]
		UnitTestContainer(LOGGER_ lg, std::string name)
			: logger{ lg }
			, name{ name }
		{
			logger << "\n" << name << " Start...\n";
		}
		// constructor [ label, logger ]
		UnitTestContainer(std::string name, LOGGER_ lg)
			: UnitTestContainer{ lg, name }
		{}


		~UnitTestContainer() = default;
		
		auto collect()
		{
			// test status conters
			size_t passed{};
			size_t failed{};
			size_t skipped{};

			// test status count
			// future::get call here
			for (auto&& method : methods) {
				switch (method.get())
				{
				case test_status::failed:
					++failed;
					break;
				case test_status::passed:
					++passed;
					break;
				case test_status::skipped:
					++skipped;
					break;
				}
			}
			// Calculate the elapsed real time for the test
			std::chrono::duration<double, std::milli> total_ = std::chrono::high_resolution_clock::now() - start;

			// Green or Red
			if(failed == 0){
				ConsoleTestColor::Green();
				logger << "Test Status: Green.\n";
			}
			else if (failed == 1){
				logger << "Test Status:";
				ConsoleTestColor::Red();
				logger << "Red";
				ConsoleTestColor::Reset();
				logger << " [1 error generated]\n";
			}
			else{
				logger << "Test Status: ";
				ConsoleTestColor::Red();
				logger << "Red";
				ConsoleTestColor::Reset();
				logger << " [" << failed << " errors generated]\n";
			}

			// Test Summry Infomations
			logger << "Total Tests: " << index << " | ";
			ConsoleTestColor::Green();
			logger << "Passed: " << passed;
			ConsoleTestColor::Reset();
			logger << " | ";
			ConsoleTestColor::Red();
			logger << "Failed: " << failed;
			ConsoleTestColor::Reset();
			logger << " | ";
			ConsoleTestColor::Yellow();
			logger << "Skipped: " << skipped;
			ConsoleTestColor::Reset();
			logger << " |\n" << name << " End : Total Real Time = " << std::fixed << std::setprecision(5);
			(total_.count() < 1000
				? (logger << total_.count() << " [ms]")
				: (logger << total_.count() / 1000 << " [sec]")) << "\n\n";


			return failed != 0;
		}

		void operator|(result_collect) {
			if (this->collect()) throw detail_::assert_except{this->name + "container failed!"};
		}

		
		// operator for pushing test method
		// Test method is executed in parallel as soon as pushing
		template < class F,
			enabler_t<
				is_callable_v<F(), test_result_t>
			> = nullptr >
		decltype(auto) operator%(F&& f) & {
			std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

			methods.emplace_back(std::async(std::launch::async, // !explicit specified async
				[&, f = detail_::make_test_method(std::move(f)).index(++index)]() mutable -> test_status {
					f.exe();
					auto_lock(mtx_), f.print(logger);
					return f.status();
			}));
			return *this;
		}

		// operator for pushing test method
		// Test method is executed in parallel as soon as pushing
		template < class F,
			enabler_t<
				is_callable_v<F(), test_result_t>
			> = nullptr >
		decltype(auto) operator%(F&& f) && {
			std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

			methods.emplace_back(std::async(std::launch::async, // !explicit specified async
				[&, f = detail_::make_test_method(std::move(f)).index(++index)]() mutable->test_status {
				f.exe();
				auto_lock(mtx_), f.print(logger);
				return f.status();
			}));
			return std::move(*this);
		}


		// operator for pushing test method with test method label
		template < class F,
			enabler_t<
				is_callable_v<F(), test_result_t>
			> = nullptr >
		decltype(auto) operator%(detail_::labeled_method<F>&& lm) & {
			std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

			methods.emplace_back(std::async(std::launch::async, // !explicit specified async
				[&, f = make_test_method(std::move(lm.f_), lm.label_).index(++index)]() mutable->test_status {
				f.exe();
				auto_lock(mtx_), f.print(logger);
				return f.status();
			}));
			return *this;
		}

		// operator for pushing test method with test method label
		template < class F,
			enabler_t<
				is_callable_v<F(), test_result_t>
			> = nullptr >
		decltype(auto) operator%(detail_::labeled_method<F>&& lm) && {
			std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

			methods.emplace_back(std::async(std::launch::async, // !explicit specified async
				[&, f = make_test_method(std::move(lm.f_), lm.label_).index(++index)]() mutable->test_status {
				f.exe();
				auto_lock(mtx_), f.print(logger);
				return f.status();
			}));
			return *this;
		}

	};

	template < class LOGGER_ = logger<> >
	UnitTestContainer<LOGGER_>
		make_unit_test_container(LOGGER_ lg = default_logger, std::string name = "UNIT TEST")
	{
		return { lg, name };
	}

	template < class LOGGER_ = logger<> >
	UnitTestContainer<LOGGER_>
		make_unit_test_container(std::string name = "UNIT TEST", LOGGER_ lg = default_logger)
	{
		return { lg, name };
	}


namespace detail_ {

	template < class Derived >
	struct enable_labeled
	{
		auto labeled(std::string label) {
			return labeled_method<Derived>{ std::move(*static_cast<Derived*>(this)), label };
		}
	};

	template < class Expr, class Generator >
	class ScrambleExe
		: public enable_labeled<ScrambleExe<Expr, Generator>>
		, private detail_::test_method_tag
	{
		Expr expr_;
		Generator generator_;
		size_t times_;
	public:
		ScrambleExe(Expr expr, Generator gen, size_t n)
			: expr_{ expr }
			, generator_{ gen }
			, times_{ n }
		{}

		test_result_t operator()() {
			for (size_t i{}; i < times_; ++i)
				cranberries::apply(expr_, generator_());
			return test_status::passed;
		}

		static std::string label() { return "scramble execute"; }
	};

	template < size_t N, class Expr, class Require, class Tuple >
	class PermutationExe
		: private detail_::test_method_tag
		, public enable_labeled<PermutationExe<N, Expr, Require, Tuple>>
	{
		Expr expr_;
		Require require_;
		Tuple sample_;
	public:
		PermutationExe(Expr expr, Require require, Tuple t)
			: expr_{ expr }
			, require_{ require }
			, sample_( t )
		{}

		template < size_t... I >
		test_result_t execute(std::index_sequence<I...>) {
			std::vector<size_t> indices(std::tuple_size<std::decay_t<Tuple>>::value);
			std::iota(std::begin(indices), std::end(indices), 0);
			test_status exit_status = test_status::passed;
			do
			{
				exit_status &= require_(expr_(sample_[indices[I]]...));
			} while (cranberries::next_partial_permutation(indices.begin(), indices.begin() + N, indices.end()));
			if (!exit_status) return std::string{ "Info> Not satisfied requirements." };
			else return exit_status; 
		}

		test_result_t operator()() {
			return execute(std::make_index_sequence<N>{});
		}

		static std::string label() { return "permutation execute"; }
	};

	template < size_t N, class Expr, class Sample >
	class CombinationExe
		: private detail_::test_method_tag
		, public enable_labeled<CombinationExe<N, Expr, Sample>>
	{
		Expr expr_;
		Sample sample_;
	public:
		CombinationExe(Expr expr, Sample t)
			: expr_{ expr }
			, sample_( t )
		{}

		template < size_t... I >
		test_result_t execute(std::index_sequence<I...>) {
			std::vector<size_t> indices(std::tuple_size<std::decay_t<Sample>>::value);
			std::iota(std::begin(indices), std::end(indices), 0);
			do
			{
				expr_(sample_[indices[I]]...);
			} while (cranberries::next_combination(indices.begin(), indices.begin() + N, indices.end()));
			return test_status::passed;
		}

		test_result_t operator()() {
			return execute(std::make_index_sequence<N>{});
		}

		static std::string label() { return "combination execute"; }
	};

	namespace infomation {
		using namespace printable_extensons;

		template < class, class, class = void >
		struct AreEqualInfo {
			static std::string invoke(...) {
				return "Info> Assertion failure: in are equal.";
			}
		};

		template < class A, class E >
		struct AreEqualInfo<A,E,
			::cranberries::void_t<decltype(
				std::declval<std::stringstream&>() << std::declval<const A&>(),
				std::declval<std::stringstream&>() << std::declval<const E&>())>>
		{
			static std::string invoke(const A& a, const E& e) {
				using printable_extensons::operator<<;
				std::stringstream ss{ "Info> Assertion failure: " };
				ss << "'" << a << "'"
					<< " expected but "
					<< "'" << e << "'"
					<< " actual.";
				return ss.str();
			}
		};
	}

	template < class Actual, class Expected >
	class AreEqual
		: private detail_::test_method_tag
		, public enable_labeled<AreEqual<Actual, Expected>>
	{
		Actual actual_;
		Expected expect_;
	public:
		AreEqual(Actual actual, Expected expect)
			: actual_{ actual }
			, expect_{ expect }
		{}

		test_result_t operator()() {
			if(actual_ == expect_) return test_status::passed;
			else return infomation::AreEqualInfo<Actual, Expected>::invoke(actual_, expect_);
		}

		static std::string label() { return "are equal"; }
	};

	namespace infomation {
		using namespace printable_extensons;

		template < class, class = void >
		struct RangeEqualInfo {
			static std::string invoke(...) {
				return "Info> Assertion failure: in range equal.";
			}
		};

		template < class Range >
		struct RangeEqualInfo<Range,
			::cranberries::void_t<decltype(
				std::declval<std::stringstream&>() << std::declval<typename std::decay_t<Range>::value_type>())>>
		{
			template < class T1, class T2 >
			static std::string invoke(T1&& range_, T2&& expect_) {
				using printable_extensons::operator<<;
				return std::string{ "Info> Assertion failure: " }
					+"'" + info_::to_string(std::forward<T2>(expect_)) + "'"
					+ " expected but "
					+ "'" + info_::to_string(std::forward<T1>(range_)) + "'"
					+ " actual.";
			}
		};
	}


	template < class Range >
	class RangeEqual
		: private detail_::test_method_tag
		, public enable_labeled<RangeEqual<Range>>
	{
		Range range_;
		std::initializer_list<typename std::decay_t<Range>::value_type> expect_;

	public:
		template < class Range_ >
		RangeEqual(Range_&& range, std::initializer_list<typename std::decay_t<Range>::value_type> expect)
			: range_{ std::forward<Range_>(range) }
			, expect_( expect )
		{}
		RangeEqual(RangeEqual const&) = default;
		RangeEqual& operator=(RangeEqual const&) = default;
		RangeEqual(RangeEqual&&) = default;
		RangeEqual& operator=(RangeEqual&&) = default;


		test_result_t operator()() {
			using std::begin; using std::end;
			auto iter_1 = begin(range_);
			auto iter_2 = begin(expect_);
			auto last_1 = end(range_);
			auto last_2 = end(expect_);
			for (; iter_1 != last_1 && iter_2 != last_2; ++iter_1, ++iter_2) {
				if (*iter_1 != *iter_2) return info();
			}
			return !(iter_1 != last_1 || iter_2 != last_2)
				? test_result_t{ test_status::passed }
				: test_result_t{ infomation::RangeEqualInfo<Range>::invoke(range_, expect_) };
		}

		static std::string label() { return "range equal"; }
	};

	template < class Expr, class Except >
	class ExactThrow
		: private detail_::test_method_tag
		, public enable_labeled<ExactThrow<Expr, Except>>
	{
		Expr expr_;
		std::string msg_{ "Info> Assertion failure: " };
	public:
		ExactThrow(Expr expr)
			: expr_{ expr }
		{}

		test_result_t operator()() try {
			expr_();      
			return msg_ + "no exception had thworn.";
		}
		catch (const Except&) {
			return test_status::passed;
		}
		catch (const std::exception& e) {
			msg_ += "unexpected exception had thrown: see below.\n";
			msg_ += e.what();
			return msg_;
		}
		catch (...) {
			return std::string{ "unhandled exception had thrown\n" };
		}

		static std::string label() { return "exact throw"; }
	};

	namespace infomation {
		using namespace printable_extensons;

		template < class, class = void >
		struct RangeMatchInfo {
			static std::string invoke(std::size_t i, ...) {
				return std::string{ "Info> Assertion failure: in range match with " } +i + "-th element.";
			}
		};

		template < class Range >
		struct RangeMatchInfo<Range,
			::cranberries::void_t<decltype(
				std::declval<std::stringstream&>() << std::declval<typename std::decay_t<Range>::value_type>())>>
		{
			static std::string invoke(std::size_t i, const typename std::decay_t<Range>::value_type& e) {
				using printable_extensons::operator<<;
				std::stringstream ss;
				ss << "Info> Assertion failure: match return false at '" << i << "-th element'[" << e << "].";
				return ss.str();
			}
		};
	}


	template < class Range, class Pred >
	class RangeMatch
		: private detail_::test_method_tag
		, public enable_labeled<RangeMatch<Range, Pred>>
	{
		Range range_;
		Pred pred_;

	public:
		template < class Range_ >
		RangeMatch(Range_&& range, Pred pred)
			: range_{ std::forward<Range_>(range) }
			, pred_{ pred }
		{}


		test_result_t operator()() {
			using std::begin; using std::end;
			test_status exit_status = test_status::passed;
			int nth = 1;
			for (auto&& e : range_) {
				exit_status &= pred_(e);
				if (!exit_status) return infomation::RangeMatchInfo<Range>::invoke(nth, e);
				nth++;
			}
			return exit_status;
		}

		static std::string label() { return "range match"; }
	};

	template < bool B >
	class AssertBool
		: private detail_::test_method_tag
		, public enable_labeled<AssertBool<B>>
	{
		bool expect_;
	public:
		AssertBool(bool a)
			: expect_{ a }
		{}

		test_result_t operator()() {
			if (B ==  expect_) return test_status::passed;
			else return
				std::string{ "Info> Assertion failure: expect " }
				+(B ? std::string{ "true" } : std::string{ "false" })
				+ "but "
				+ (!B ? std::string{ "true" } : std::string{ "false" })
				+ " returned.";
		}

		static std::string label() {
			return std::string{ "assert " } + (B ? std::string{ "true" } : std::string{ "false" });
		}
	};


}

namespace assertion
{
	static test_status test_skip(...) { return test_status::skipped; }

	template < class A, class E, enabler_t<cranberries::is_equality_comparable_to_v<A, E>> = nullptr>
	static auto
		are_equal(A&& actual, E&& expect)
	{
		return detail_::AreEqual<A, E>{ std::forward<A>(actual), std::forward<E>(expect) };
	}


	template < class Range >
	static auto
		range_equal(Range&& range, std::initializer_list<typename std::decay_t<Range>::value_type> expected)
		-> detail_::RangeEqual<Range>
	{
		return { std::forward<Range>(range), expected };
	}

	template < class Expr, class Pred >
	static auto
		range_match(Expr&& expr, Pred&& pred)
		-> detail_::RangeMatch<Expr, Pred>
	{
		return { std::forward<Expr>(expr), std::forward<Pred>(pred) };
	}



	template < class Except, class Expr >
	static detail_::ExactThrow<Expr, Except>
		excact_throw(Expr&& expr)
	{
		return { std::forward<Expr>(expr) };
	}

	template < size_t N, class F, class Require, class... Samples >
	static auto
		permutation_execute(F&& target, Require&& req, Samples&&... samples)
		-> detail_::PermutationExe<N, F, Require, decltype(make_array(std::forward<Samples>(samples)...))>
	{
		return { std::forward<F>(target), std::forward<Require>(req), make_array(std::forward<Samples>(samples)...) };
	}

	static detail_::AssertBool<true>
		is_true(bool actual)
	{
		return { actual };
	}

	static detail_::AssertBool<false>
		is_false(bool actual)
	{
		return { actual };
	}




}

}}
#endif