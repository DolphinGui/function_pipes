#pragma once
#include <tuple>
#include <utility>

namespace mtx {

namespace internal {}

template <typename inner> class pipe : public inner {
public:
  constexpr pipe(inner inside) : inner{inside} {};
};

template <typename F, typename... T>
constexpr decltype(auto) operator | (std::tuple<T...> params, pipe<F> function) {
  return std::apply(function, params);
}
template <typename F, typename T>
constexpr decltype(auto) operator | (T param, pipe<F> function) {
  return function(param);
}

template <typename F1, typename F2>
constexpr decltype(auto) operator | (pipe<F1> function1, pipe<F2> function2) {
  return pipe([&](auto... args){ return function2(function1(args...));});
}

template <typename F1, typename F2>
constexpr decltype(auto) operator & (pipe<F1> function1, pipe<F2> function2) {
  return pipe([=](auto... args){ return function2(function1(args...));});
}

template <typename Int, Int... ints, typename... tuple_types,
          typename... pipe_types>
decltype(auto) unpack(std::integer_sequence<Int, ints...> int_seq,
            std::tuple<tuple_types...> tuple, pipe_types... pipes) {
  static_assert(sizeof...(tuple_types) == sizeof...(pipe_types),
                "number of functions does not match tuple members");
  return std::make_tuple((pipes(std::get<ints>(tuple)))...);
}
template <typename... tuple_types, typename... pipe_types>
decltype(auto) unpack(std::tuple<tuple_types...> tuple, pipe_types... pipes) {
  return unpack(std::make_integer_sequence<int, sizeof...(tuple_types)>(),
                tuple, pipes...);
}
template<typename... args>
pipe nullpipe([](args...){return;});
// std::apply([](auto ...x)
// {std::make_tuple(some_function(x)...);} , the_tuple);
template <typename funct> using function_pipe = pipe<funct>;
} // namespace mtx
