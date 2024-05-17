#include <gtest/gtest.h>
#include <span>

namespace spantest {

TEST(SpanTest, SubSequence) {
  std::vector<int> v = {1, 2, 3, 4, 5};

  std::span s{v};

  // (1) : テンプレート引数として要素数を指定して、先頭3要素を取得する。
  // テンプレート内で使用するときは、s.template first<3>();
  // のようにtemplate限定子の指定が必要
  std::span<int, 3> static_first = s.first<3>();    // [1, 2, 3]
  std::span<int, 3> static_last = s.last<3>();      // [3, 4, 5]
  std::span<int, 3> static_sub = s.subspan<2, 3>(); // [3, 4, 5]

  // (2) : 引数として `size_t Extent = std::dynamic_extent`
  // を指定して、先頭3要素を取得する
  std::span<int, std::dynamic_extent> dynamic_first = s.first(3);
  std::span<int, std::dynamic_extent> dynamic_last = s.last(3);
  std::span<int, std::dynamic_extent> dynamic_sub = s.subspan(2, 3);

  assert(std::equal(static_first.begin(), static_first.end(),
                    dynamic_first.begin()));
  assert(
      std::equal(static_last.begin(), static_last.end(), dynamic_last.begin()));
  assert(std::equal(static_sub.begin(), static_sub.end(), dynamic_sub.begin()));
}

TEST(SpanTest, Size) {
  std::vector<int> v = {1, 2, 3, 4, 5};

  // size() const noexcept; 要素数を取得する
  assert(std::span{v}.size() == v.size());
  assert(std::span{v}.first(3).size() == 3);

  // size_bytes() const noexcept; バイト数を取得する
  // [NOTE] std::span の対象はメモリの連続性を持つシーケンスであるため、
  // 常に sizeof(T) * size() と等価
  assert(std::span{v}.size_bytes() == v.size() * sizeof(int));
  assert(std::span{v}.first(3).size_bytes() == 3 * sizeof(int));

  // bool empty() const noexcept; 空かどうかを判定する
  assert(std::span{v}.empty() == false);
  assert(std::span<int>{}.empty());
  assert((std::span<int>{v.data(), 0}.empty()));
}
} // namespace spantest