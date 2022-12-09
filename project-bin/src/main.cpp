
#include "hello_world.hh"

#include <fmt/format.h>
#include <iostream>

int main()
{
    auto const message = GetHelloWorld();
    fmt::print("{}\n",message);
}
