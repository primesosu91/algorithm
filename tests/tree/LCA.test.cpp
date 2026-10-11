#include "library/tree/LCA.cpp"

int main() {
    mt19937 rng(20261012);
    for (int trial = 0; trial < 100; ++trial) {
        int n = 1 + rng() % 80;
        LCA tree(n);
        vector<vector<int>> graph(n);
        for (int v = 1; v < n; ++v) {
            int p = rng() % v;
            tree.add_edge(p, v);
            graph[p].push_back(v); graph[v].push_back(p);
        }
        for (int repeat = 0; repeat < 3; ++repeat) {
            int root = rng() % n;
            tree.build(root);
            vector<int> parent(n, -1), depth(n, -1);
            queue<int> que;
            depth[root] = 0; que.push(root);
            while (!que.empty()) {
                int v = que.front(); que.pop();
                for (int to : graph[v]) if (depth[to] == -1) {
                    parent[to] = v; depth[to] = depth[v] + 1; que.push(to);
                }
            }
            for (int v = 0; v < n; ++v) {
                int ancestor = v;
                for (int k = 0; k <= depth[v] + 1; ++k) {
                    assert(tree.kth_ancestor(v, k) == ancestor);
                    if (ancestor != -1) ancestor = parent[ancestor];
                }
            }
            for (int query = 0; query < 100; ++query) {
                int u = rng() % n, v = rng() % n;
                int a = u, b = v;
                while (depth[a] > depth[b]) a = parent[a];
                while (depth[b] > depth[a]) b = parent[b];
                while (a != b) { a = parent[a]; b = parent[b]; }
                assert(tree.lca(u, v) == a);
                vector<int> left, right;
                for (int x = u; x != a; x = parent[x]) left.push_back(x);
                left.push_back(a);
                for (int x = v; x != a; x = parent[x]) right.push_back(x);
                reverse(right.begin(), right.end());
                left.insert(left.end(), right.begin(), right.end());
                assert(tree.distance(u, v) == (int)left.size() - 1);
                for (int k = 0; k < (int)left.size(); ++k) assert(tree.jump(u, v, k) == left[k]);
                assert(tree.jump(u, v, left.size()) == -1);
            }
        }
    }
    cout << "LCA: parent walking, complete path jumps, and different roots passed\n";
}
