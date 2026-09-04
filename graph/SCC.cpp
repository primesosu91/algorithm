#include <bits/stdc++.h>
using namespace std;

struct SCC {
    int n;
    vector<vector<int>> graph;
    vector<vector<int>> reverse_graph;

    bool built;
    vector<int> component_ids;
    vector<vector<int>> component_groups;

    // 初期化: SCC(頂点数)
    SCC(int n)
        : n(n),
          graph(n),
          reverse_graph(n),
          built(false),
          component_ids(n, -1) {
        assert(n >= 0);
    }

    // 辺追加: add_edge(始点, 終点)
    void add_edge(int from, int to) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        graph[from].push_back(to);
        reverse_graph[to].push_back(from);

        built = false;
    }

    // 強連結成分分解を実行
    void build() {
        vector<bool> used(n, false);
        vector<int> next_index(n, 0);
        vector<int> order;
        order.reserve(n);

        // 元グラフで帰りがけ順を求める
        for (int start = 0; start < n; ++start) {
            if (used[start]) continue;

            vector<int> stack;
            stack.push_back(start);
            used[start] = true;

            while (!stack.empty()) {
                int v = stack.back();

                if (next_index[v] == (int)graph[v].size()) {
                    order.push_back(v);
                    stack.pop_back();
                    continue;
                }

                int to = graph[v][next_index[v]++];

                if (!used[to]) {
                    used[to] = true;
                    stack.push_back(to);
                }
            }
        }

        component_ids.assign(n, -1);
        component_groups.clear();

        // 逆グラフを帰りがけ順の逆順に探索する
        for (int i = n - 1; i >= 0; --i) {
            int start = order[i];

            if (component_ids[start] != -1) continue;

            int id = (int)component_groups.size();
            component_groups.push_back({});

            vector<int> stack;
            stack.push_back(start);
            component_ids[start] = id;

            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();

                component_groups[id].push_back(v);

                for (int to : reverse_graph[v]) {
                    if (component_ids[to] != -1) continue;

                    component_ids[to] = id;
                    stack.push_back(to);
                }
            }
        }

        built = true;
    }

    // 強連結成分ID: component_id(頂点v) -> v が属する強連結成分ID
    int component_id(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        return component_ids[v];
    }

    // 強連結成分数: count() -> 強連結成分数
    int count() const {
        assert(built);

        return (int)component_groups.size();
    }

    // 強連結成分一覧: groups() -> 各強連結成分に属する頂点一覧
    const vector<vector<int>>& groups() const {
        assert(built);

        return component_groups;
    }
};

// examples

void yosupo_judge_Strongly_Connected_Components() {
    int N, M;
    cin >> N >> M;
    SCC g(N);
    for (int i = 0; i < M; i ++) {
        int a, b;
        cin >> a >> b;
        g.add_edge(a, b);
    }
    g.build();
    cout << g.count() << endl;
    vector<vector<int>> list = g.groups();
    for (int i = 0; i < (int) list.size(); i ++) {
        cout << list[i].size();
        for (int v : list[i]) {
            cout << " " << v;
        }
        cout << endl;
    }
}

int main() {
    yosupo_judge_Strongly_Connected_Components();
}