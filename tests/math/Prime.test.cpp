#include "math/Prime.cpp"

int main() {
    const int maximum = 6000000;
    vector<bool> prime(maximum + 1, true);
    prime[0] = prime[1] = false;
    for (int p = 2; (long long)p * p <= maximum; ++p) if (prime[p]) {
        for (int multiple = p * p; multiple <= maximum; multiple += p) prime[multiple] = false;
    }
    int count = 0;
    for (int n = 0; n <= maximum; ++n) {
        count += prime[n];
        if (n <= 20000) assert(is_prime(n) == prime[n]);
        if (n <= 100 || n % 123457 == 0 || n == maximum) assert(prime_count(n) == count);
    }
    for (int n = 1; n <= 5000; ++n) {
        auto factors = factorize(n);
        long long restored = 1, previous = 0;
        for (auto [p, exponent] : factors) {
            assert(p > previous && prime[p] && exponent > 0);
            previous = p;
            for (int i = 0; i < exponent; ++i) restored *= p;
        }
        assert(restored == n);
        vector<long long> expected;
        for (int d = 1; d <= n; ++d) if (n % d == 0) expected.push_back(d);
        assert(divisors(n) == expected);
    }
    for (int p = 2; p < 200; ++p) if (prime[p]) {
        long long root = primitive_root(p);
        auto generates = [&](long long g) {
            vector<bool> seen(p);
            long long value = 1;
            for (int i = 0; i < p - 1; ++i) { seen[value] = true; value = value * g % p; }
            for (int v = 1; v < p; ++v) if (!seen[v]) return false;
            return true;
        };
        assert(generates(root));
        for (long long g = 1; g < root; ++g) assert(!generates(g));
    }
    const long long p = 999999937, q = 1000000007;
    auto large_factors = factorize(p * q);
    assert((large_factors == vector<pair<long long, int>>{{p, 1}, {q, 1}}));
    assert(!is_prime(1000000000000000000LL));
    cout << "Prime: independent sieve to 6e6, factor/divisor restoration, and minimal primitive roots passed\n";
}
