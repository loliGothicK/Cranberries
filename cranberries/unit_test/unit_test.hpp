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
#include "../algorithm.hpp"
#include "../optinal.hpp"
#include "../type_traits.hpp"
#include "../utility/utility.hpp"
#include "../detection_toolkit.hpp"
#ifdef _WIN32
#include <Windows.h>
#endif

namespace cranberries {

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

  template < class T, class E > class expected;

  template < class E >
  struct unexpected {
    E error;
    template < class T >
    operator expected<T, E>() { return { *this }; }
  };

  template < class E >
  unexpected<std::decay_t<E>> make_unexpected(E&& e) { return { std::forward<E>(e) }; }

  template < class T, class E = std::string >
  class expected {
    optional<T> opt_;
    E error_;
  public:
    expected() = default;
    expected(const T& value) : opt_{ value }, error_{} {}
    template < class Err, enabler_t<std::is_constructible<E,Err>::value> = nullptr >
    expected(const unexpected<Err>& e) : opt_{ nullopt }, error_{ e.error } {}
    expected(const expected&) = default;
    expected(expected&&) = default;
    expected& operator=(const expected&) = default;
    expected& operator=(expected&&) = default;

    T value() const { return opt_ ? *opt_ : throw error_; }
    T value_or(const T& v = T{}) const { return opt_.value_or(v); }
    E error() const { return error_; }
    bool valid() const { return opt_; }

    template < class F >
    expected<std::result_of_t<F(T)>, E> map(F&& f) const {
      if (opt_) return std::forward<F>(f)(*opt_);
      else return make_unexpected(error_);
    }
    template < class F >
    expected<std::result_of_t<F(T)>, E> map_or(const std::result_of_t<F(T)>& v, F&& f) const {
      return opt_ ? std::forward<F>(f)(*opt_) : v;
    }
    template < class F, class G >
    expected<std::common_type_t<std::result_of_t<F(E)>, std::result_of_t<G(T)>>, E>
    map_or_else(F&& f, G&& g) const {
      if (opt_) return std::forward<G>(g)(*opt_);
      else return std::forward<F>(f)(error_);
    }
    template < class U >
    expected<std::decay_t<U>, E> and_if(U&& v) const {
      if (opt_) return v; else return make_unexpected(error_);
    }
    template < class F >
    decltype(auto) then(F&& f) const {
      return std::forward<F>(f)(*this);
    }

  };

namespace unit_test_framework {

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

  test_status& operator &= (test_status& a, test_status b) {
    return a = a&b;
  }

  bool operator | (bool b, test_status s) {
    return b = (s == test_status::failed ? false : true);
  }


  class TestResult {
    std::string label_;
    double time_;
    expected<test_status, std::string> result_;
  public:
    TestResult() = default;
    TestResult(std::string label, double time, expected<test_status, std::string> result)
      : label_{ label }
      , time_{ time }
      , result_{ result }
    {}
    auto time() { return time_; }
    test_status log(std::ostream& os) const {
      return result_.map_or_else(
        [&](std::string error) {
          ConsoleTestColor::Red();
          os << label_ << ": ... fail! " << time_ << "[ms]\n" << error << "\n";
          ConsoleTestColor::Reset();
          return test_status::failed;
        },
        [&] (test_status status){
          if (status == test_status::passed) {
            ConsoleTestColor::Green();
            os << label_ << ": ... passed. " << time_ << "[ms]\n";
            ConsoleTestColor::Reset();
            return test_status::passed;
          }
          else
          {
            ConsoleTestColor::Yellow();
            os << label_ << ": ... skipped. " << time_ << "[ms]\n";
            ConsoleTestColor::Reset();
            return test_status::skipped;
          }
        }
      ).value();
    }

  };

  template < class F >
  class TestMethod {
    F f_;
    std::string label_;
  public:
    TestMethod(F f) : f_{ std::move(f) }, label_{} {}

