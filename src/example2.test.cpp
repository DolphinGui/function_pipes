#include "function_pipes.hpp"
#include <tuple>

using namespace mtx;

auto composeWrong() {
  int x = 2, y = 5;
  auto pipe1 = pipe([&](int a) { return a * x; });
  auto pipe2 = pipe([&](int a) { return a - y; });
  return pipe1 | pipe2;
}

auto composeRight() {
  int x = 2, y = 5;
  auto pipe1 = pipe([&](int a) { return a * x; });
  auto pipe2 = pipe([&](int a) { return a - y; });
  return pipe1 & pipe2;
}

int main() {
  // very subtle undefined behavior
  // spot the UB. See answer.md for the answer.
  int a = 2 | composeWrong();

  int b = 2 | composeRight();

  return 0;
}