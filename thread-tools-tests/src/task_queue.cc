
#include "thread-tools/task_queue.hh"

#include <gtest/gtest.h>
#include <string>
#include <functional>

void ExpectString(std::string const& str)
{
    EXPECT_EQ(str,"FooBar");
}

TEST(TASK_QUEUE,TASKS)
{
    thd::task_queue tq;

    {
        auto future = tq.push_task(ExpectString,"FooBar");
        tq.execute_one();
    }

    {
        auto future = tq.push_task([]() -> std::string
        {
            return "Hello, World!";
        });
        tq.execute_one();
        EXPECT_EQ(future.get(),"Hello, World!");
    }

    {
        std::function f_add = [](int a, int b)
        {
            return a + b;
        };
        auto future = tq.push_task(f_add,24,18);
        tq.execute_one();
        EXPECT_EQ(future.get(),42);
    }

}