    decltype(auto) label(std::string name) {
      label_ = name;
      return *this;
    }
    decltype(auto) index(size_t index) {
      label_ = std::string{"#"} + std::to_string(index) + label_;
      return *this;
    }
    TestResult operator()() {
      auto first = std::chrono::high_resolution_clock::now();
      test_status status = static_cast<test_status>(f_());
      auto last = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = last - first;
      return {
        label_,
        elapsed.count(),
        [&]() ->expected<test_status, std::string> {
        if (static_cast<bool>(status)) return status;
        else return make_unexpected(f_.info());
      }() };
    }
  };

  template < class F >
  TestMethod<F> make_test_method(F&& f) {
    return { std::forward<F>(f) };
  }

  
  class unit_test {
    std::ostream& logger;
    std::string name;
    decltype(std::chrono::high_resolution_clock::now()) start;

    std::once_flag once;
    std::vector<std::future<TestResult>> methods{};
    size_t index{};
    size_t passed{};
    size_t failed{};
    size_t skipped{};
  public:
    unit_test(std::ostream& os = std::cout, std::string name = "Unit Test")
      : logger{ os }
      , name{ name }
    {
      ConsoleTestColor::Blue();
      logger << name << " Start...\n";
      ConsoleTestColor::Reset();
    }

    unit_test(std::string name = "Unit Test", std::ostream& os = std::cout)
      : unit_test{ os, name }
    {}


    ~unit_test() {

      for (auto&& method : methods) {
        auto result_ = method.get();
        test_status status = result_.log(logger);
        switch (status)
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
      std::chrono::duration<double, std::milli> total_ = std::chrono::high_resolution_clock::now() - start;
      if(failed == 0){
        ConsoleTestColor::Green();
        logger << "Test Status: Grean.\n";
      }
      else if (failed == 1){
        ConsoleTestColor::Red();
        logger << "Test Status: Red: 1 error generated.\n";
      }
      else{
        ConsoleTestColor::Red();
        logger << "Test Status: Red: " << failed << " errors generated.\n";
      }
      ConsoleTestColor::Blue();
      logger << "Total Tests: " << index 
             << " | Passed: "   << passed
             << " | Failed: "   << failed
             << " | Skipped: "  << skipped << std::endl;
        [&](auto&& os) {
        (total_.count() < 1000
          ? (os << total_.count() << " [ms]")
          : (os << total_.count() /1000 << " [sec]")
          )<< std::endl;
      }(logger << name << " End : Total Tests Execute Time = " << std::fixed << std::setprecision(5));
      
      ConsoleTestColor::Reset();
    }

    
    template < class F >
    auto& operator<<(TestMethod<F> f) {
      std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });
      methods.emplace_back(std::async(std::launch::async, f.index(++index)));
      return *this;
    }
    
  };

  namespace test_method_detail {

    template < class Expr, class Generator >
    class ScrambleExe {
      Expr expr_;
      Generator generator_;
      size_t times_;
    public:
      ScrambleExe(Expr expr, Generator gen, size_t n)
        : expr_{ expr }
        , generator_{ gen }
        , times_{ n }
      {}

      bool operator()() {
        for (size_t i{}; i < times_; ++i)
          cranberries::apply(expr_, generator_());
        return true;
      }
    };

    template < size_t N, class Expr, class Require, class Tuple >
    class PermutationExe {
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
      test_status execute(std::index_sequence<I...>) {
        std::vector<size_t> indices(std::tuple_size<std::decay_t<Tuple>>::value);
        std::iota(std::begin(indices), std::end(indices), 0);
        test_status exit_status = test_status::passed;
        do
        {
          exit_status &= static_cast<test_status>(require_(expr_(sample_[indices[I]]...)));
        } while (cranberries::next_partial_permutation(indices.begin(), indices.begin() + N, indices.end()));
        return exit_status;
      }

      test_status operator()() {
        return execute(std::make_index_sequence<N>{});
      }

      std::string info() {
        return std::string{ "Not satisfied requirements." };
      }
    };

    template < size_t N, class Expr, class Sample >
    class CombinationExe {
      Expr expr_;
      Sample sample_;
    public:
      CombinationExe(Expr expr, Sample t)
        : expr_{ expr }
        , sample_( t )
      {}

