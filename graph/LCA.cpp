#include <bits/stdc++.h>
using namespace std;

struct LCA {
    int n;
    int log;
    vector<vector<int>> graph;
    vector<vector<int>> parent;
    vector<int> depth;

    // 初期化: LCA(頂点数)
    LCA(int n) : n(n), graph(n), depth(n, -1) {
        assert(n > 0);

        log = 1;
        while ((1LL << log) <= n) {
            log++;
        }

        parent.assign(log, vector<int>(n, -1));
    }

    // 辺追加: add_edge(頂点u, 頂点v)
    void add_edge(int u, int v) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // 構築: build(根) -> 木の祖先表を計算、辺の追加後は再構築する
    void build(int root) {
        assert(0 <= root && root < n);

        // 根からBFSして深さと1個上の親を求める
        fill(depth.begin(), depth.end(), -1);
        for (int k = 0; k < log; k++) {
            fill(parent[k].begin(), parent[k].end(), -1);
        }

        queue<int> que;
        depth[root] = 0;
        que.push(root);

        while (!que.empty()) {
            int v = que.front();
            que.pop();

            for (int next : graph[v]) {
                if (next == parent[0][v]) continue;

                parent[0][next] = v;
                depth[next] = depth[v] + 1;
                que.push(next);
            }
        }

        // parent[k][v] = vから2^k個上の祖先
        for (int k = 1; k < log; k++) {
            for (int v = 0; v < n; v++) {
                int p = parent[k - 1][v];

                if (p != -1) {
                    parent[k][v] = parent[k - 1][p];
                }
            }
        }
    }

    // k個上の祖先: kth_ancestor(頂点v, 個数k) -> 存在しない場合 -1
    int kth_ancestor(int v, int k) const {
        assert(0 <= v && v < n);
        assert(k >= 0);
        assert(depth[v] != -1);

        if (k > depth[v]) return -1;

        for (int bit = 0; bit < log; bit++) {
            if ((k >> bit) & 1) {
                v = parent[bit][v];
            }
        }

        return v;
    }

    // 最小共通祖先: lca(頂点u, 頂点v)
    int lca(int u, int v) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        assert(depth[u] != -1);
        assert(depth[v] != -1);

        // 深さを揃える
        if (depth[u] < depth[v]) {
            swap(u, v);
        }

        u = kth_ancestor(u, depth[u] - depth[v]);

        if (u == v) return u;

        // LCAの直下まで同時に持ち上げる
        for (int k = log - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }

        return parent[0][u];
    }

    // 距離: distance(頂点u, 頂点v) -> u-v間の辺数
    int distance(int u, int v) const {
        int p = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[p];
    }

    // パス上の頂点: jump(頂点u, 頂点v, uから進む辺数k) -> 存在しない場合 -1
    int jump(int u, int v, int k) const {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        assert(k >= 0);
        assert(depth[u] != -1);
        assert(depth[v] != -1);

        int p = lca(u, v);
        int up = depth[u] - depth[p];
        int dist = up + depth[v] - depth[p];

        if (k > dist) return -1;

        if (k <= up) {
            return kth_ancestor(u, k);
        }

        return kth_ancestor(v, dist - k);
    }
};

// examples

void yosupo_judge_Lowest_Common_Ancestor() {
    int N, Q;
    cin >> N >> Q;
    LCA g(N);
    for (int i = 1; i < N; i ++) {
        int p;
        cin >> p;
        g.add_edge(i, p);
    }
    g.build(0);
    for (int q = 0; q < Q; q ++) {
        int u, v;
        cin >> u >> v;
        cout << g.lca(u, v) << endl;
    }
}

void yosupo_judge_Jump_on_Tree() {
    int N, Q;
    cin >> N >> Q;
    LCA g(N);
    for (int i = 0; i < N - 1; i ++) {
        int a, b;
        cin >> a >> b;
        g.add_edge(a, b);
    }
    g.build(0);
    for (int q = 0; q < Q; q ++) {
        int s, t, i;
        cin >> s >> t >> i;
        cout << g.jump(s, t, i) << endl;
    }
}

int main() {
    yosupo_judge_Lowest_Common_Ancestor();
    // yosupo_judge_Jump_on_Tree();
}