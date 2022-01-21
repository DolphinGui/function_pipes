#include "function_pipes.hpp"
#include <tuple>
#include <utility>

using namespace mtx;

auto composeCopy() {
  int x = 2, y = 5;
  auto pipe1 = pipe([=](int a) { return a * x; });
  auto pipe2 = pipe([=](int a) { return a - y; });
  return pipe1 | pipe2;
}

auto composeReference() {
  int x = 2, y = 5;
  auto pipe1 = pipe([=](int a) { return a * x; });
  auto pipe2 = pipe([=](int a) { return a - y; });
  return std::forward<decltype(pipe1)>(pipe1) &
         std::forward<decltype(pipe2)>(pipe2);
}

auto composeMove() {
  int x = 2, y = 5;
  auto pipe1 = pipe([=](int a) { return a * x; });
  auto pipe2 = pipe([=](int a) { return a - y; });
  return std::forward<decltype(pipe1)>(pipe1) ^
         std::forward<decltype(pipe2)>(pipe2);
}

int main() {
  // very subtle undefined behavior
  // & will capture the lambdas by reference,
  // but the lambdas will technically expire.
  int a = 2 | composeReference();

  int b = 2 | composeCopy();

  return 0;
}