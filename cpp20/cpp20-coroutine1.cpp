#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <variant>
#include <experimental/coroutine>

// Only works on Visual Studio
// #include <experimental/generator>

using namespace std::experimental::coroutines_v1;

template <class T>
struct co_optional_handle {
  std::optional<T>** m_opt_ptr;
  coroutine_handle<> m_handle;

  co_optional_handle(std::optional<T>** OP, coroutine_handle<> H)
      : m_opt_ptr(OP), m_handle(H) {}

  auto get() {
    std::optional<T> opt;
    *m_opt_ptr = &opt;
    m_handle.resume();
    return opt;
  }
};

template <typename T, typename... Args>
struct std::experimental::coroutine_traits<co_optional_handle<T>, Args...> {
  struct promise_type {
    std::optional<T>* opt;
    co_optional_handle<T> get_return_object() {
      return {&opt, coroutine_handle<promise_type>::from_promise(*this)};
    }
    suspend_always initial_suspend() { return {}; }
    suspend_never final_suspend() { return {}; }
    void set_exception(std::exception_ptr) { opt->reset(); }
    void return_value(T value) { opt->emplace(std::move(value)); }
    auto yield_value(std::optional<T> v) {
      struct suspend_on_empty {
        std::optional<T> m_opt;
        bool await_ready() { return m_opt.has_value(); }
        void await_suspend(coroutine_handle<> H) { H.destroy(); }
        T await_resume() {
          assert(m_opt);
          return *m_opt;
        }
      };
      return suspend_on_empty{std::move(v)};
    }
    void unhandled_exception() {}
  };
};


std::optional<int> five() { return 5; }
std::optional<int> six() { return 6; }
std::optional<int> empty() { return {}; }

co_optional_handle<int> sum() {
  auto a = co_yield five();
  auto b = co_yield six();

  co_return a + b;
}

co_optional_handle<int> sum2() {
  auto a = co_yield five();
  auto b = co_yield empty();

  co_return a + b;
}

#if 0
auto gen_sequence() -> generator_t
{
    co_yield 10;
    co_yield 16;
    co_yield 20;
}
#endif

// Output:
// Opt contains 11
// Opt2 is empty
int main() {


  auto opt = sum().get();
  if (opt) {
    std::cout << "Opt contains " << *opt << "\n";
  } else {
    std::cout << "Opt is empty\n";
  }

  auto opt2 = sum2().get();
  if (opt2) {
    std::cout << "Opt2 contains " << *opt2 << "\n";
  } else {
    std::cout << "Opt2 is empty\n";
  }
}
