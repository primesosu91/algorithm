#include <bits/stdc++.h>
using namespace std;

struct TreeDiameter {
    struct Edge {
        int to;
        long long weight;
        int id;
    };

    int n;
    vector<vector<Edge>> graph;

    int edge_count;
    bool built;

    long long diameter_dist;
    pair<int, int> diameter_ends;
    vector<int> diameter_path_vertices;
    vector<int> diameter_path_edges;

    vector<int> parent;
    vector<int> parent_edge;

    // 初期化: TreeDiameter(頂点数)
    TreeDiameter(int n)
        : n(n),
          graph(n),
          edge_count(0),
          built(false),
          diameter_dist(0),
          diameter_ends({0, 0}) {
        assert(n >= 1);
    }

    // 辺追加: add_edge(頂点u, 頂点v) -> 重み1の無向辺
    void add_edge(int u, int v) {
        add_edge(u, v, 1);
    }

    // 辺追加: add_edge(頂点u, 頂点v, 重み) -> 重みは正
    void add_edge(int u, int v, long long weight) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        assert(weight > 0);

        graph[u].push_back({v, weight, edge_count});
        graph[v].push_back({u, weight, edge_count});

        edge_count++;
        built = false;
    }

    // 構築: build() -> 連結な木の直径と復元情報を計算
    // 前提: パスの重みの和が long long の範囲に収まる
    void build() {
        if (n == 1) {
            diameter_dist = 0;
            diameter_ends = {0, 0};
            diameter_path_vertices = {0};
            diameter_path_edges.clear();
            built = true;
            return;
        }

        auto first = farthest_vertex(0, false);
        int start = first.first;

        auto second = farthest_vertex(start, true);
        int goal = second.first;

        diameter_dist = second.second;
        diameter_ends = {start, goal};

        diameter_path_vertices.clear();
        diameter_path_edges.clear();

        int current = goal;

        while (current != start) {
            diameter_path_vertices.push_back(current);
            diameter_path_edges.push_back(parent_edge[current]);
            current = parent[current];
        }

        diameter_path_vertices.push_back(start);

        reverse(diameter_path_vertices.begin(), diameter_path_vertices.end());
        reverse(diameter_path_edges.begin(), diameter_path_edges.end());

        built = true;
    }

    // 直径長: diameter_length() -> 木の直径の長さ
    long long diameter_length() const {
        assert(built);
        return diameter_dist;
    }

    // 両端点: diameter_endpoints() -> 直径の両端点
    pair<int, int> diameter_endpoints() const {
        assert(built);
        return diameter_ends;
    }

    // 頂点列: diameter_vertices() -> 直径上の頂点を順番に返す
    const vector<int>& diameter_vertices() const {
        assert(built);
        return diameter_path_vertices;
    }

    // 辺列: diameter_edges() -> 直径上の辺番号を順番に返す
    const vector<int>& diameter_edges() const {
        assert(built);
        return diameter_path_edges;
    }

private:
    // 最遠点探索: farthest_vertex(開始頂点, 親情報を保存するか)
    pair<int, long long> farthest_vertex(int start, bool save_parent) {
        vector<long long> dist(n, -1);

        if (save_parent) {
            parent.assign(n, -1);
            parent_edge.assign(n, -1);
        }

        stack<int> st;
        st.push(start);
        dist[start] = 0;

        int farthest = start;

        while (!st.empty()) {
            int v = st.top();
            st.pop();

            if (dist[v] > dist[farthest]) {
                farthest = v;
            }

            for (const Edge& edge : graph[v]) {
                if (dist[edge.to] != -1) {
                    continue;
                }

                dist[edge.to] = dist[v] + edge.weight;

                if (save_parent) {
                    parent[edge.to] = v;
                    parent_edge[edge.to] = edge.id;
                }

                st.push(edge.to);
            }
        }

        return {farthest, dist[farthest]};
    }
};

// examples

void yosupo_judge_Tree_Diameter() {
    int N;
    cin >> N;
    TreeDiameter g(N);
    for (int i = 0; i < N - 1; i ++) {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        g.add_edge(a, b, c);
    }
    g.build();
    long long x = g.diameter_length();
    auto list = g.diameter_vertices();
    cout << x << " " << list.size() << endl;
    for (int i = 0; i < (int) list.size(); i ++) {
        cout << list[i];
        if (i < list.size() - 1) cout << " ";
        else cout << endl;
    }
}

int main() {
    yosupo_judge_Tree_Diameter();
}