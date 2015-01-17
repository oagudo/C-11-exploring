#include <functional>
#include <iostream>
#include <string>
#include <vector>

/*
  std::function overload resolution based on SFINAE on the converting constructor
  Problem description:
   - std::function contains a templatized constructor and can be initialized with any object. 
   - Compiler cannot resolve the overload of two signatures using std::function parameters.

  The following program will not compile (tested in g++-4.7):

  #include <functional>
  #include <string>

  struct Result { };
  struct Input { };

  void foo(const std::function<Result(const Input &)> &f) {}
  void foo(const std::function<void(const Input &)> &f) {}

  int main() {
	  foo([](const Input &) { return Result(); }); // -> error: call of overloaded ‘foo(main()::)’ is ambiguous
  }
*/

struct Result {
};

struct Input {
};

/*!
  Holds a set of std functions based on a common signature
*/
std::vector<std::function<Result(const Input &)> > _functions;

template <typename T>
typename std::enable_if<std::is_convertible<T, Result(*)(const Input &)>::value>::type
foo(const T&& f)
{
    _functions.push_back(f);
    std::cout << "foo(const std::string &, const std::function<Result(const Input &msg)> &)" << std::endl;
    f(Input());
}

void foo(const std::function<void(const Input &)> &f)
{
    _functions.push_back(
                      // Wrap function passed as argument
                      [&](const Input &msg) -> Result
                      {
                          f(msg);
                          return Result();
                      }
                      );
    std::cout << "foo(const std::string &, const std::function<void(const Input &msg)> &)" << std::endl;
    f(Input());
}

int main()
{
    foo([](const Input &) -> Result
        {
            std::cout << "lambda 1 called" << std::endl;
            return Result();
        });

    foo([](const Input &)
        {
            std::cout << "lambda 2 called" << std::endl;
        } );
}
