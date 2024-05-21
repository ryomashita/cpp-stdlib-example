#include <gtest/gtest.h>

#include <numeric>
#include <ranges>

// Range アダプタ (C++20~)
// [Description]
// Range アダプタは 既存の Range から新たな view を生成できる。
// view は Range 型の特殊なケースで、所有権を持たず軽量な参照を提供する。
// Range はコンテナや配列を（従来のイテレータ組と異なり）１つの値として直接表す
// [Usage]
// <ranges> ヘッダー (`std::ranges::view` または `std::views`) に定義
// 関数記法に加え、パイプ演算子 `|` を使った記法が利用できる。
// - `R3(R2(R1(r)))` は `r | R1 | R2 | R3` と書ける。
// Range
// アダプタの処理は遅延評価される。つまり要素は必要になるまで生成されない。

TEST(RangeTest, RangeAdapter) {
  std::vector<int> v{1, 2, 3, 4, 5};

  // std::views::all : すべての要素を含む view を生成
  {
    auto r = v | std::views::all;
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 15);
  }

  // std::views::filter : 条件を満たす要素だけを含む view を生成
  {
    auto r = v | std::views::filter([](int x) { return x % 2 == 0; });
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 6);
  }

  // std::views::transform : 要素を変換した view を生成
  {
    auto r = v | std::views::transform([](int x) { return x * 2; });
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 30);
  }
  {
    // ラムダのほかに、メンバポインタも transform に渡せる
    struct Record {
      int id = 0;
      std::string name;
    };
    Record records[] = {{1, "Alice"}, {2, "Bob"}, {3, "Charlie"}};
    auto r = records | std::views::transform(&Record::name);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), std::string{}),
              "AliceBobCharlie");
  }

  // std::views::take : 先頭から指定した要素だけを含む view を生成
  // [WARN]
  // take は ランダムアクセス可能 (random_access_range) な入力にのみ適用可能。
  // 逆に無限長の Range (iota, istream) では無限ループが発生する。
  // (Reference: https://cpprefjp.github.io/reference/ranges/take_view.html)
  {
    auto r = v | std::views::take(3);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 6);
  }

  // std::views::take_while : 先頭から連続して条件を満たす要素だけを含む view
  // を生成
  {
    auto r = v | std::views::take_while([](int x) { return x < 4; });
    for (auto x : r) {
      EXPECT_LT(x, 4);
    }
    // EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 6); // compile error
  }
}
