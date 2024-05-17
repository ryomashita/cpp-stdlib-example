#include <gtest/gtest.h>
#include <span>

namespace spantest {

// std::span (C++20~) Constructor
// Reference: https://cpprefjp.github.io/reference/span/span/op_constructor.html

TEST(SpanTest, ctor) {
  std::vector<int> v = {1, 2, 3, 4, 5};

  // (1) デフォルトコンストラクタ
  // span()
  {
    // 長さ0の参照範囲 をもつspanオブジェクト
    std::span<int, 0> s1;
    assert(s1.empty());

    // 動的な要素数 をもつspanオブジェクト
    std::span<int> s2;
    assert(s2.empty());

    // 長さ1以上のspanは、参照範囲を設定しなければならない
    // std::span<int, 1> s3{}; // コンパイルエラー
  }

  // (2) イテレータと要素数の組を指定
  // span(It first, size_type count)
  {
    // vの先頭3要素を参照する。
    std::span<int> s{v.begin(), 3};
    assert(s.size() == 3);
    assert(s[0] == 1);
    assert(s[1] == 2);
    assert(s[2] == 3);
  }

  // (3) 範囲を指定
  // span(It first, End last)
  {
    std::span<int> s{v.begin(), v.begin() + 3};
    assert(s.size() == 3);
    assert(s[0] == 1);
    assert(s[1] == 2);
    assert(s[2] == 3);
  }

  // (4) 組み込み配列への参照を指定
  // span(type_identity_t<element_type> (&arr)[N])
  {
    int ar[] = {1, 2, 3, 4, 5};
    std::span<int> s{ar};
    assert(s.size() == 5);
    assert(s.data() == ar); // 元の配列をコピーせず、参照している
  }

  // (5) std::arrayオブジェクトへの参照を指定
  // span(array<T, N> &arr)
  {
    std::array ar = {1, 2, 3, 4, 5};
    std::span<int> s{ar};
    assert(s.size() == ar.size());
    assert(s.data() == ar.data());
  }

  // (6) const std::arrayオブジェクトへの参照を指定
  // span(const array<T, N> &arr)
  {
    std::array ar = {1, 2, 3, 4, 5};
    const auto &car = ar;
    std::span<const int> s{car};

    assert(s.size() == car.size());
    assert(s.data() == car.data());
  }

  // (7) メモリの連続性をもつイテレータをもつオブジェクトの要素全体を参照させる
  // span(R&& r)
  {
    std::span<int> s1{v};
    assert(s1.size() == v.size());
    assert(s1.data() == v.data());

    // std::string_viewの代わり
    std::string str = "Hello";
    std::span<char> s2{str};
    assert(s2.size() == str.size());
    assert(s2.data() == str.data());
  }

  // (8) コピーコンストラクタ
  // span(const span &other)
  {
    std::span<int> s1{v};
    std::span<int> s2 = s1;

    // コピー元とコピー先が同じ範囲を参照する
    assert(s1.data() == v.data());
    assert(s2.data() == v.data());
  }

  // (9) 変換コンストラクタ
  // span(const span<OtherElementType, OtherExtent>& s)
  {
    int ar[] = {1, 2, 3};

    std::span<int, 3> s1{ar};
    std::span<int> s2 = s1;          // span<T, N> -> span<T> への変換
    std::span<int> s3 = s2.first(2); // span<T> -> span<T> への変換
    std::span<const int> s4 = s3;    // T -> const T への変換
    std::span<const int32_t> s5 = s4;
    // T -> U への変換（std::convertible_to<T, U> && sizeof(U) == sizeof(T)）

    assert(s5.size() == 2);
    assert(s5.data() == ar);
  }
}

} // namespace spantest
