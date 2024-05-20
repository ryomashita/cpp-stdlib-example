#include <gtest/gtest.h>

#include <ranges>

// Range ファクトリ (C++20~)
// [Description]
// Range ファクトリは 非Rangeの値から view 型を生成できる
// view 型は Range 型の特殊なケースで、所有権を持たず軽量な参照を提供する
// Range型はコンテナや配列を（従来のイテレータ組と異なり）１つの値として直接表す
// [Usage]
// <ranges> ヘッダーに定義されている
// `std::ranges::view` または `std::views` 名前空間から利用できる

TEST(RangeTest, RangeFactory) {
  std::vector<int> v{1, 2, 3, 4, 5};
  auto r = std::ranges::subrange(v.begin(), v.end());
  EXPECT_EQ(5, std::ranges::distance(r));

  // std::ranges::empty_view
  // 空のRangeを表すview
  {
    std::ranges::empty_view<int> x;
    auto y = std::views::empty<int>; // empty_view を返す変数テンプレート
    assert(std::ranges::empty(x));
    assert(std::ranges::size(y) == 0);
  }

  // std::ranges::single_view
  // 要素１つのRangeを表すview
  {
    std::ranges::single_view<int> x{42};
    auto y = std::views::single(42); // single_view を返す関数テンプレート
    assert(std::ranges::size(x) == 1);
    assert(std::ranges::size(y) == 1);
  }

  // std::ranges::iota_view
  // 単調増加する整数列を表すview
  // step は指定できない。 代わりに views::transform や std::generate
  // などより汎用的な方法を使う必要がある。
  {
    // iota_view{} か views::iota() で生成
    constexpr auto x = std::views::iota(0, 10);
    constexpr decltype(x) sub{x.begin() + 3, x.end()};
    static_assert(sub.front() == 3);

    // 無限長（Rangeアダプタの処理は遅延評価される）
    constexpr std::ranges::iota_view iota1{0};
    static_assert(not std::ranges::sized_range<decltype(iota1)>);
    static_assert(iota1.front() == 0);

    // 1引数だが、範囲は [-5, int()) となり有限長
    constexpr std::ranges::iota_view<int, int> iota2{-5};
    static_assert(std::ranges::sized_range<decltype(iota2)>);
    static_assert(std::ranges::size(iota2) == 5);
  }

  // std::ranges::repeat_view (C++23~)
  // 指定した値を指定回数繰り返す view

  // std::ranges::basic_istream_view
  // 入力ストリームから要素を読み込むview
  {
    auto iss = std::istringstream{"1 2 3 4 5"};
    auto x = std::ranges::views::istream<int>(iss); // std::cin も指定可
    assert(not std::ranges::sized_range<decltype(x)>);
    auto it = std::ranges::begin(x);
    assert(*it == 1);
    ++it; // ここで次の要素が読み込まれる（遅延評価）
    assert(*it == 2);
  }
}