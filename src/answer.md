## The answer

The undefined behavior here is an implicit capture by reference
the `operator |` does.

    template <typename F1, typename F2>
    constexpr decltype(auto) operator | (pipe<F1> function1, pipe<F2> function2) {
      return pipe([&](auto... args){ return function2(function1(args...));});
    }

Capture by value might copy large objects, creating unnecessary overhead
and increasing memory footprint, as well as preventing move-only captures
such as unique_ptr. If preventing the dangling reference is really necessary,
`operator &`, a function-composition-only operator, captures by value rather than reference.