#include "function_pipes.hpp"
#include <tuple>

using namespace mtx;

int main() {

  // an example of single parameter function composition
  int a = 3 | pipe([](int a) { return a + 2; }) |
          pipe([](int a) { return a * 4; }) | pipe([](int a) { return a - 2; });
  
  // an example of multiple parameter function composition
  int b = std::make_tuple(4 | pipe([](int a){ return a * 2;}), 2)
  | pipe([](int a, int b) {return a - b;});
  
  // an example of pipe composition. see example 2 for an edge case
  // involving this.
  auto composed = pipe([](int a){ return a; }) | pipe([](int a){ return a; });

  // an example of unpacking tuples into multiple pipes
  auto c = unpack(std::make_tuple(2, 4, 5), 
  pipe([](int a) { return a / 2; }),
  pipe([](int a) { return a + 4; }), 
  pipe([](int a) { return a * 2; }));
  return 0;
}