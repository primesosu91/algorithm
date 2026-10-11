#include "graph/ProjectSelection.cpp"

int main() {
    mt19937 rng(20261012);
    for (int trial = 0; trial < 1000; ++trial) {
        int n = rng() % 9;
        ProjectSelection problem(n);
        for (int v = 0; v < n; ++v) {
            problem.add_profit(v, (int)(rng() % 21) - 10);
            problem.add_profit(v, (int)(rng() % 7) - 3);
        }
        for (int i = 0; n && i < 12; ++i) {
            if (rng() % 3) problem.add_penalty(rng() % n, rng() % n, rng() % 11);
            else problem.add_dependency(rng() % n, rng() % n);
        }
        auto evaluate = [&](int mask) {
            for (auto [u, v] : problem.dependencies) {
                if ((mask >> u & 1) && !(mask >> v & 1)) return numeric_limits<long long>::min();
            }
            long long value = 0;
            for (int v = 0; v < n; ++v) if (mask >> v & 1) value += problem.profit[v];
            for (auto e : problem.penalties) {
                if ((mask >> e.from & 1) && !(mask >> e.to & 1)) value -= e.penalty;
            }
            return value;
        };
        long long expected = 0;
        for (int mask = 0; mask < (1 << n); ++mask) expected = max(expected, evaluate(mask));
        assert(problem.solve() == expected);
        auto chosen = problem.selected();
        int mask = 0;
        for (int v = 0; v < n; ++v) if (chosen[v]) mask |= 1 << v;
        assert(evaluate(mask) == expected);
    }
    cout << "ProjectSelection: exhaustive subsets and restored choices passed\n";
}
