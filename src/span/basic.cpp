#include <gtest/gtest.h>

#include <iostream>
#include <span>
#include <utility>
#include <vector>

namespace spantest {

// メモリ連続性をもつあらゆる範囲を出力する関数。
// std::spanオブジェクトはコピーで受け取るのが基本的な使い方
template <class T, std::size_t Extent> void print(std::span<T, Extent> s) {
  const char *delimiter = "";

  std::cout << '{';
  for (const T &x : s) {
    std::cout << std::exchange(delimiter, ",") << x;
  }
  std::cout << '}' << std::endl;
}

TEST(SpanTest, basic) {
  std::vector<int> v = {1, 2, 3, 4};
  std::span<int> s(v);
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s[0], 1);
  EXPECT_EQ(s[1], 2);
  EXPECT_EQ(s[2], 3);
  EXPECT_EQ(s[3], 4);
}

} // namespace spantest
