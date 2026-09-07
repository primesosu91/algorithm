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

void ABC_049_D() {
    int N, K, L;
    cin >> N >> K >> L;
    UnionFind uf_road(N), uf_train(N);
    for (int i = 0; i < K; i ++) {
        int p, q;
        cin >> p >> q;
        p --, q --;
        uf_road.merge(p, q);
    }
    for (int i = 0; i < L; i ++) {
        int r, s;
        cin >> r >> s;
        s --, r --;
        uf_train.merge(r, s);
    }
    map<pair<int, int>, int> mp;
    for (int i = 0; i < N; i ++) {
        int r_road = uf_road.leader(i);
        int r_train = uf_train.leader(i);
        mp[{r_road, r_train}] ++;
    }
    for (int i = 0; i < N; i ++) {
        int r_road = uf_road.leader(i);
        int r_train = uf_train.leader(i);
        cout << mp[{r_road, r_train}] << endl;
        if (i < N - 1) cout << " ";
        else cout << endl;
    }
}

void ABC_380_E() {
    int N, Q;
    cin >> N >> Q;
    UnionFind uf(N);
    vector<int> col(N);
    vector<int> num(N, 1);
    vector<pair<int, int>> I(N);
    for (int q = 0; q < N; q ++) {
        I[q] = {q, q+1};
    }
    for (int i = 0; i < N; i ++) col[i] = i;
    for (int q = 0; q < Q; q ++) {
        int query;
        cin >> query;
        if (query == 1) {
            int x, c;
            cin >> x >> c;
            x --, c --;
            int root = uf.leader(x);
            num[col[root]] -= uf.size(root);
            col[root] = c;
            num[c] += uf.size(root);
            if (I[root].first > 0) {
                int root_left = uf.leader(I[root].first-1);
                if (col[root] == col[root_left]) {
                    int left = I[root_left].first;
                    int right = I[root].second;
                    int nv = uf.merge(root, root_left);
                    I[nv] = {left, right};
                    col[nv] = col[root];
                }
            }
            root = uf.leader(x);
            if (I[root].second < N) {
                int root_right = uf.leader(I[root].second);
                if (col[root] == col[root_right]) {
                    int left = I[root].first;
                    int right = I[root_right].second;
                    int nv = uf.merge(root, root_right);
                    I[nv] = {left, right};
                    col[nv] = col[root];
                }
            }
        } else {
            int c;
            cin >> c;
            c --;
            cout << num[c] << endl;
        }
    }
}

int main() {
    yosupo_judge_UnionFind();
    // ABC_049_D(); 道路・鉄道のどちらでも行ける都市の数
    // ABC_380_E(); 左右の区間と同じ色なら区間をマージ
}