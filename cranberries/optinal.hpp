#ifndef CRANBERRIES_OPTINAL_HPP
#define CRANBERRIES_OPTINAL_HPP
#include <type_traits>

namespace cranberries {

class nullopt_t {};
constexpr nullopt_t nullopt{};

template <typename T>
class optional {
  bool init_;
  T storage_;

public:
  constexpr optional(const T& value) : init_{ true }, storage_{ value } {}
  constexpr optional(nullopt_t) : init_{} {}
  constexpr optional() = default;

  constexpr optional(const optional&) = default;
  constexpr optional(optional&&) = default;
  constexpr optional& operator=(const optional&) = default;
  constexpr optional& operator=(optional&&) = default;
  ~optional() = default;

  constexpr optional& operator=(const T& value) {
    init_ = true;
    storage_ = value;
    return *this;
  };

  constexpr optional& operator=(nullopt_t) {
    init_ = false;
    return *this;
  };


  constexpr const T& operator*() const {
    return storage_;
  }
  constexpr operator bool() const {
    return init_;
  }
  constexpr const T& value_or(const T& or_) const {
    return init_ ? storage_ : or_;
  }
};

template <typename T>
class optional<T&> {
  bool init_;
  T& storage_;

public:
  constexpr optional(T& value) : init_{ true }, storage_{ value } {}
  constexpr optional(nullopt_t) : init_{} {}
  constexpr optional() = default;

  constexpr optional(const optional&) = default;
  constexpr optional(optional&&) = default;
  constexpr optional& operator=(const optional&) = default;
  constexpr optional& operator=(optional&&) = default;
  ~optional() = default;

  constexpr optional& operator=(const T& value) {
    init_ = true;
    storage_ = value;
    return *this;
  };

  constexpr optional& operator=(nullopt_t) {
    init_ = false;
    return *this;
  };


  constexpr T& operator*() const {
    return storage_;
  }
  constexpr operator bool() const {
    return init_;
  }
  constexpr T value_or(const T& or_) const {
    return init_ ? storage_ : or_;
  }
};

}

#endif