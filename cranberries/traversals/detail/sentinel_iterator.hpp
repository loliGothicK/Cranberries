#ifndef CRANBERRIES_RANGES_SENTINEL_ITERATOR_HPP
#define CRANBERRIES_RANGES_SENTINEL_ITERATOR_HPP
#include <memory>
#include <type_traits>
#include <utility>
#include "../../common/concepts.hpp"
#include "../../meta_bind.hpp"
#include "../../utility/utility.hpp"
#include "tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

namespace _detail {

struct _inf {
  using infinite_ = std::nullptr_t;
};
struct _fin {
  using finite_ = std::nullptr_t;
};

template <class, class = void>
struct is_inf : std::false_type {};

template <class T>
struct is_inf<T, cranberries::void_t<typename remove_cvr_t<T>::infinite_>>
    : std::true_type {};

template <class... Ts>
struct disjunction_bound
    : std::conditional_t<disjunction_v<is_inf<Ts>...>, _inf, _fin> {};

template <class... Ts>
struct conjunction_bound
    : std::conditional_t<conjunction_v<negation<is_inf<Ts>>...>, _inf, _fin> {};

struct _immut {
  using immutable_ = std::nullptr_t;
};

struct _mut {
  using mutable_ = std::nullptr_t;
};

template <class, class = void>
struct is_immut : std::false_type {};

template <class T>
struct is_immut<T, cranberries::void_t<typename remove_cvr_t<T>::immutable_>>
    : std::true_type {};

template <class... Ts>
struct mut_or
    : std::conditional_t<disjunction_v<is_immut<Ts>...>, _immut, _mut> {};

template <class, class, class = void>
struct size_impl_injection {};

template <class Derived, class Trav>
struct size_impl_injection<
    Derived, Trav,
    cranberries::void_t<decltype(std::declval<const Trav &>().size())>> {
  constexpr size_t size() const {
    return static_cast<const Derived *>(this)->traversal.size();
  }
};
}  // namespace _detail

template <class T>
struct polymorphic_view {
  virtual ~polymorphic_view() = default;
  virtual T get() const = 0;
  virtual void next() = 0;
  virtual bool is_end() const = 0;
};

class forward_traversal {};

class reversable_traversal : forward_traversal {};

struct view_get {
  template <class T>
  static std::unique_ptr<polymorphic_view<typename remove_cvr_t<T>::value_type>>
  fwd(T &&trav) {
    return trav.fwd();
  }
  template <class T>
  static std::unique_ptr<polymorphic_view<typename remove_cvr_t<T>::value_type>>
  rev(T &&trav) {
    return trav.rev();
  }
};

template <class T>
class default_rev_view final : public polymorphic_view<T> {
  std::vector<T> data_;
  int index_;

 public:
  template <class View>
  default_rev_view(std::unique_ptr<View> view) : data_{} {
    for (; !view->is_end(); view->next()) data_.emplace_back(view->get());
    index_ = data_.size() - 1;
  }

 protected:
  T get() const override final { return data_[index_]; }
  void next() override final { index_--; }
  bool is_end() const override final { return index_ < 0; }
};

enum class sentinel_flag { on, off };

template <class T>
class sentinel_iterator {
 protected:
  // copy construct/copy asignment implementation
  sentinel_iterator deep_copy(const sentinel_iterator &iter) & {
    this->swap(sentinel_iterator{iter});
    return *this;
  }

 public:
  // For std::iterator_traits<sentinel_iterator>
  using value_type = T;
  using difference_type = int;
  using pointer = std::nullptr_t;  // disable
  using reference = value_type &;
  using iterator_category = std::output_iterator_tag;

  // Constructor for Sentinel Range
  sentinel_iterator(std::unique_ptr<polymorphic_view<T>> ptr)
      : sentinel{std::move(ptr)}, is_sentinel{sentinel_flag::off} {}

  // Default Constructor for End Sentinel
  sentinel_iterator() : sentinel{nullptr}, is_sentinel{sentinel_flag::on} {};

  // Copy constructor/Copy assignment operator
  /*[Note: Copy constructor makes deep-copied sentinel_iterator.
          It meens copying current state of sentinel.
          - end note ]*/
  sentinel_iterator(const sentinel_iterator &iter)
      : sentinel{std::make_unique<polymorphic_view<T>>(iter.sentinel)} {}

  sentinel_iterator &operator=(const sentinel_iterator &rhs) {
    return deep_copy(rhs);
  }

  // Move constructor/Move assignment operator
  sentinel_iterator &operator=(sentinel_iterator &&) = default;
  sentinel_iterator(sentinel_iterator &&) = default;

  // Destructor
  ~sentinel_iterator() = default;

  // For iterator requirements
  void swap(sentinel_iterator &iter) & { std::swap(sentinel, iter.sentinel); }
  void swap(sentinel_iterator &&iter) & { std::swap(sentinel, iter.sentinel); }

  // Dereferene
  decltype(auto) get() const { return sentinel->get(); }
  decltype(auto) operator*() const { return sentinel->get(); }

  // Increment
  bool next() { return sentinel->next(); }
  sentinel_iterator &operator++() { return sentinel->next(), *this; }

  // Sentinel invoke
  bool is_end() const { return sentinel->is_end(); }

  bool operator==(const sentinel_iterator &iter) const {
    return is_sentinel == sentinel_flag::off
               ? iter.is_sentinel == sentinel_flag::off ? false : this->is_end()
               : iter.is_end();
  }
  bool operator!=(const sentinel_iterator &iter) const {
    return is_sentinel == sentinel_flag::off
               ? iter.is_sentinel == sentinel_flag::off ? true : !this->is_end()
               : !iter.is_end();
  }

 private:
  // Iteration state
  /*[Note: This is implementation of Sentinel Iterator.
          It manages iteration, access and end-point checking.
  -end note]*/
  std::unique_ptr<polymorphic_view<T>> sentinel;

  sentinel_flag is_sentinel;
};

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif