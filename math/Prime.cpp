#include <bits/stdc++.h>
using namespace std;

namespace prime_internal {
    // mod 乗算: mod_mul(a, b, mod) -> a * b mod mod
    long long mod_mul(long long a, long long b, long long mod) {
        return (long long)((__int128)a * b % mod);
    }

    // mod 累乗: mod_pow(a, n, mod) -> a^n mod mod
    long long mod_pow(long long a, long long n, long long mod) {
        long long res = 1;

        while (n > 0) {
            if (n & 1) res = mod_mul(res, a, mod);
            a = mod_mul(a, a, mod);
            n >>= 1;
        }

        return res;
    }

    // Miller-Rabin: miller_rabin(n, a) -> 基底 a のテストを通れば true
    bool miller_rabin(long long n, long long a) {
        if (a % n == 0) return true;

        long long d = n - 1;
        int s = 0;

        while ((d & 1) == 0) {
            d >>= 1;
            s++;
        }

        long long x = mod_pow(a % n, d, n);

        if (x == 1 || x == n - 1) return true;

        for (int r = 1; r < s; r++) {
            x = mod_mul(x, x, n);

            if (x == n - 1) return true;
        }

        return false;
    }
}

// 素数判定: is_prime(n) -> n が素数なら true（0 <= n <= 1e18）
bool is_prime(long long n) {
    if (n < 2) return false;

    for (long long p : {
        2LL, 3LL, 5LL, 7LL, 11LL, 13LL,
        17LL, 19LL, 23LL, 29LL, 31LL, 37LL
    }) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    for (long long a : {
        2LL, 325LL, 9375LL, 28178LL,
        450775LL, 9780504LL, 1795265022LL
    }) {
        if (!prime_internal::miller_rabin(n, a)) return false;
    }

    return true;
}

namespace prime_internal {
    // Pollard's Rho: pollard_rho(n) -> n の非自明な約数を1つ返す
    long long pollard_rho(long long n) {
        if (n % 2 == 0) return 2;
        if (n % 3 == 0) return 3;

        static mt19937_64 rng(
            chrono::steady_clock::now().time_since_epoch().count()
        );

        while (true) {
            long long c =
                uniform_int_distribution<long long>(1, n - 1)(rng);

            long long x =
                uniform_int_distribution<long long>(0, n - 1)(rng);

            long long y = x;
            long long d = 1;

            auto f = [&](long long v) {
                return (mod_mul(v, v, n) + c) % n;
            };

            while (d == 1) {
                x = f(x);
                y = f(f(y));

                long long diff = x > y ? x - y : y - x;
                d = gcd(diff, n);
            }

            if (d != n) return d;
        }
    }

    // 素因数列挙: factorize_recursive(n, factors) -> 素因数を重複込みで追加
    void factorize_recursive(
        long long n,
        vector<long long>& factors
    ) {
        if (n == 1) return;

        if (is_prime(n)) {
            factors.push_back(n);
            return;
        }

        long long d = pollard_rho(n);

        factorize_recursive(d, factors);
        factorize_recursive(n / d, factors);
    }
}

// 素因数分解: factorize(n) -> {素因数, 指数} を素因数昇順で返す（1 <= n <= 1e18）
vector<pair<long long, int>> factorize(long long n) {
    assert(n >= 1);

    vector<long long> factors;
    prime_internal::factorize_recursive(n, factors);
    sort(factors.begin(), factors.end());

    vector<pair<long long, int>> res;

    for (long long p : factors) {
        if (res.empty() || res.back().first != p) {
            res.push_back({p, 1});
        } else {
            res.back().second++;
        }
    }

    return res;
}

// 約数列挙: divisors(n) -> 正の約数を昇順で返す（1 <= n <= 1e18）
vector<long long> divisors(long long n) {
    assert(n >= 1);

    auto factors = factorize(n);
    vector<long long> res = {1};

    for (auto [p, e] : factors) {
        int size = (int)res.size();
        long long mul = 1;

        for (int i = 1; i <= e; i++) {
            mul *= p;

            for (int j = 0; j < size; j++) {
                res.push_back(res[j] * mul);
            }
        }
    }

    sort(res.begin(), res.end());
    return res;
}

namespace prime_internal {
    const int SIEVE_MAX = 5000000;
    const int PHI_N = 10000;
    const int PHI_M = 100;

    vector<int> sieve_primes;
    vector<int> pi_table;

    long long phi_table[PHI_N][PHI_M];

    bool sieve_initialized = false;

    unordered_map<long long, long long> lehmer_cache;

