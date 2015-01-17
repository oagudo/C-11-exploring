#include <functional>
#include <iostream>
#include <string>
#include <map>



class Result{
public:    
    Result() {}
};

class Message{
public:    
    Message() {}
};

std::map<std::string, std::function<Result(const Message &)> > _map;

// Result(const Message &)
// void(const Message &)

template <typename T>
typename std::enable_if<std::is_convertible<T, Result(*)(const Message &)>::value>::type
foo(const std::string &str, const T&& f)
{
    _map.insert( std::make_pair(str, f) );
    f(Message());
}

void foo(const std::string &str, const std::function<void(const Message &msg)> &f)
{
    _map.insert( 
        
    std::make_pair(str, 
    [&](const Message &msg) -> auto 
    { 
        f(msg);
        return Result();
    }
    )
    
    );

    f(Message());

}

int main()
{
    foo("str", [](const Message &) -> auto 
    { 
        std::cout << "lambda 1 called" << std::endl;
        return Result();
    });
    foo("str", [](const Message &) { std::cout << "lambda 2 called" << std::endl;  } );
}
