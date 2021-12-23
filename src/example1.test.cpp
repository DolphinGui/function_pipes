#include "function_pipes.hpp"
#include <tuple>
#include <utility>

using namespace mtx;

int main() {

  // an example of single parameter function composition
  int a = 3 | pipe([](int a) { return a + 2; }) |
          pipe([](int a) { return a * 4; }) | pipe([](int a) { return a - 2; });

  if (a != 18)
    return 1;

  // an example of multiple parameter function composition
  int b = std::make_tuple(4 | pipe([](int a) { return a * 2; }), 2) |
          pipe([](int a, int b) { return a - b; });
  if (b != 6)
    return 2;
  // an example of pipe composition. see example 2 for an edge case
  // involving this.
  auto composed = pipe([](int a) { return a; }) | pipe([](int a) { return a; });

  if ((0 | composed) != 0)
    return 3;

  // an example of unpacking tuples into multiple pipes
  auto c = unpack(std::make_tuple(2, 4, 5), pipe([](int a) { return a / 2; }),
                  pipe([](int a) { return a + 4; }),
                  pipe([](int a) { return a * 2; }));

  if (c != std::make_tuple(1, 8, 10))
    return 4;

  // manually specifying the function via integer_sequence
  // not really recommended.
  auto d = unpack(std::integer_sequence<int, 0, 1, 2>{},
                  std::integer_sequence<int, 0, 1, 0>{},
                  std::make_tuple(2, 4, 6), pipe([](int a) { return a / 2; }),
                  pipe([](int a) { return a * 2; }));

  if (d != std::make_tuple(1, 8, 3))
    return 5;

  // using unpack() in pipe format. Just for fun.
  auto e =
      std::make_tuple(5, 2, 1) | pipe([](auto a, auto b, auto c) {
        return unpack(std::make_tuple(a, b, c), pipe([](int a) { return a; }),
                      pipe([](int a) { return a * 2; }),
                      pipe([](int a) { return a; }));
      });

  if (e != std::make_tuple(5, 4, 1))
    return 6;

  return 0;
}