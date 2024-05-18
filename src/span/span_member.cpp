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

  // constexpr size_type size() const noexcept; 要素数を取得する
  assert(std::span{v}.size() == v.size());
  assert(std::span{v}.first(3).size() == 3);

  // constexpr size_type size_bytes() const noexcept; バイト数を取得する
  // [NOTE] std::span の対象はメモリの連続性を持つシーケンスであるため、
  // 常に sizeof(T) * size() と等価
  assert(std::span{v}.size_bytes() == v.size() * sizeof(int));
  assert(std::span{v}.first(3).size_bytes() == 3 * sizeof(int));

  // [[nodiscard]] constexpr bool empty() const noexcept;
  // シーケンスが空かどうか判定する
  assert(std::span{v}.empty() == false);
  assert(std::span<int>{}.empty());
  assert((std::span<int>{v.data(), 0}.empty()));
}

TEST(SpanTest, Accessor) {
  std::vector<int> v = {1, 2, 3, 4, 5};

  // constexpr reference operator[](size_type i) const;
  {
    int &x = std::span{v}[2];
    assert(x == 3);
    int &y = std::span{v}.subspan(2, 3)[1];
    assert(y == 4);
  }

  // (C++26~) constexpr reference at(size_type i) const;

  // constexpr reference front() const;
  {
    int &x = std::span{v}.front();
    assert(x == 1);
    int &y = std::span{v}.subspan(2, 3).front();
    assert(y == 3);
  }

  // constexpr reference back() const;
  {
    int &x = std::span{v}.back();
    assert(x == 5);
    int &y = std::span{v}.subspan(2, 3).back();
    assert(y == 5);
  }

  // constexpr pointer data() const noexcept;
  {
    int *p1 = std::span{v}.data();
    assert(p1 == &v[0]);
    int *p2 = std::span{v}.subspan(2, 3).data();
    assert(p2 == &v[2]);
  }
}

} // namespace spantest
