#include <gtest/gtest.h>
#include "QueueStructure.h"

TEST(QueueTest, EnqueueAndSize) {
    QueueStructure q;
    q.add(100);
    q.add(200);
    EXPECT_EQ(q.size(), 2);
}

TEST(QueueTest, Dequeue) {
    QueueStructure q;
    q.add(1);
    q.add(2);
    q.remove();                       // удаляет голову (1)
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.getElements()[0], Element(2));
}

TEST(QueueTest, DequeueFromEmpty) {
    QueueStructure q;
    EXPECT_THROW(q.remove(), std::out_of_range);
}

TEST(QueueTest, Replace) {
    QueueStructure q;
    q.add(10);
    q.add(20);
    q.replace(0, 15);                 // замена головы
    EXPECT_EQ(q.getElements()[0], Element(15));
}
