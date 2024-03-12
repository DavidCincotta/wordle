#include "PyString.h"
#include <gtest/gtest.h>

TEST(PyStringTest, Split) {
  std::vector<std::string> expected{"hello", "world"};
  auto result = PyString::split("hello,world", ",");
  ASSERT_EQ(expected, result);
}

TEST(PyStringTest, Join) {
  std::vector<std::string> parts{"hello", "world"};
  std::string expected = "hello,world";
  auto result = PyString::join(parts, ",");
  ASSERT_EQ(expected, result);
}

TEST(PyStringTest, Strip) {
  std::string expected = "hello, world";
  auto result = PyString::strip("  hello, world  ");
  ASSERT_EQ(expected, result);
}

TEST(PyStringTest, Replace) {
  std::string expected = "hello, C++";
  auto result = PyString::replace("hello, world", "world", "C++");
  ASSERT_EQ(expected, result);
}

TEST(PyStringTest, Upper) {
  std::string expected = "HELLO, WORLD";
  auto result = PyString::upper("hello, world");
  ASSERT_EQ(expected, result);
}

TEST(PyStringTest, Lower) {
  std::string expected = "hello, world";
  auto result = PyString::lower("HELLO, WORLD");
  ASSERT_EQ(expected, result);
}

// Add more tests for the remaining functions...

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
