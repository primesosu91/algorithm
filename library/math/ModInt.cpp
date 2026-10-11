#include <bits/stdc++.h>
using namespace std;

template<int MOD>
struct ModInt {
    static_assert(MOD > 1);

    int value;

    // 初期化: x を [0, MOD) に正規化
    ModInt(long long x = 0) {
        x %= MOD;
        if (x < 0) x += MOD;
        value = (int)x;
    }

    // 値の取得: val() -> [0, MOD)
    int val() const {
        return value;
    }

    // 加算代入: 自身に other を加える
    ModInt& operator+=(const ModInt& other) {
        long long sum = (long long)value + other.value;
        if (sum >= MOD) sum -= MOD;
        value = (int)sum;
        return *this;
    }

    // 減算代入: 自身から other を引く
    ModInt& operator-=(const ModInt& other) {
        value -= other.value;
        if (value < 0) value += MOD;
        return *this;
    }

    // 乗算代入: 自身に other を掛ける
    ModInt& operator*=(const ModInt& other) {
        value = (long long)value * other.value % MOD;
        return *this;
    }

    // 除算代入: 自身を other で割る
    ModInt& operator/=(const ModInt& other) {
        return *this *= other.inv();
    }

    // 単項プラス
    ModInt operator+() const {
        return *this;
    }

    // 単項マイナス
    ModInt operator-() const {
        return value == 0 ? ModInt(0) : ModInt(MOD - value);
    }

    // 累乗: pow(n) -> 自身の n 乗、n >= 0
    ModInt pow(long long n) const {
        assert(n >= 0);

        ModInt result = 1;
        ModInt base = *this;

        while (n > 0) {
            if (n & 1) result *= base;
            base *= base;
            n >>= 1;
        }

        return result;
    }

    // 逆元: inv() -> 自身の逆元、MOD は素数かつ自身は 0 でない
    ModInt inv() const {
        assert(value != 0);
        return pow(MOD - 2);
    }

    // 加算
    friend ModInt operator+(ModInt lhs, const ModInt& rhs) {
        return lhs += rhs;
    }

    // 減算
    friend ModInt operator-(ModInt lhs, const ModInt& rhs) {
        return lhs -= rhs;
    }

    // 乗算
    friend ModInt operator*(ModInt lhs, const ModInt& rhs) {
        return lhs *= rhs;
    }

    // 除算
    friend ModInt operator/(ModInt lhs, const ModInt& rhs) {
        return lhs /= rhs;
    }

    // 等価判定
    friend bool operator==(const ModInt& lhs, const ModInt& rhs) {
        return lhs.value == rhs.value;
    }

    // 非等価判定
    friend bool operator!=(const ModInt& lhs, const ModInt& rhs) {
        return lhs.value != rhs.value;
    }

    // 入力: 整数を読み込み [0, MOD) に正規化
    friend istream& operator>>(istream& is, ModInt& x) {
        long long value;
        if (is >> value) x = ModInt(value);
        return is;
    }

    // 出力: [0, MOD) の値を出力
    friend ostream& operator<<(ostream& os, const ModInt& x) {
        return os << x.value;
    }
};
