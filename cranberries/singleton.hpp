#ifndef CRANBERRIES_SINGLETON_HPP
#define CRANBERRIES_SINGLETON_HPP
#include <cassert>
#include <mutex>
namespace cranberries {

class SingletonFinalizer {
 public:
  typedef void (*FinalizerFunc)();

  static void addFinalizer(FinalizerFunc func);
  static void finalize();
};

template <typename T>
class singleton final {
 public:
  static T &get_instance() {
    std::call_once(initFlag, create);
    assert(instance);
    return *instance;
  }

 private:
  static void create() {
    instance = new T;
    SingletonFinalizer::addFinalizer(&singleton<T>::destroy);
  }

  static void destroy() {
    delete instance;
    instance = nullptr;
  }

  static std::once_flag initFlag;
  static T *instance;
};

template <typename T>
std::once_flag singleton<T>::initFlag;
template <typename T>
T *singleton<T>::instance = nullptr;

constexpr size_t CranberriesMaxFinalizersSize = 256;
static size_t CranberriesFinalizersCounter = 0;
static SingletonFinalizer::FinalizerFunc
    CranberriesFinalizers[CranberriesMaxFinalizersSize];

void SingletonFinalizer::addFinalizer(FinalizerFunc func) {
  assert(CranberriesFinalizersCounter < CranberriesMaxFinalizersSize);
  CranberriesFinalizers[CranberriesFinalizersCounter++] = func;
}

void SingletonFinalizer::finalize() {
  for (int i = CranberriesFinalizersCounter - 1; i >= 0; --i) {
    (*CranberriesFinalizers[i])();
  }
  CranberriesFinalizersCounter = 0;
}
}  // namespace cranberries
#endif