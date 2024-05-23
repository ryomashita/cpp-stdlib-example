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

  // std::views::drop : 先頭から指定した個数の要素を除去した view を生成
  {
    auto r = v | std::views::drop(3); // 4, 5
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 9);
  }

  // std::views::drop_while : 先頭から連続して条件を満たす要素を除去した view
  {
    auto r = v | std::views::drop_while([](int x) { return x < 3; }); // 3, 4, 5
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 12);
  }

  // std::views::join : ネストされた Range を平坦化した view を生成
  {
    std::vector<std::vector<int>> vv{{1, 2}, {3, 4}, {5}};
    auto r = vv | std::views::join; // 1, 2, 3, 4, 5
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 15);

    std::vector<std::string> vs{"Alice", "Bob", "Charlie"};
    auto r2 = vs | std::views::join; // 'A', 'l', 'i', 'c', 'e', ...
    EXPECT_EQ(std::accumulate(r2.begin(), r2.end(), std::string{}),
              "AliceBobCharlie");
  }

  // std::views::lazy_sprit_view : Range を指定した区切り文字で分割した view
  // を生成
  // 入力に input_range = シングルパス(1回だけ走査可能) を要求する。
  // そのため stream の操作に適する。
  {
    std::vector<int32_t> delim{3, 4};
    auto r = v | std::views::lazy_split(delim);
    // {1, 2}, {5}
    for (auto &&sub : r) {
      for (auto x : sub) {
        EXPECT_NE(x, 3);
        EXPECT_NE(x, 4);
      }
    }

    auto iss = std::istringstream{"1 2 3 4 5"};
    auto r2 = std::views::istream<int>(iss) | std::views::lazy_split(3);
    // {1, 2}, {4, 5}
    for (auto &&sub : r2) {
      for (auto x : sub) {
        EXPECT_NE(x, 3);
      }
    }
  }

  // std::views::sprit_view : Range を指定した区切り文字で分割した view を生成
  // 入力に forward_range = マルチパス(複数回走査可能) を要求する
  // lazy_sprit_with より効率的なため、要件を満たす限りこちらを使用すべき。
  // https://learn.microsoft.com/en-us/cpp/standard-library/lazy-split-view-class?view=msvc-170
  {
    auto r = "1,2,3,4,5" | std::views::split(',');
    // {"1"}, {"2"}, {"3"}, {"4"}, {"5"}
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), std::string{},
                              [](std::string acc, auto &&sub) {
                                return acc + sub.front();
                              }),
              "12345");
  }

  // std::views::common : Range から common_range コンセプトを満たす view を生成
  // common_range : イテレータ と 番兵 の型が等しい Range
  // (STL のコンテナは全て common_range)
  {
    auto r = v | std::views::common;
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 15);
  }

  // std::views::reverse : Range を逆順にした view を生成
  // 入力に bidirectional_range (双方向イテレータ) を要求する。
  // つまり単に逆順イテレータを返すだけ。他の Range と同様、遅延評価される。
  {
    auto r = v | std::views::reverse;
    // 5, 4, 3, 2, 1
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 15);
  }
}
