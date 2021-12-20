# function_pipes
a simple header-only library for composing functions in c++

## Why?

Function composition in c++ is painful because it always ends in a pyramid of doom.

```c++
auto json =
stringToJSON(
  read(
    openFile(
      "filename", 
      "permissions"
    ), 
    5,
    123
  )
);
```

Instead now its
```c++
auto json =
std::make_tuple("filename", "permissions") 
| pipe([](const char* filename, const char* permissions){ return openFile(filename, permissions); })
| pipe([](FILE* file){ return read(file, 5, 123); });
| pipe([](std::string string){ return stringToJSON(string); });
```

Where the parameters are actually near the function they're relevant to, and instead of having to keep track
of which scope you're in, it's sequential.

## How to use

```c++
int main(){
  int a = 4 | mtx::pipe([](int a){ return a * 3; });
  int b = std::make_tuple(2,4) | mtx::pipe([](int a, int b){ return a * b; });
  return 0;
}
```

The syntax is [parameters] | mtx::pipe([lambda]). If there are
multiple parameters, use std::tuple to combine them and the library
will deal with std::apply for you.

```c++
int main(){
  int a = 4 | (mtx::pipe([](int a){ return a * 3; }) | mtx::pipe([](int a){ return a - 4; }));
  int b = 4 | (mtx::pipe([](int a){ return a * 3; }) & mtx::pipe([](int a){ return a - 4; }));
  return 0;
}
```

You can even compose functions using `|` or `&`. See example 2 for the differences between the two.

## Performance considerations

None. At least on -O1 for gcc and clang on godbolt, it seems to inline perfectly. Internally it uses
lambdas, so it's the same as using lambdas on the stack.

## How to install

Clone this git repo into your project, and include this in your cmake `include_directories`. I'll figure
out conan and vcpkg packaging later.
