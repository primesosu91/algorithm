#include <bits/stdc++.h>
using namespace std;

template<class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
struct LazySegmentTree {
    int n;
    int size;
    int log;
    vector<T> data;
    vector<F> lazy;

    // 再計算: 子の値からノード k の値を更新
    void update(int k) {
        data[k] = op(data[2 * k], data[2 * k + 1]);
    }

    // 作用: ノード k 全体に作用 f を適用
    void all_apply(int k, F f) {
        data[k] = mapping(f, data[k]);

        if (k < size) {
            lazy[k] = composition(f, lazy[k]);
        }
    }

    // 伝播: ノード k の遅延作用を子へ伝播
    void push(int k) {
        all_apply(2 * k, lazy[k]);
        all_apply(2 * k + 1, lazy[k]);
        lazy[k] = id();
    }

    // 初期化: LazySegmentTree(要素数)
    LazySegmentTree(int n) : n(n) {
        assert(n >= 0);

        size = 1;
        log = 0;

        while (size < n) {
            size *= 2;
            log++;
        }

        data.assign(2 * size, e());
        lazy.assign(size, id());
    }

    // 初期化: LazySegmentTree(初期配列)
    LazySegmentTree(const vector<T>& v) : n((int)v.size()) {
        size = 1;
        log = 0;

        while (size < n) {
            size *= 2;
            log++;
        }

        data.assign(2 * size, e());
        lazy.assign(size, id());

        // 葉に初期値を設定
        for (int i = 0; i < n; i++) {
            data[size + i] = v[i];
        }

        // 親を構築
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    // 更新: set(位置, 値)
    void set(int pos, T value) {
        assert(0 <= pos && pos < n);

        pos += size;

        // 対象位置まで遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            push(pos >> i);
        }

        data[pos] = value;

        // 根まで更新
        for (int i = 1; i <= log; i++) {
            update(pos >> i);
        }
    }

    // 取得: get(位置) -> 現在値
    T get(int pos) {
        assert(0 <= pos && pos < n);

        pos += size;

        // 対象位置まで遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            push(pos >> i);
        }

        return data[pos];
    }

    // 区間取得: prod(左端, 右端) -> [left, right)
    T prod(int left, int right) {
        assert(0 <= left && left <= right && right <= n);

        if (left == right) return e();

        left += size;
        right += size;

        // 左右の境界に必要な遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            if (((left >> i) << i) != left) {
                push(left >> i);
            }

            if (((right >> i) << i) != right) {
                push((right - 1) >> i);
            }
        }

        T left_value = e();
        T right_value = e();

        while (left < right) {
            if (left % 2 == 1) {
                left_value = op(left_value, data[left]);
                left++;
            }

            if (right % 2 == 1) {
                right--;
                right_value = op(data[right], right_value);
            }

            left /= 2;
            right /= 2;
        }

        return op(left_value, right_value);
    }

    // 全区間取得: all_prod() -> [0, n)
    T all_prod() {
        return data[1];
    }

    // 作用: apply(位置, 作用)
    void apply(int pos, F f) {
        assert(0 <= pos && pos < n);

        pos += size;

        // 対象位置まで遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            push(pos >> i);
        }

        data[pos] = mapping(f, data[pos]);

        // 根まで更新
        for (int i = 1; i <= log; i++) {
            update(pos >> i);
        }
    }

    // 区間作用: apply(左端, 右端, 作用) -> [left, right)
    void apply(int left, int right, F f) {
        assert(0 <= left && left <= right && right <= n);

        if (left == right) return;

        left += size;
        right += size;

        // 左右の境界に必要な遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            if (((left >> i) << i) != left) {
                push(left >> i);
            }

            if (((right >> i) << i) != right) {
                push((right - 1) >> i);
            }
        }

        int left_copy = left;
        int right_copy = right;

        // 区間に作用を適用
        while (left < right) {
            if (left % 2 == 1) {
                all_apply(left, f);
                left++;
            }

            if (right % 2 == 1) {
                right--;
                all_apply(right, f);
            }

            left /= 2;
            right /= 2;
        }

        left = left_copy;
        right = right_copy;

        // 左右の境界に関係するノードを再計算
        for (int i = 1; i <= log; i++) {
            if (((left >> i) << i) != left) {
                update(left >> i);
            }

            if (((right >> i) << i) != right) {
                update((right - 1) >> i);
            }
        }
    }

    // 二分探索: max_right(左端, 判定関数)
    // g(prod(left, right)) が true となる最大の right を返す
    // 前提: g(e()) == true、判定に単調性がある
    template<class G>
    int max_right(int left, G g) {
        assert(0 <= left && left <= n);
        assert(g(e()));

        if (left == n) return n;

        left += size;

        // 探索開始位置まで遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            push(left >> i);
        }

        T value = e();

        do {
            while (left % 2 == 0) {
                left /= 2;
            }

            if (!g(op(value, data[left]))) {
                while (left < size) {
                    push(left);
                    left *= 2;

                    if (g(op(value, data[left]))) {
                        value = op(value, data[left]);
                        left++;
                    }
                }

                return left - size;
            }

            value = op(value, data[left]);
            left++;

        } while ((left & -left) != left);

        return n;
    }

    // 二分探索: min_left(右端, 判定関数)
    // g(prod(left, right)) が true となる最小の left を返す
    // 前提: g(e()) == true、判定に単調性がある
    template<class G>
    int min_left(int right, G g) {
        assert(0 <= right && right <= n);
        assert(g(e()));

        if (right == 0) return 0;

        right += size;

        // 探索開始位置まで遅延作用を伝播
        for (int i = log; i >= 1; i--) {
            push((right - 1) >> i);
        }

        T value = e();

        do {
            right--;

            while (right > 1 && right % 2 == 1) {
                right /= 2;
            }

            if (!g(op(data[right], value))) {
                while (right < size) {
                    push(right);
                    right = 2 * right + 1;

                    if (g(op(data[right], value))) {
                        value = op(data[right], value);
                        right--;
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

int MOD = 998244353;

struct S {
    long long sum;
    long long len;
};

S op(S a, S b) {
    return {(a.sum + b.sum) % MOD, (a.len + b.len)};
}

S e() {
    return {0LL, 0LL};
}

struct F {
    long long a;
    long long b;
};

S mapping(F f, S x) {
    return {(f.a * x.sum + f.b * x.len) % MOD, x.len};
}

F composition(F g, F f) {
    return {(f.a * g.a) % MOD, (f.b * g.a + g.b) % MOD};
}

F id() {
    return {1LL, 0LL};
}

void yosupo_judge_Range_Affine_Range_Sum() {
    int N, Q;
    cin >> N >> Q;
    vector<S> a(N);
    for (int i = 0; i < N; i ++) {
        int x;
        cin >> x;
        a[i] = {x, 1LL};
    }
    LazySegmentTree<S, op, e, F, mapping, composition, id> seg(a);
    for (int q = 0; q < Q; q ++) {
        int query;
        cin >> query;
        if (query == 0) {
            int l, r;
            long long b, c;
            cin >> l >> r >> b >> c;
            seg.apply(l, r, {b, c});
        } else {
            int l, r;
            cin >> l >> r;
            cout << seg.prod(l, r).sum << endl;
        }
    }
}

int main() {
    yosupo_judge_Range_Affine_Range_Sum();
}