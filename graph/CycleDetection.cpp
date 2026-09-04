#include <bits/stdc++.h>
using namespace std;

struct CycleDetection {
    int n;
    bool directed;
    int edge_count;
    vector<vector<pair<int, int>>> graph;

    bool searched;
    bool cycle_exists;
    vector<int> cycle_vertices;
    vector<int> cycle_edges;

    // 初期化: CycleDetection(頂点数, 有向グラフならtrue)
    CycleDetection(int n, bool directed)
        : n(n),
          directed(directed),
          edge_count(0),
          searched(false),
          cycle_exists(false) {
        assert(n >= 0);
        graph.resize(n);
    }

    // 辺追加: add_edge(頂点u, 頂点v) -> 追加した辺ID
    int add_edge(int u, int v) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        assert(edge_count < numeric_limits<int>::max());

        int edge_id = edge_count++;

        graph[u].push_back({v, edge_id});

        if (!directed) {
            graph[v].push_back({u, edge_id});
        }

        searched = false;
        cycle_vertices.clear();
        cycle_edges.clear();

        return edge_id;
    }

    // サイクル判定: has_cycle() -> サイクルが存在すれば true
    bool has_cycle() {
        search();
        return cycle_exists;
    }

    // サイクルの頂点列: get_cycle_vertices() -> 辺数と同じ長さの頂点列
    vector<int> get_cycle_vertices() {
        search();
        return cycle_vertices;
    }

    // サイクルの辺ID列: get_cycle_edges() -> 頂点列に対応する辺ID列
    vector<int> get_cycle_edges() {
        search();
        return cycle_edges;
    }

private:
    // サイクルを復元
    void restore_cycle(
        int from,
        int to,
        int closing_edge,
        const vector<int>& parent_vertex,
        const vector<int>& parent_edge
    ) {
        vector<int> vertices;
        vector<int> edges;

        int v = from;
        vertices.push_back(v);

        while (v != to) {
            edges.push_back(parent_edge[v]);
            v = parent_vertex[v];
            vertices.push_back(v);
        }

        reverse(vertices.begin(), vertices.end());
        reverse(edges.begin(), edges.end());

        edges.push_back(closing_edge);

        cycle_vertices = move(vertices);
        cycle_edges = move(edges);
    }

    // サイクルを探索
    void search() {
        if (searched) return;

        searched = true;
        cycle_exists = false;
        cycle_vertices.clear();
        cycle_edges.clear();

        // 0: 未訪問, 1: 探索中, 2: 探索完了
        vector<int> state(n, 0);
        vector<int> parent_vertex(n, -1);
        vector<int> parent_edge(n, -1);
        vector<int> next_index(n, 0);

        for (int start = 0; start < n; ++start) {
            if (state[start] != 0) continue;

            vector<int> stack;
            stack.push_back(start);
            state[start] = 1;

            while (!stack.empty()) {
                int u = stack.back();

                // 頂点uから出る辺をすべて調べ終えた
                if (next_index[u] == (int)graph[u].size()) {
                    state[u] = 2;
                    stack.pop_back();
                    continue;
                }

                auto [v, edge_id] = graph[u][next_index[u]++];

                // 無向グラフでは、来たときに使った同じ辺だけ無視する
                if (!directed && edge_id == parent_edge[u]) {
                    continue;
                }

                if (state[v] == 0) {
                    parent_vertex[v] = u;
                    parent_edge[v] = edge_id;

                    state[v] = 1;
                    stack.push_back(v);
                } else if (state[v] == 1) {
                    restore_cycle(
                        u,
                        v,
                        edge_id,
                        parent_vertex,
                        parent_edge
                    );

                    cycle_exists = true;
                    return;
                }
            }
        }
    }
};

// examples

void yosupo_judge_Cycle_Detection_Directed() {
    int N, M;
    cin >> N >> M;
    CycleDetection g(N, true);
    for (int i = 0; i < M; i ++) {
        int u, v;
        cin >> u >> v;
        g.add_edge(u, v);
    }
    bool ok = g.has_cycle();
    if (!ok) {
        cout << -1 << endl;
        return;
    }
    vector<int> list = g.get_cycle_edges();
    cout << list.size() << endl;
    for (int i = 0; i < (int) list.size(); i ++) {
        cout << list[i] << endl;
    }
}

void yosupo_judge_Cycle_Detection_Undirected() {
    int N, M;
    cin >> N >> M;
    CycleDetection g(N, false);
    for (int i = 0; i < M; i ++) {
        int u, v;
        cin >> u >> v;
        g.add_edge(u, v);
    }
    bool ok = g.has_cycle();
    if (!ok) {
        cout << -1 << endl;
        return;
    }
    vector<int> list_v = g.get_cycle_vertices();
    vector<int> list_e = g.get_cycle_edges();
    cout << list_v.size() << endl;
    for (int i = 0; i < (int) list_v.size(); i ++) {
        cout << list_v[i];
        if (i != (int) list_v.size() - 1) cout << " ";
        else cout << endl;
    }
    for (int i = 0; i < (int) list_e.size(); i ++) {
        cout << list_e[i];
        if (i != (int) list_e.size() - 1) cout << " ";
        else cout << endl;
    }
}

int main() {
    yosupo_judge_Cycle_Detection_Directed();
    // yosupo_judge_Cycle_Detection_Undirected();
}