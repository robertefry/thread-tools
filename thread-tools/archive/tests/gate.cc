
#include "thread-tools/gate.hh"

#include <gtest/gtest.h>
#include <functional>
#include <thread>

TEST(GATE,WAIT_OPEN)
{
    std::function f_async = [&](thd::gate::async async)
    {
        async.wait<thd::gate::opened>();
        EXPECT_EQ(async.is_open(),true);
    };

    thd::gate gate;
    gate = thd::gate::closed;

    std::thread t_async {f_async,gate.access()};
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    gate = thd::gate::opened;

    t_async.join();
}

TEST(GATE,WAIT_CLOSE)
{
    std::function f_async = [&](thd::gate::async async)
    {
        async.wait<thd::gate::closed>();
        EXPECT_EQ(async.is_open(),false);
    };

    thd::gate gate;
    gate = thd::gate::opened;

    std::thread t_async {f_async,gate.access()};
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    gate = thd::gate::closed;

    t_async.join();
}
