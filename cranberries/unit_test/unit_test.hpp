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
#include "../type_traits.hpp"
#include "../utility/utility.hpp"
#include "../experimental/ranges/view_adaptors/zip_with.hpp"
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


namespace unit_test_framework {
namespace detail_ {
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

  test_status& operator &= (test_status& a, test_status b) {
    return a = a&b;
  }

  test_status& operator &= (test_status& a, bool b) {
    return a = a&static_cast<test_status>(b);
  }

  bool operator ! (test_status s) {
    return s == test_status::failed ? true : false;
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
  template < class F >
  class TestMethod {
    F f_;
    std::string label_;

    std::chrono::duration<double, std::milli> elapsed_;
    test_result_t result_;
  public:
    TestMethod(F f) : f_{ std::move(f) }, label_{} {}
    TestMethod(F f, std::string _) : f_{ std::move(f) }, label_{ _ } {}

    decltype(auto) index(size_t index) {
      label_ = std::string{ "#" } + std::to_string(index) + ": " + label_;
      return *this;
    }
    void exe() {
      auto first = std::chrono::high_resolution_clock::now();
      result_ = f_();
      auto last = std::chrono::high_resolution_clock::now();
      elapsed_ = last - first;
    }
    void print(std::ostream& logger) {
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




  template < class F, enabler_t<!std::is_base_of<detail_::test_method_tag, std::decay_t<F>>::value> = nullptr >
  TestMethod<F> make_test_method(F&& f) {
    return { std::forward<F>(f), "test method" };
  }

  template < class F, enabler_t<std::is_base_of<detail_::test_method_tag, std::decay_t<F>>::value> =nullptr >
  TestMethod<F> make_test_method(F&& f) {
    return { std::forward<F>(f), std::decay_t<F>::label() };
  }


  template < class F >
  TestMethod<F> make_test_method(F&& f, std::string label) {
    return { std::forward<F>(f), label };
  }
}

  
  class UnitTestContainer {
    // members
    std::ostream& logger; // for output
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
    UnitTestContainer(std::ostream& os = std::cout, std::string name = "Unit Test")
      : logger{ os }
      , name{ name }
    {
      logger << "\n" << name << " Start...\n";
    }
    // constructor [ label, logger ]
    UnitTestContainer(std::string name = "Unit Test", std::ostream& os = std::cout)
      : UnitTestContainer{ os, name }
    {}


    // destructor
    // Collect and output test summary when test container destruct
    ~UnitTestContainer() {
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
        logger << "Test Status: Grean.\n";
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
        : (logger << total_.count() / 1000 << " [sec]")) << "\n" << std::endl;
    }

    
    // operator for pushing test method
    // Test method is executed in parallel as soon as pushing
    template < class F, enabler_t<is_callable_v<F(), test_result_t>> = nullptr >
    auto& push(F&& f) {
      std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

      methods.emplace_back(std::async(std::launch::async, // !explicit specified async
        [&, f = detail_::make_test_method(std::move(f)).index(++index)]() mutable -> test_status {
          f.exe();
          auto_lock(mtx_), f.print(logger);
          return f.status();
      }));
      return *this;
    }

    // operator for pushing test method with test method label
    template < class F, enabler_t<is_callable_v<F(), test_result_t>> = nullptr >
    auto& push(F&& f, std::string label) {
      std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

      methods.emplace_back(std::async(std::launch::async, // !explicit specified async
        [&, f = make_test_method(std::move(f), label).index(++index)]() mutable->test_status {
        f.exe();
        auto_lock(mtx_), f.print(logger);
        return f.status();
      }));
      return *this;
    }

    
  };

namespace detail_ {

  template < class Expr, class Generator >
  class ScrambleExe
    : private detail_::test_method_tag
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

  template < class Expr, class Expected >
  class AreEqual
    : private detail_::test_method_tag
  {
    Expr expr_;
    Expected expect_;
    std::result_of_t<Expr()> result_;
  public:
    AreEqual(Expr expr, Expected expect)
      : expr_{ expr }
      , expect_{ expect }
      , result_{}
    {}

    test_result_t operator()() {
      if((result_ = expr_()) == expect_) return test_status::passed;
      else return info();
    }

    std::string info() {
      return std::string{ "Info> Assertion failure: " }
        + "'" + std::to_string(expect_) + "'"
        + " expected but "
        + "'" + std::to_string(result_) + "'"
        + " actual.";
    }

    static std::string label() { return "are equal"; }
  };

  template < class Expr, class Expected >
  class RangeEqual
    : private detail_::test_method_tag
  {
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


    test_result_t operator()() {
      using std::begin; using std::end;
      test_status exit_status = test_status::passed;
      result_ = expr_();
      if (cranberries::size(result_) != cranberries::size(expect_)) return info();
      for (auto&& e : cranberries::experimental::ranges::view::make_zipped(result_, expect_)) {
        exit_status &= std::get<0>(e) == std::get<1>(e);
      }
      if(!exit_status) return info();
      else return exit_status;
    }
    std::string info() {
      return std::string{ "Info> Assertion failure: " }
        +"'" + range2string(expect_) + "'"
        + " expected but "
        + "'" + range2string(result_) + "'"
        + " actual.";
    }

    static std::string label() { return "range equal"; }
  };

  template < class Expr, class Except >
  class ExactThrow
    : private detail_::test_method_tag
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

}

namespace assert
{
  test_status test_skip(...) { return test_status::skipped; }

  template < class Expr, class Expected >
  static detail_::AreEqual<Expr, Expected>
    are_equal(Expr&& expr, Expected expect)
  {
    return { std::forward<Expr>(expr), expect };
  }


  template < class Expr, class... Expected >
  static auto
    range_equal(Expr&& expr, const Expected&... expect)
    -> detail_::RangeEqual<Expr, decltype(make_array(std::declval<const Expected&>()...))>
  {
    return { std::forward<Expr>(expr), make_array(expect...) };
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


}

}}
#endif