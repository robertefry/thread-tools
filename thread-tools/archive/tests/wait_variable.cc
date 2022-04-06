
#include "thread-tools/wait_variable.hh"

#include <gtest/gtest.h>
#include <thread>
#include <condition_variable>

TEST(WAIT_VARIABLE,CONDITION_VARIABLE)
{
    std::condition_variable cv;
    std::mutex mutex;
    bool succsess = false;

    std::function f_async = [&](thd::wait_variable& wv)
    {
        wv.wait();
        succsess = true;
    };

    thd::wait_variable wv {cv,mutex};
    std::thread t_async {f_async,wv};

    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    {
        std::unique_lock lock {mutex};
        cv.notify_all();
    }
    EXPECT_TRUE(succsess);
}
