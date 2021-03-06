### Getting started

Simply save [`cpp_arrows.hpp`](https://raw.githubusercontent.com/yarric/CppArrows/master/release/CppArrows/cpp_arrows.hpp) to your preferred location for header files. A C++14 compiler required (tested with clang 3.6.0).

### Background

Arrows represent a general method of composing computations. Another (less general) method 
is, for example, monads. Arrows were first defined by John Hughes 
([draft](http://www.cse.chalmers.se/~rjmh/Papers/arrows.pdf)). For more info on arrows please visit [here](https://www.haskell.org/arrows/index.html) (in Haskell). There's also a [tutorial](https://wiki.haskell.org/Arrow_tutorial) 
and a [wiki](https://en.wikibooks.org/wiki/Haskell/Understanding_arrows) on arrows (with code examples in Haskell).
 
The concept of arrows is used in real-world programming in frameworks like [Yampa](https://wiki.haskell.org/Yampa) and [Netwire](https://wiki.haskell.org/Netwire) intended for creating game logic, artificial intelligence, continuous signal synthesis and simulations. 

### Usage example 

Please see the complete [`usage_example.cpp`](/examples/usage_example.cpp). Scalar types and arithmetic 
operations are used for simplicity, the arguments can be of any type. 

First off, let's include `cpp_arrows.hpp` as usual 

```c++
#include "cpp_arrows.hpp"  // supposing it's in the same folder with usage_example.cpp
```

Arrows are normally created from lambdas or class methods with the `make_arrow<In>(lambda)`
function where `In` is the type of the lambda's input parameter

```c++
// <int> is the type of the lambda input param
auto plus10  = cpp_arrows::make_arrow<int>([](int x) { return x+10; });
auto mult55  = cpp_arrows::make_arrow<int>([](int x) { return x*55; });
auto div10_8 = cpp_arrows::make_arrow<int>([](int x) { return (float)x/10.8f; });
```

An arrow can be used as the usual lambda

```c++
auto res1 = plus10(20);  // res == 30 
```

Arrows support different kinds of composition, for example serial composition. Here
`plus10` is applied first to `75`, then `mult55` is applied to the result of `plus10(75)`

```c++
auto plus_mult = plus10 >> mult55;   // equivalent to mult55(plus10(75))
auto res1 = plus_mult(75);           // res1 == (75+10)*55 == 4675
```

Arrows also support parallel composition. In this example `mult55` and `div10_8` are 
applied simultaneously with the result of each operation stored as the `first` and `second`
member of `std::pair`, respectively

```c++
auto mult_div = mult55 & div10_8;    
auto res3 = mult_div(35);            // res3 == std::make_pair(mult55(35), div10_8(35));
```

Let's compose some more arrows. Here the `peek` arrow prints the pair with `std::cout` and 
the `sum_pair` sums the `first` and `second` members of `std::pair` (see 
[`usage_example.cpp`](/examples/usage_example.cpp) for more details).
 
 ```c++
 auto composed_with_arrows = plus10 >> (mult55 & div10_8) >> peek >> sum_pair;
 ```
 
 Programmed with the usual application of functions the computation represented by `composed_arrow`
 would look like
 
 ```c++
 auto composed_with_funcs = [=](int in)
 {
    int  res_plus = plus10(in);
    auto res_mult_div = std::make_pair(mult55(res_plus), div10_8(res_plus));
    return sum_pair(peek(res_mult_div));
 };
 ```
 
 So the following statements yield the same result
 ```c++
 float res_1 = composed_with_arrows(55);
 float res_2 = composed_with_funcs(55);
 // res_1 == res_2
 std::cout << "   Result with arrows: " << res_1 << " vs. "
           << "result with functions: " << res_2 << std::endl;
 ```
 
 The output is
 
 ```   Result with arrows: 3581.02 vs. result with functions: 3581.02```
