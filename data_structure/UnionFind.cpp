#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
    int n;
    int group_count;
    vector<int> parent_or_size;

    // 初期化: UnionFind(頂点数) -> 各頂点が独立した連結成分の状態で構築
    UnionFind(int n) : n(n), group_count(n) {
        assert(n >= 0);
        parent_or_size.assign(n, -1);
    }

    // 代表元: leader(頂点v) -> v が属する連結成分の代表元
    int leader(int v) {
        assert(0 <= v && v < n);
        if (parent_or_size[v] < 0) return v;
        return parent_or_size[v] = leader(parent_or_size[v]);
    }

    // 結合: merge(頂点u, 頂点v) -> 結合後の連結成分の代表元
    int merge(int u, int v) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        u = leader(u);
        v = leader(v);
        if (u == v) return u;

        // サイズの大きい連結成分を親にする
        if (-parent_or_size[u] < -parent_or_size[v]) swap(u, v);
        parent_or_size[u] += parent_or_size[v];
        parent_or_size[v] = u;
        --group_count;
        return u;
    }

    // 判定: same(頂点u, 頂点v) -> 同じ連結成分なら true
    bool same(int u, int v) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        return leader(u) == leader(v);
    }

    // サイズ: size(頂点v) -> v が属する連結成分の頂点数
    int size(int v) {
        assert(0 <= v && v < n);
        return -parent_or_size[leader(v)];
    }

    // 連結成分数: count() -> 現在の連結成分数
    int count() const {
        return group_count;
    }

    // 連結成分一覧: groups() -> 各連結成分に属する頂点一覧
    vector<vector<int>> groups() {
        vector<vector<int>> result(n);

        for (int v = 0; v < n; ++v) {
            result[leader(v)].push_back(v);
        }

        vector<vector<int>> groups;
        for (int v = 0; v < n; ++v) {
            if (!result[v].empty()) {
                groups.push_back(move(result[v]));
            }
        }

        return groups;
    }
};

// examples

void yosupo_judge_UnionFind() {
    int N, Q;
    cin >> N >> Q;
    UnionFind uf(N);
    for (int q = 0; q < Q; q ++) {
        int query, u, v;
        cin >> query >> u >> v;
        if (query == 0) {
            uf.merge(u, v);
        } else {
            if (uf.same(u, v)) cout << 1 << endl;
            else cout << 0 << endl;
        }
    }
}

int main() {
    yosupo_judge_UnionFind();
}