    // Lehmer用前計算
    void init_sieve() {
        if (sieve_initialized) return;
        sieve_initialized = true;

        vector<bool> is_composite(SIEVE_MAX + 1, false);
        pi_table.assign(SIEVE_MAX + 1, 0);

        for (int i = 2; i <= SIEVE_MAX; i++) {
            if (!is_composite[i]) {
                sieve_primes.push_back(i);

                if ((long long)i * i <= SIEVE_MAX) {
                    for (
                        long long j = (long long)i * i;
                        j <= SIEVE_MAX;
                        j += i
                    ) {
                        is_composite[(int)j] = true;
                    }
                }
            }

            pi_table[i] =
                pi_table[i - 1] + (!is_composite[i]);
        }

        for (int x = 0; x < PHI_N; x++) {
            phi_table[x][0] = x;
        }

        for (int s = 1; s < PHI_M; s++) {
            int p = sieve_primes[s - 1];

            for (int x = 0; x < PHI_N; x++) {
                phi_table[x][s] =
                    phi_table[x][s - 1]
                    - phi_table[x / p][s - 1];
            }
        }
    }

    // φ(x, s): 最初の s 個の素数で割り切れない 1..x の個数
    long long phi(long long x, int s) {
        if (s == 0) return x;

        if (s < PHI_M && x < PHI_N) {
            return phi_table[x][s];
        }

        return
            phi(x, s - 1)
            - phi(x / sieve_primes[s - 1], s - 1);
    }

    // 整数平方根
    long long isqrt(long long x) {
        long long r = sqrtl((long double)x);

        while ((__int128)(r + 1) * (r + 1) <= x) r++;
        while ((__int128)r * r > x) r--;

        return r;
    }

    // 整数立方根
    long long icbrt(long long x) {
        long long r = cbrtl((long double)x);

        while (
            (__int128)(r + 1) * (r + 1) * (r + 1) <= x
        ) {
            r++;
        }

        while ((__int128)r * r * r > x) {
            r--;
        }

        return r;
    }

    // 整数4乗根
    long long iroot4(long long x) {
        long long r =
            sqrtl((long double)isqrt(x));

        auto leq = [&](long long v) {
            __int128 vv = (__int128)v * v;
            return vv * vv <= x;
        };

        while (leq(r + 1)) r++;
        while (!leq(r)) r--;

        return r;
    }

    // Lehmer素数計数
    long long lehmer_pi(long long x) {
        if (x < SIEVE_MAX) {
            return pi_table[(int)x];
        }

        auto it = lehmer_cache.find(x);
        if (it != lehmer_cache.end()) {
            return it->second;
        }

        long long a = lehmer_pi(iroot4(x));
        long long b = lehmer_pi(isqrt(x));
        long long c = lehmer_pi(icbrt(x));

        long long sum =
            phi(x, (int)a)
            + (b + a - 2) * (b - a + 1) / 2;

        for (long long i = a; i < b; i++) {
            long long w =
                x / sieve_primes[(int)i];

            sum -= lehmer_pi(w);

            if (i < c) {
                long long lim =
                    lehmer_pi(isqrt(w));

                for (
                    long long j = i;
                    j < lim;
                    j++
                ) {
                    sum -=
                        lehmer_pi(
                            w / sieve_primes[(int)j]
                        ) - j;
                }
            }
        }

        lehmer_cache[x] = sum;
        return sum;
    }
}

// 素数カウント: prime_count(n) -> n 以下の素数の個数（0 <= n <= 1e11）
long long prime_count(long long n) {
    if (n < 2) return 0;

    assert(n <= 100000000000LL);

    prime_internal::init_sieve();

    return prime_internal::lehmer_pi(n);
}

// 原始根: primitive_root(p) -> 素数 p の最小の原始根（p <= 1e18）
long long primitive_root(long long p) {
    assert(is_prime(p));

    if (p == 2) return 1;

    auto factors = factorize(p - 1);

    for (long long g = 2; ; g++) {
        bool ok = true;

        for (auto [q, e] : factors) {
            if (
                prime_internal::mod_pow(
                    g,
                    (p - 1) / q,
                    p
                ) == 1
            ) {
                ok = false;
                break;
            }
        }

        if (ok) return g;
    }
}

// examples

void yosupo_judge_Primality_Test() {
    int Q;
    cin >> Q;
    for (int q = 0; q < Q; q ++) {
        long long N;
        cin >> N;
        if (is_prime(N)) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
}

void yosupo_judge_Counting_Primes() {
    long long N;
    cin >> N;
    cout << prime_count(N) << endl;
}

void yosupo_judge_Factorize() {
    int Q;
    cin >> Q;
    for (int q = 0; q < Q; q ++) {
        long long a;
        cin >> a;
        vector<pair<long long, int>> list = factorize(a);
        vector<long long> vals;
        for (auto [p, num] : list) {
            for (int i = 0; i < num; i ++) vals.push_back(p);
        }
        cout << vals.size();
        for (long long p : vals) cout << " " << p;
        cout << endl;
    }
}

void yosupo_judge_Primitive_Root() {
    int Q;
    cin >> Q;
    for (int q = 0; q < Q; q ++) {
        long long p;
        cin >> p;
        cout << primitive_root(p) << endl;
    }
}

int main() {
    yosupo_judge_Primality_Test();
    // yosupo_judge_Counting_Primes();
    // yosupo_judge_Factorize();
    // yosupo_judge_Primitive_Root()
}