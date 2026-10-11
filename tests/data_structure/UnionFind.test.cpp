#include "library/data_structure/UnionFind.cpp"

// UnionFindと異なる方法で、追加済みの辺から成分を再計算する。
vector<vector<int>> reference_groups(const vector<vector<int>>& graph) {
    int n = (int)graph.size();
    vector<bool> seen(n, false);
    vector<vector<int>> result;
    for (int start = 0; start < n; ++start) {
        if (seen[start]) continue;
        vector<int> group;
        queue<int> queue;
        queue.push(start);
        seen[start] = true;
        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            group.push_back(v);
            for (int to : graph[v]) {
                if (seen[to]) continue;
                seen[to] = true;
                queue.push(to);
            }
        }
        sort(group.begin(), group.end());
        result.push_back(group);
    }
    sort(result.begin(), result.end());
    return result;
}

void verify(UnionFind& uf, const vector<vector<int>>& graph) {
    auto expected = reference_groups(graph);
    auto actual = uf.groups();
    for (auto& group : actual) sort(group.begin(), group.end());
    sort(actual.begin(), actual.end());
    assert(actual == expected);
    assert(uf.count() == (int)expected.size());
    vector<int> component(uf.n);
    for (int i = 0; i < (int)expected.size(); ++i) {
        for (int v : expected[i]) {
            component[v] = i;
            assert(uf.size(v) == (int)expected[i].size());
            assert(uf.leader(v) == uf.leader(expected[i][0]));
            assert(uf.leader(uf.leader(v)) == uf.leader(v));
        }
    }
    for (int u = 0; u < uf.n; ++u) {
        for (int v = 0; v < uf.n; ++v) {
            assert(uf.same(u, v) == (component[u] == component[v]));
        }
    }
}

int main() {
    mt19937 rng(20261012);
    for (int n : {0, 1, 2, 7, 32}) {
        UnionFind uf(n);
        vector<vector<int>> graph(n);
        verify(uf, graph);
        for (int step = 0; n > 0 && step < 200; ++step) {
            int u = (int)(rng() % n);
            int v = (int)(rng() % n);
            int leader = uf.merge(u, v);
            assert(leader == uf.leader(u));
            assert(leader == uf.leader(v));
            graph[u].push_back(v);
            graph[v].push_back(u);
            verify(uf, graph);
        }
    }
    cout << "UnionFind: BFS reference comparison passed\n";
}
