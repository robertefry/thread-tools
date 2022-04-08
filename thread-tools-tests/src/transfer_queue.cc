
#include "thread-tools/transfer_queue.hh"

#include <gtest/gtest.h>

TEST(TRANSFER_QUEUE,TRANSFER)
{
    thd::transfer_queue<int> tq;

    for (int i = 0; i < 4; ++i)
    {
        tq.push(i);
        EXPECT_FALSE(tq.empty());
        EXPECT_EQ(tq.size(),i+1);
        EXPECT_EQ(tq.back(),i);
    }
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(tq.front(),i);
        EXPECT_EQ(tq.size(),4-i);
        EXPECT_EQ(tq.pop(),i);
        EXPECT_EQ(tq.size(),3-i);
    }
    EXPECT_TRUE(tq.empty());
}

TEST(TRANSFER_QUEUE,COPY)
{
    thd::transfer_queue<int> tq1;
    tq1.push(42);

    thd::transfer_queue<int> tq2 = tq1;
    EXPECT_TRUE(tq1.front()==tq2.front());

    thd::transfer_queue<int> tq3;
    tq3 = tq1;
    EXPECT_TRUE(tq1.front()==tq2.front());
}

TEST(TRANSFER_QUEUE,MOVE)
{
    thd::transfer_queue<int> tq1;
    tq1.push(42);

    thd::transfer_queue<int> tq2 = std::move(tq1);
    EXPECT_TRUE(tq1.empty());
    EXPECT_EQ(tq2.front(),42);

    thd::transfer_queue<int> tq3;
    tq3 = std::move(tq2);
    EXPECT_TRUE(tq2.empty());
    EXPECT_EQ(tq3.front(),42);
}
