#include <bits/stdc++.h>
using namespace std;

template<class T, T (*op)(T, T), T (*e)()>
struct SegmentTree {
    int n;
    int size;
    vector<T> data;

    // 初期化: SegmentTree(要素数)
    // 前提: 0 <= 要素数 <= 2^30
    SegmentTree(int n) : n(n) {
        assert(n >= 0);
        assert(n <= (1 << 30));

        size = 1;
        while (size < n) size *= 2;

        data.assign((size_t)2 * size, e());
    }

    // 初期化: SegmentTree(初期配列)
    // 前提: 要素数 <= 2^30
    SegmentTree(const vector<T>& v) : n((int)v.size()) {
        assert(v.size() <= (size_t)(1 << 30));

        size = 1;
        while (size < n) size *= 2;

        data.assign((size_t)2 * size, e());

        // 葉に初期値を設定
        for (int i = 0; i < n; ++i) {
            data[size + i] = v[i];
        }

        // 親を構築
        for (int i = size - 1; i >= 1; --i) {
            data[i] = op(data[2 * i], data[2 * i + 1]);
        }
    }

    // 更新: set(位置, 値)
    void set(int pos, T value) {
        assert(0 <= pos && pos < n);

        pos += size;
        data[pos] = value;

        // 根まで更新
        while (pos > 1) {
            pos /= 2;
            data[pos] = op(data[2 * pos], data[2 * pos + 1]);
        }
    }

    // 取得: get(位置) -> 現在値
    T get(int pos) const {
        assert(0 <= pos && pos < n);

        return data[size + pos];
    }

    // 区間取得: prod(左端, 右端) -> [left, right)
    T prod(int left, int right) const {
        assert(0 <= left && left <= right && right <= n);

        T left_value = e();
        T right_value = e();

        left += size;
        right += size;

        while (left < right) {
            if (left % 2 == 1) {
                left_value = op(left_value, data[left]);
                ++left;
            }

            if (right % 2 == 1) {
                --right;
                right_value = op(data[right], right_value);
            }

            left /= 2;
            right /= 2;
        }

        return op(left_value, right_value);
    }

    // 全区間取得: all_prod() -> [0, n)
    T all_prod() const {
        return data[1];
    }

    // 二分探索: max_right(左端, 判定関数)
    // f(prod(left, right)) が true となる最大の right を返す
    // 前提: f(e()) == true、判定に単調性がある
    template<class F>
    int max_right(int left, F f) const {
        assert(0 <= left && left <= n);
        assert(f(e()));

        if (left == n) return n;

        left += size;
        T value = e();

        do {
            while (left % 2 == 0) {
                left /= 2;
            }

            if (!f(op(value, data[left]))) {
                while (left < size) {
                    left *= 2;

                    if (f(op(value, data[left]))) {
                        value = op(value, data[left]);
                        ++left;
                    }
                }

                return left - size;
            }

            value = op(value, data[left]);
            ++left;

        } while ((left & -left) != left);

        return n;
    }

    // 二分探索: min_left(右端, 判定関数)
    // f(prod(left, right)) が true となる最小の left を返す
    // 前提: f(e()) == true、判定に単調性がある
    template<class F>
    int min_left(int right, F f) const {
        assert(0 <= right && right <= n);
        assert(f(e()));

        if (right == 0) return 0;

        right += size;
        T value = e();

        do {
            --right;

            while (right > 1 && right % 2 == 1) {
                right /= 2;
            }

            if (!f(op(data[right], value))) {
                while (right < size) {
                    right = 2 * right + 1;

                    if (f(op(data[right], value))) {
                        value = op(data[right], value);
                        --right;
                    }
                }

                return right + 1 - size;
            }

            value = op(data[right], value);

        } while ((right & -right) != right);

        return 0;
    }
};

// examples

long long op(long long a, long long b) {
    return a + b;
}

long long e() {
    return 0LL;
}

void yosupo_judge_Point_Add_Range_Sum() {
    int N, Q;
    cin >> N >> Q;
    vector<long long> a(N);
    for (int i = 0; i < N; i ++) cin >> a[i];
    SegmentTree<long long, op, e> seg(a);
    for (int q = 0; q < Q; q ++) {
        int query;
        cin >> query;
        if (query == 0) {
            int p, x;
            cin >> p >> x;
            seg.set(p, a[p] + x);
            a[p] += x;
        } else {
            int l, r;
            cin >> l >> r;
            cout << seg.prod(l, r) << endl;
        }
    }
}

int main() {
    yosupo_judge_Point_Add_Range_Sum();
}