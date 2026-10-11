#include "library/data_structure/OrderedSet.cpp"

template<class Compare>
void check_set(Compare compare) {
    OrderedSet<int, Compare> actual(compare);
    set<int, Compare> expected(compare);
    mt19937 rng(20261012);
    for (int step = 0; step < 5000; ++step) {
        int value = (int)(rng() % 101) - 50;
        if (rng() % 2) assert(actual.insert(value) == expected.insert(value).second);
        else assert(actual.erase(value) == (expected.erase(value) != 0));
        assert(actual.size() == (int)expected.size());
        assert(actual.empty() == expected.empty());
        assert(actual.contains(value) == expected.contains(value));
        auto it = actual.begin();
        int index = 0;
        for (int x : expected) {
            assert(it != actual.end() && *it == x && *it.operator->() == x);
            assert(actual.kth(index++) == x);
            auto old = it++;
            assert(*old == x);
        }
        assert(it == actual.end());
        for (auto reverse = expected.rbegin(); reverse != expected.rend(); ++reverse) {
            --it;
            assert(*it == *reverse);
        }
        for (int query : {-60, value, 60}) {
            int rank = 0;
            for (int x : expected) rank += compare(x, query);
            assert(actual.order_of(query) == rank);
            auto compare_iterator = [&](auto got, auto want) {
                assert((got == actual.end()) == (want == expected.end()));
                if (want != expected.end()) assert(*got == *want);
            };
            compare_iterator(actual.find(query), expected.find(query));
            compare_iterator(actual.lower_bound(query), expected.lower_bound(query));
            compare_iterator(actual.upper_bound(query), expected.upper_bound(query));
        }
    }
}

int main() {
    check_set(less<int>{});
    check_set(greater<int>{});
    cout << "OrderedSet: std::set reference, both orders, and iterators passed\n";
}
