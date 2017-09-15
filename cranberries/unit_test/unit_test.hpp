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



  template < class F >
  class TestMethod {
    F f_;
    std::string label_;
    std::chrono::duration<double, std::milli> elapsed_;
    test_status status_;
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
    void exe() {
      auto first = std::chrono::high_resolution_clock::now();
      status_ = static_cast<test_status>(f_());
      auto last = std::chrono::high_resolution_clock::now();
      elapsed_ = last - first;
    }
    void print(std::ostream& logger) {
      switch (status_)
      {
      case test_status::failed:
        ConsoleTestColor::Red();
        logger << label_ << ": => fail! " << elapsed_.count() << "[ms]\n" << f_.info() << "\n";
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

    auto status() { return status_; }
  };

  template < class F >
  TestMethod<F> make_test_method(F&& f) {
    return { std::forward<F>(f) };
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
    template < class F >
    auto& operator<<(TestMethod<F> f) {
      std::call_once(once, [&] {start = std::chrono::high_resolution_clock::now(); });

      methods.emplace_back(std::async(std::launch::async, // !explicit specified async
        [&, f = std::move(f.index(++index))]() mutable->test_status {
          f.exe();
          std::lock_guard<std::mutex> lock(mtx_);
          f.print(logger);
          return f.status();
      }));
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
        return std::string{ "Info> Not satisfied requirements." };
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
        return std::string{ "Info> Assertion failure: " }
          + "'" + std::to_string(expect_) + "'"
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
        return std::string{ "Info> Assertion failure: " }
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
        return std::string{ "Info> Assertion failure: " } + msg_;
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