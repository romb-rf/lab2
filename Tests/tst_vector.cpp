#include <gtest/gtest.h>
#include "VectorStructure.h"
TEST(VectorTest, AddAndSize) {
    VectorStructure vec;
    EXPECT_EQ(vec.size(), 0);
    vec.add(42);
    vec.add(84);
    EXPECT_EQ(vec.size(), 2);
}
TEST(VectorTest, RemoveLast) {
    VectorStructure vec;
    vec.add(1);
    vec.add(2);
    vec.remove();
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.getElements()[0], Element(1));
}
TEST(VectorTest, Clear) {
    VectorStructure vec;
    vec.add(10);
    vec.add(20);
    vec.clear();
    EXPECT_TRUE(vec.getElements().empty());
    EXPECT_EQ(vec.size(), 0);
}
TEST(VectorTest, ReplaceElement) {
    VectorStructure vec;
    vec.add(5);
    vec.add(15);
    vec.replace(0, 55);
    EXPECT_EQ(vec.getElements()[0], Element(55));
}
TEST(VectorTest, ReplaceOutOfRange) {
    VectorStructure vec;
    vec.add(1);
    EXPECT_THROW(vec.replace(5, 0), std::out_of_range);
}
