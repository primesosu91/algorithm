#include "data_structure/LazySegmentTree.cpp"

struct Sum {
    long long value;
    int len;
};

struct Affine {
    long long mul;
    long long add;
};

Sum op(Sum a, Sum b) { return {a.value + b.value, a.len + b.len}; }
Sum e() { return {0, 0}; }
Sum mapping(Affine f, Sum x) { return {f.mul * x.value + f.add * x.len, x.len}; }
Affine composition(Affine f, Affine g) {
    return {f.mul * g.mul, f.mul * g.add + f.add};
}
Affine id() { return {1, 0}; }

int main() {
    mt19937 rng(20261012);
    using Tree = LazySegmentTree<Sum, op, e, Affine, mapping, composition, id>;
    Tree zero(0);
    assert(zero.all_prod().value == 0);
    assert(zero.prod(0, 0).len == 0);
    zero.apply(0, 0, {0, 9});

    // gの後にfを適用する順序は、代入と加算を重ねると区別できる。
    Tree order(vector<Sum>(4, {1, 1}));
    order.apply(0, 4, {0, 7});
    order.apply(0, 4, {1, 3});
    assert(order.get(2).value == 10);
    order.apply(0, 4, {1, 5});
    order.apply(0, 4, {0, 2});
    assert(order.get(1).value == 2);

    for (int n : {0, 1, 2, 7, 16, 31}) {
        vector<long long> values(n, 0);
        Tree tree(vector<Sum>(n, {0, 1}));
        for (int step = 0; step < 1000; ++step) {
            int left = (int)(rng() % (n + 1));
            int right = (int)(rng() % (n + 1));
            if (left > right) swap(left, right);
            int operation = (int)(rng() % 4);
            Affine f{(long long)(rng() % 2), (long long)(rng() % 10)};
            if (operation == 0) {
                tree.apply(left, right, f);
                for (int i = left; i < right; ++i) values[i] = f.mul * values[i] + f.add;
            } else if (operation == 1 && n) {
                int pos = (int)(rng() % n);
                tree.apply(pos, f);
                values[pos] = f.mul * values[pos] + f.add;
            } else if (operation == 2 && n) {
                int pos = (int)(rng() % n);
                values[pos] = rng() % 30;
                tree.set(pos, {values[pos], 1});
            }
            long long expected = accumulate(values.begin() + left,
                                            values.begin() + right, 0LL);
            assert(tree.prod(left, right).value == expected);
            assert(tree.prod(left, right).len == right - left);
            assert(tree.prod(left, left).value == 0);
            assert(tree.all_prod().value == accumulate(values.begin(), values.end(), 0LL));
            assert(tree.all_prod().len == n);

            long long limit = rng() % 100;
            auto predicate = [limit](Sum value) { return value.value <= limit; };
            int end = left;
            long long sum = 0;
            while (end < n && sum + values[end] <= limit) sum += values[end++];
            assert(tree.max_right(left, predicate) == end);
            int begin = right;
            sum = 0;
            while (begin > 0 && sum + values[begin - 1] <= limit) sum += values[--begin];
            assert(tree.min_left(right, predicate) == begin);
            if (n) {
                int pos = (int)(rng() % n);
                assert(tree.get(pos).value == values[pos]);
                assert(tree.get(pos).len == 1);
            }
        }
    }
    cout << "LazySegmentTree: affine composition and array reference checks passed\n";
}
