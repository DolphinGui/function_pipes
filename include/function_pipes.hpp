// unfortunately module support
// isn't very good, so I'll have
// to disable this for now.
#if (__cplusplus >= 202002L && false)

#define __export_macro_mtx export

import <tuple>
import <utility>
export module mtx.function_pipes

#else
// yes I know this is super illegal no I don't care
#define __export_macro_mtx
#pragma once
#include <tuple>
#include <utility>

#endif
namespace mtx {

__export_macro_mtx
template <typename inner> class pipe : public inner {
public:
  constexpr pipe(inner inside) : inner{inside} {};
};

__export_macro_mtx
template <typename F, typename... T>
constexpr decltype(auto) operator | (std::tuple<T...> params, pipe<F> function) {
  return std::apply(function, params);
}

__export_macro_mtx
template <typename F, typename T>
constexpr decltype(auto) operator | (T param, pipe<F> function) {
  return function(param);
}

__export_macro_mtx
template <typename F1, typename F2>
constexpr decltype(auto) operator | (pipe<F1> function1, pipe<F2> function2) {
  return pipe([&](auto... args){ return function2(function1(args...));});
}

__export_macro_mtx
template <typename F1, typename F2>
constexpr decltype(auto) operator & (pipe<F1> function1, pipe<F2> function2) {
  return pipe([=](auto... args){ return function2(function1(args...));});
}

__export_macro_mtx
template <typename Int, Int... ints, typename... tuple_types,
          typename... pipe_types>
constexpr decltype(auto) unpack(std::integer_sequence<Int, ints...> int_seq,
            std::tuple<tuple_types...> tuple, pipe_types... pipes) {
  static_assert(sizeof...(tuple_types) == sizeof...(pipe_types),
                "number of functions does not match tuple members");
  return std::make_tuple((pipes(std::get<ints>(tuple)))...);
}

__export_macro_mtx
template <typename... tuple_types, typename... pipe_types>
constexpr decltype(auto) unpack(std::tuple<tuple_types...> tuple, pipe_types... pipes) {
  return unpack(std::make_integer_sequence<int, sizeof...(tuple_types)>(),
                tuple, pipes...);
}
} // namespace mtx
