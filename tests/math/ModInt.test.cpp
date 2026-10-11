#include "math/ModInt.cpp"

long long inverse_reference(long long a, long long mod) {
    long long b = mod, x = 1, y = 0;
    while (b) {
        long long quotient = a / b;
        a -= quotient * b; swap(a, b);
        x -= quotient * y; swap(x, y);
    }
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

template<int modulus>
void verify() {
    using Mint = ModInt<modulus>;
    mt19937_64 rng(20261012);
    assert(Mint(numeric_limits<long long>::min()).val() >= 0);
    for (int i = 0; i < 10000; ++i) {
        long long a = rng() % modulus, b = rng() % modulus;
        Mint x(a), y(b);
        assert((x + y).val() == (a + b) % modulus);
        assert((x - y).val() == (a - b + modulus) % modulus);
        assert((x * y).val() == a * b % modulus);
        assert((-x).val() == (modulus - a) % modulus);
        assert(Mint(a - modulus).val() == a);
        long long expected = 1;
        int exponent = rng() % 30;
        for (int j = 0; j < exponent; ++j) expected = expected * a % modulus;
        assert(x.pow(exponent).val() == expected);
        if (b) {
            assert(y.inv().val() == inverse_reference(b, modulus));
            assert((x / y).val() == a * inverse_reference(b, modulus) % modulus);
        }
        assert((x == y) == (a == b));
        assert((x != y) == (a != b));
    }
    stringstream stream;
    stream << Mint(-1);
    assert(stream.str() == to_string(modulus - 1));
    stream.str("-1"); stream.clear();
    Mint parsed;
    stream >> parsed;
    assert(parsed.val() == modulus - 1);
}

int main() {
    verify<2>(); verify<998244353>(); verify<2147483647>();
    cout << "ModInt: direct arithmetic, Euclidean inverses, powers, and streams passed\n";
}
