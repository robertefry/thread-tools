
#include "thread-tools/wait_lock.hh"

#include <gtest/gtest.h>
#include <functional>
#include <thread>

TEST(WAIT_LOCK,TESTS)
{
    bool succsess = false;

    std::function f_async = [&](thd::wait_lock::async&& async)
    {
        EXPECT_TRUE(async.alive());

        async.wait();
        succsess = true;
    };

    thd::wait_lock wl;
    std::thread t_async {f_async,wl.access()};

    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    wl.notify_all();

    t_async.join();
    EXPECT_TRUE(succsess);
}
