#include <gtest/gtest.h>
#include "DataManager.h"
class DataManagerTest : public ::testing::Test {
protected:
    DataManager dm;
};
TEST_F(DataManagerTest, DefaultStructureIsVector) {
    EXPECT_EQ(dm.currentType(), "Vector");
}
TEST_F(DataManagerTest, SwitchStructure) {
    dm.setStructureType("Stack");
    EXPECT_EQ(dm.currentType(), "Stack");
}
TEST_F(DataManagerTest, AddElement) {
    dm.addElement("42");
    EXPECT_EQ(dm.elements().size(), 1);
    EXPECT_EQ(dm.elements()[0].toString(), "42");
}
TEST_F(DataManagerTest, RemoveElementFromVector) {
    dm.addElement("1");
    dm.addElement("2");
    dm.removeElement();
    ASSERT_EQ(dm.elements().size(), 1);
    EXPECT_EQ(dm.elements()[0].toString(), "1");
}
TEST_F(DataManagerTest, StackLIFO) {
    dm.setStructureType("Stack");
    dm.addElement("A");
    dm.addElement("B");
    dm.removeElement();
    EXPECT_EQ(dm.elements().size(), 1);
    EXPECT_EQ(dm.elements()[0].toString(), "A");
}
TEST_F(DataManagerTest, QueueFIFO) {
    dm.setStructureType("Queue");
    dm.addElement("X");
    dm.addElement("Y");
    dm.removeElement();
    EXPECT_EQ(dm.elements().size(), 1);
    EXPECT_EQ(dm.elements()[0].toString(), "Y");
}
TEST_F(DataManagerTest, Clear) {
    dm.addElement("1");
    dm.addElement("2");
    dm.clear();
    EXPECT_EQ(dm.elements().size(), 0);
}
TEST_F(DataManagerTest, MedianOdd) {
    dm.addElement("3");
    dm.addElement("1");
    dm.addElement("2");
    EXPECT_EQ(dm.getMedian(), 2.0);
}
TEST_F(DataManagerTest, MedianEven) {
    dm.addElement("4");
    dm.addElement("1");
    dm.addElement("3");
    dm.addElement("2");
    EXPECT_EQ(dm.getMedian(), 2.5);
}
TEST_F(DataManagerTest, MedianSingle) {
    dm.addElement("5");
    EXPECT_EQ(dm.getMedian(), 5.0);
}
TEST_F(DataManagerTest, MedianEmpty) {
    EXPECT_DOUBLE_EQ(dm.getMedian(), 0.0);
}
