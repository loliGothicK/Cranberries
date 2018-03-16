#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_FOPEN_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_FOPEN_HPP
#include <fstream>
#include <iostream>
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

namespace detail_ {
enum class delim { defaulted, specified };
}

template <detail_::delim>
class ReadLine;

template <>
class ReadLine<detail_::delim::specified> : private tag::sentinel_traversal_tag,
                                            public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = std::string;
  using iterator = sentinel_iterator<std::string>;
  using reverse_iterator = sentinel_iterator<std::string>;
  using finite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  ReadLine(std::istream &ifs, char delim) : ifs(ifs), delim{delim} {}
  ReadLine(const ReadLine &) = default;

 private:
  class view_ final : public polymorphic_view<std::string> {
    std::istream &ifs;
    char delim;
    std::string line{};

   public:
    view_(std::istream &ifs, char delim) : ifs(ifs), delim{delim} {
      std::getline(ifs, line, delim);
    }

   protected:
    virtual std::string get() const override final { return line; }
    virtual void next() override final { std::getline(ifs, line, delim); }
    virtual bool is_end() const override final { return ifs.eof(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(ifs, delim);
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<default_rev_view<std::string>>(this->fwd());
  }

 public:
  iterator begin() { return {this->fwd()}; }
  iterator end() { return {}; }
  reverse_iterator rbegin() { return {this->rev()}; }
  reverse_iterator rend() { return {}; }

 private:
  std::istream &ifs;
  char delim;
};

template <>
class ReadLine<detail_::delim::defaulted> : private tag::sentinel_traversal_tag,
                                            public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = std::string;
  using iterator = sentinel_iterator<std::string>;
  using reverse_iterator = sentinel_iterator<std::string>;
  using finite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  ReadLine(std::istream &ifs) : ifs(ifs) {}
  ReadLine(const ReadLine &) = default;

 private:
  class view_ final : public polymorphic_view<std::string> {
    std::istream &ifs;
    std::string line{};

   public:
    view_(std::istream &ifs) : ifs(ifs) { std::getline(ifs, line); }

   protected:
    virtual std::string get() const override final { return line; }
    virtual void next() override final { std::getline(ifs, line); }
    virtual bool is_end() const override final { return ifs.eof(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(ifs);
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<default_rev_view<std::string>>(this->fwd());
  }

 public:
  iterator begin() { return {this->fwd()}; }
  iterator end() { return {}; }
  reverse_iterator rbegin() { return {this->rev()}; }
  reverse_iterator rend() { return {}; }

 private:
  std::istream &ifs;
};

namespace create {
ReadLine<detail_::delim::specified> read_line(std::istream &ifs, char delim) {
  return {ifs, delim};
}
ReadLine<detail_::delim::defaulted> read_line(std::istream &ifs) {
  return {ifs};
}
}  // namespace create

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
