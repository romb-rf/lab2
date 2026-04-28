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

// Аналогично векторной логике, если есть переопределение размера – тоже тестируем.
