#include <gtest/gtest.h>
#include "ArrayStructure.h"
TEST(ArrayTest, AddAndSize) {
    ArrayStructure arr;
    EXPECT_EQ(arr.size(), 0);
    arr.add(10);
    arr.add(20);
    EXPECT_EQ(arr.size(), 2);
}
TEST(ArrayTest, RemoveElement) {
    ArrayStructure arr;
    arr.add(5);
    arr.add(15);
    arr.remove();
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr.getElements()[0], Element(5));
}
TEST(ArrayTest, ShiftCyclic) {
    ArrayStructure arr;
    arr.add(1);
    arr.add(2);
    arr.add(3);
    arr.shift(1);
    auto elems = arr.getElements();
    EXPECT_EQ(elems[0], Element(3));
    EXPECT_EQ(elems[1], Element(1));
    EXPECT_EQ(elems[2], Element(2));
}
TEST(ArrayTest, ReplaceCorrectIndex) {
    ArrayStructure arr;
    arr.add(100);
    arr.add(200);
    arr.replace(0, 999);
    EXPECT_EQ(arr.getElements()[0], Element(999));
}

TEST(ArrayTest, ReplaceOutOfRange) {
    ArrayStructure arr;
    arr.add(1);
    EXPECT_THROW(arr.replace(5, 0), std::out_of_range);
}
TEST(ArrayTest, Clear) {
    ArrayStructure arr;
    arr.add(1);
    arr.add(2);
    arr.clear();
    EXPECT_TRUE(arr.getElements().empty());
}
TEST(ArrayTest, AddAt) {
    ArrayStructure arr;
    arr.add(10);
    arr.add(30);
    arr.addAt(1, 20);
    auto elems = arr.getElements();
    ASSERT_EQ(elems.size(), 3);
    EXPECT_EQ(elems[0], Element(10));
    EXPECT_EQ(elems[1], Element(20));
    EXPECT_EQ(elems[2], Element(30));
}
TEST(ArrayTest, TypeConflict) {
    ArrayStructure arr;
    arr.add(10); // Int
    EXPECT_THROW(arr.add(Element("string")), std::invalid_argument);
}
