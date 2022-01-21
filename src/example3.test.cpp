#include "function_pipes.hpp"
#include <iostream>
#include <memory>
#include <random>
#include <tuple>
#include <utility>

using namespace mtx;

struct resource {
  int a;
  bool b;
};

int main() {
  auto pointer = std::make_unique<resource>(resource{4, false});
  auto generator = std::mt19937(std::random_device()());
  auto dist = std::uniform_int_distribution<>(1, 6);
  auto random_ints = pipe([&]() { return dist(generator); });

  auto uncopiable = pipe([a = std::move(pointer)]() -> bool { return a->b; });

  auto composition = std::forward<decltype(uncopiable)>(uncopiable) &
                     pipe([](bool a) { return !a; });

  auto uncopiable_tuple =
      unpack(std::make_tuple(std::make_unique<resource>(resource{8, true})),
             pipe([](std::unique_ptr<resource> &a) { return a->a; }));
  
  auto lazy_eval =
      random_ints | pipe([](int a) { return a + 5; }) | pipe([](int a) {
        if (a < 8)
          return "foo";
        else
          return "bar";
      });
  for (int _ = 5; _ != 0; _--)
    std::cout << lazy_eval() << '\n';
  return 0;
}