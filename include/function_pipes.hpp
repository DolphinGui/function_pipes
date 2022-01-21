// unfortunately module support
// isn't very good, so I'll have
// to disable this for now.
#if (__cplusplus >= 202002L && false)

#define __export_macro_mtx export

import<tuple> import<utility> export module mtx.function_pipes

#else
// yes I know this is super illegal no I don't care
#define __export_macro_mtx
#pragma once
#include <tuple>
#include <utility>

#endif
    namespace mtx {

  __export_macro_mtx template <typename inner> class pipe : public inner {
  public:
    constexpr explicit pipe(inner &&inside) noexcept
        : inner{std::forward<inner>(inside)} {};
  };

  __export_macro_mtx template <typename F, typename T>
  constexpr decltype(auto) operator|(T &&param, pipe<F> function) {
    return function(std::forward<T>(param));
  }

  __export_macro_mtx template <typename F, typename... T>
  constexpr decltype(auto) operator|(std::tuple<T...> &&params,
                                     pipe<F> function) {
    return std::apply(function, std::forward<std::tuple<T...>>(params));
  }

  __export_macro_mtx template <typename F1, typename F2>
  constexpr decltype(auto) operator|(pipe<F1> function1, pipe<F2> function2) {
    return pipe([=](auto... args) { return function2(function1(args...)); });
  }

  __export_macro_mtx template <typename F1, typename F2>
  constexpr decltype(auto) operator&(pipe<F1> &&function1,
                                     pipe<F2> &&function2) {
    return pipe([&](auto... args) { return function2(function1(args...)); });
  }

  __export_macro_mtx template <typename F1, typename F2>
  constexpr decltype(auto) operator^(pipe<F1> &&function1,
                                     pipe<F2> &&function2) {
    return pipe([function1 = std::move(function1),
                 function2 = std::move(function2)](auto... args) {
      return function2(function1(args...));
    });
  }

  __export_macro_mtx template <typename Int, Int... params, Int... functs,
                               typename... tuple_types, typename... pipe_types>
  constexpr decltype(auto) unpack(
      std::integer_sequence<Int, params...> param_seq,
      std::integer_sequence<Int, functs...> funct_seq,
      std::tuple<tuple_types...>&& tuple, pipe_types && ...pipes) {
    auto functions = std::make_tuple(pipes...);
    return std::make_tuple(
        (std::get<functs>(functions)(std::get<params>(tuple)))...);
  }

  __export_macro_mtx template <typename... tuple_types, typename... pipe_types>
  constexpr decltype(auto) unpack(std::tuple<tuple_types...> && tuple,
                                  pipe_types && ...pipes) {
    static_assert(sizeof...(tuple_types) == sizeof...(pipe_types),
                  "number of functions does not match tuple members");
    return unpack(std::make_integer_sequence<int, sizeof...(tuple_types)>(),
                  std::make_integer_sequence<int, sizeof...(tuple_types)>(),
                  std::forward<decltype(tuple)>(tuple),
                  std::forward<decltype(pipes)>(pipes)...);
  }
} // namespace mtx
