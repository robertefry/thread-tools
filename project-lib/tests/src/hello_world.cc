
#include <gtest/gtest.h>

#include "hello_world.hh"

TEST(PROJECT_LIB,HELLO_WORLD)
{
    EXPECT_EQ(GetHelloWorld(),"Hello, World!");
}
