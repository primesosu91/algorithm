#include "library/data_structure/SegmentTree.cpp"

long long sum_op(long long a, long long b) { return a + b; }
long long sum_e() { return 0; }
string concat_op(string a, string b) { return a + b; }
string concat_e() { return ""; }

int main() {
    mt19937 rng(20261012);
    for (int n : {0, 1, 2, 7, 16, 31}) {
        vector<long long> values(n, 0);
        SegmentTree<long long, sum_op, sum_e> tree(values);
        SegmentTree<long long, sum_op, sum_e> empty_initial(n);
        assert(empty_initial.all_prod() == 0);
        for (int step = 0; step < 500; ++step) {
            if (n && rng() % 2) {
                int pos = (int)(rng() % n);
                values[pos] = rng() % 25;
                tree.set(pos, values[pos]);
                assert(tree.get(pos) == values[pos]);
            }
            int left = (int)(rng() % (n + 1));
            int right = (int)(rng() % (n + 1));
            if (left > right) swap(left, right);
            long long expected = accumulate(values.begin() + left,
                                            values.begin() + right, 0LL);
            assert(tree.prod(left, right) == expected);
            assert(tree.prod(left, left) == 0);
            assert(tree.all_prod() == accumulate(values.begin(), values.end(), 0LL));

            long long limit = rng() % 100;
            auto predicate = [limit](long long value) { return value <= limit; };
            int end = left;
            long long sum = 0;
            while (end < n && sum + values[end] <= limit) sum += values[end++];
            assert(tree.max_right(left, predicate) == end);
            int begin = right;
            sum = 0;
            while (begin > 0 && sum + values[begin - 1] <= limit) sum += values[--begin];
            assert(tree.min_left(right, predicate) == begin);
        }

        // 和では検出できない左右の順序を、非可換な文字列連結で確認する。
        vector<string> characters;
        string whole;
        for (int i = 0; i < n; ++i) {
            characters.push_back(string(1, 'a' + i % 26));
            whole += characters.back();
        }
        SegmentTree<string, concat_op, concat_e> strings(characters);
        for (int left = 0; left <= n; ++left) {
            for (int right = left; right <= n; ++right) {
                assert(strings.prod(left, right) == whole.substr(left, right - left));
            }
            string suffix = whole.substr(left);
            auto prefix_matches = [suffix](string value) {
                return value.size() <= suffix.size() && suffix.starts_with(value);
            };
            assert(strings.max_right(left, prefix_matches) == n);
        }
        for (int right = 0; right <= n; ++right) {
            string prefix = whole.substr(0, right);
            auto suffix_matches = [prefix](string value) {
                return value.size() <= prefix.size() && prefix.ends_with(value);
            };
            assert(strings.min_left(right, suffix_matches) == 0);
        }
    }
    cout << "SegmentTree: array, boundary search, and noncommutative checks passed\n";
}
