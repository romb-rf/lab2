#include <gtest/gtest.h>
#include "StackStructure.h"
TEST(StackTest, AddAndTop) {
    StackStructure st;
    st.add(10);
    st.add(20);
    EXPECT_EQ(st.size(), 2);
    auto elems = st.getElements();
    EXPECT_EQ(elems[1], Element(20));
}
TEST(StackTest, Pop) {
    StackStructure st;
    st.add(1);
    st.add(2);
    st.remove();
    EXPECT_EQ(st.size(), 1);
    EXPECT_EQ(st.getElements()[0], Element(1));
}
TEST(StackTest, PopFromEmpty) {
    StackStructure st;
    EXPECT_THROW(st.remove(), std::out_of_range);
}
TEST(StackTest, Replace) {
    StackStructure st;
    st.add(1);
    st.add(2);
    st.replace(1, 99);
    EXPECT_EQ(st.getElements()[1], Element(99));
}
TEST(StackTest, Clear) {
    StackStructure st;
    st.add(1);
    st.clear();
    EXPECT_TRUE(st.getElements().empty());
}