      template < size_t... I >
      void execute(std::index_sequence<I...>) {
        std::vector<size_t> indices(std::tuple_size<std::decay_t<Sample>>::value);
        std::iota(std::begin(indices), std::end(indices), 0);
        do
        {
          expr_(sample_[indices[I]]...);
        } while (cranberries::next_combination(indices.begin(), indices.begin() + N, indices.end()));
      }

      bool operator()() {
        execute(std::make_index_sequence<N>{});
        return true;
      }
    };

    template < class Expr, class Expected >
    class AreEqual {
      Expr expr_;
      Expected expect_;
      std::result_of_t<Expr()> result_;
    public:
      AreEqual(Expr expr, Expected expect)
        : expr_{ expr }
        , expect_{ expect }
        , result_{}
      {}

      bool operator()() {
        return (result_ = expr_()) == expect_;
      }
      std::string info() {
        return std::string{ "=> Assertion failure: " }
          +"'" + std::to_string(expect_) + "'"
          + " expected but "
          + "'" + std::to_string(result_) + "'"
          + " actual.";
      }
    };

    template < class Expr, class Expected >
    class RangeEqual {
      Expr expr_;
      Expected expect_;
      std::result_of_t<Expr()> result_;

      template < class Range >
      static std::string range2string(Range&& range) {
        using std::to_string;
        std::string str{ "[" };
        using std::begin; using std::end;
        auto iter = begin(range);
        str += to_string(*iter);
        ++iter;
        for (; iter != end(range); ++iter) str += std::string(",") += to_string(*iter);
        return str + "]";
      }

    public:
      RangeEqual(Expr expr, Expected expect)
        : expr_{ expr }
        , expect_( expect )
        , result_{}
      {}


      bool operator()() {
        using std::begin; using std::end;
        bool exit_status = true;
        result_ = expr_();
        if (cranberries::size(result_) != cranberries::size(expect_)) return false;
        for (auto&& e : cranberries::zip(result_, expect_)) {
          exit_status &= std::get<0>(e) == std::get<1>(e);
        }
        return exit_status;
      }
      std::string info() {
        return std::string{ "=> Assertion failure: " }
          +"'" + range2string(expect_) + "'"
          + " expected but "
          + "'" + range2string(result_) + "'"
          + " actual.";
      }
    };

    template < class Expr, class Except >
    class ExactThrow {
      Expr expr_;
      std::string msg_;
    public:
      ExactThrow(Expr expr)
        : expr_{ expr }
      {}

      bool operator()() try {
        expr_();
        msg_ = "no exception thworn.";
        return false;
      }
      catch (const Except& e) {
        return true;
      }
      catch (const std::exception& e) {
        msg_ = "unhandled exception thrown: see below.\n";
        msg_ += e.what();
        return false;
      }
      std::string info() {
        return std::string{ "=> Assertion failure: " } +msg_;
      }
    };

  }

  namespace assert
  {
    test_status test_skip(...) { return test_status::skipped; }

    template < class Expr, class Expected >
    static TestMethod<test_method_detail::AreEqual<Expr, Expected>>
      are_equal(Expr&& expr, Expected expect)
    {
      return {{ std::forward<Expr>(expr), expect }};
    }


    template < class Expr, class... Expected >
    static auto
      range_equal(Expr&& expr, const Expected&... expect)
      -> TestMethod<test_method_detail::RangeEqual<Expr, decltype(make_array(std::declval<const Expected&>()...))>>
    {
      return {{ std::forward<Expr>(expr), make_array(expect...) }};
    }


    template < class Except, class Expr >
    static TestMethod<test_method_detail::ExactThrow<Expr, Except>>
      excact_throw(Expr&& expr)
    {
      return {{ std::forward<Expr>(expr) }};
    }

    template < size_t N, class F, class Require, class... Samples >
    static auto
      permutation_execute(F&& target, Require&& req, Samples&&... samples)
      -> TestMethod<test_method_detail::PermutationExe<N, F, Require, decltype(make_array(std::forward<Samples>(samples)...))>>
    {
      return {{ std::forward<F>(target), std::forward<Require>(req), make_array(std::forward<Samples>(samples)...) }};
    }


  }

}}
#endif