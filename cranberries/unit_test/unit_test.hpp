#ifndef CRANBERRIES_UNIT_TEST_HPP
#define CRANBERRIES_UNIT_TEST_HPP
#include <iostream>
#include <vector>
#include <memory>
#include "../type_traits.hpp"
#include "../detection_toolkit.hpp"
#include "../stopwatch.hpp"


namespace cranberries {

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
    std::unique_ptr<T> value_;
    E error_;
  public:
    expected() = delete;
    expected(const T& value) : value_{ std::make_unique<T>(value) }, error_{} {}
    template < class Err, enabler_t<std::is_constructible_v<E,Err>> = nullptr >
    expected(const unexpected<Err>& e) : value_{ nullptr }, error_{ e.error } {}
    expected(const expected& expect)
      : value_{ expect.valid() ? std::make_unique<T>(expect.value()) : nullptr }
      , error_{ expect.error_ } {}
    expected(expected&&) = default;
    expected& operator=(const expected& expect) {
      value_ = expect.valid() ? std::make_unique<T>(expect.value()) : nullptr;
      error_ = expect.error_;
      return *this;
    }
    expected& operator=(expected&&) = default;

    T value() const { return value_ ? *value_ : throw error_; }
    T value_or(const T& v = T{}) const { return value_ ? *value_ : v; }
    E error() const { return error_; }
    bool valid() const { return value_; }

    template < class F >
    expected<std::result_of_t<F(T)>, E> map(F&& f) const {
      if (value_) return std::forward<F>(f)(*value_);
      else return make_unexpected(error_);
    }
    template < class F >
    expected<std::result_of_t<F(T)>, E> map_or(const std::result_of_t<F(T)>& v, F&& f) const {
      return value_ ? std::forward<F>(f)(*value_) : v;
    }
    template < class U >
    expected<std::decay_t<U>, E> and_if(U&& v) const {
      if (value_) return v; else return make_unexpected(error_);
    }
    template < class F >
    decltype(auto) and_then(F&& f) const {
      return value_ ?  f(value_) : make_unexpected(error_);
    }

  };





namespace magic_arts {
  struct holder {
    virtual void run() = 0;
  };

  template<typename F>
  struct holder_impl : public holder {
    holder_impl(F f) : f{f} {}
    virtual void run() override final { f(); }
    F f;
  };
  
  struct test_method
  {
    template<typename F> test_method(F&& f)
      : any_{ std::make_unique<holder_impl<std::decay_t<F>>>(std::forward<F>(f)) }
    {}

    void run() { any_->run(); }

    std::unique_ptr<holder> any_;
  };
}
namespace unit_test_framework{

  struct UnitTestRunTrigger{};
  constexpr UnitTestRunTrigger test_run{};
  
  class unit_test {
    std::string name;
    std::vector<magic_arts::test_method> methods;
  public:
    unit_test(std::string name) : name{name}, methods{} {}
    unit_test() : unit_test{ "Unit Test" } {}
    
    template < class F >
    auto& operator<<(F&& f) {
      methods.emplace_back(std::forward<F>(f));
      return *this;
    }
    
    void operator|(const UnitTestRunTrigger&) {
      this->run();
    }
    void run() {
      auto timer = stopwatch{name.c_str(), true};
      for(auto&& method : methods) method.run();
    }
  };
  
  struct Assert{
    template < class L, class R >
    static constexpr std::enable_if_t<is_equality_comparable_to_v<L,R>>
      AreEqual(L&& _1, R&& _2) {
      if(_1 == _2) 
    }
  };

}}
#endif