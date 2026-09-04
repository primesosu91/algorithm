#include <bits/stdc++.h>
using namespace std;

template<class T>
struct Dijkstra {
    struct Edge {
        int to;
        T cost;
        int id;
    };

    static constexpr T INF = numeric_limits<T>::max() / 2;

    int n;
    bool directed;
    int edge_count;
    vector<vector<Edge>> graph;

    bool built;
    int start;
    vector<T> distance;
    vector<bool> reached;
    vector<int> parent_vertex;
    vector<int> parent_edge;

    // 初期化: Dijkstra(頂点数, 有向グラフならtrue)
    Dijkstra(int n, bool directed = true)
        : n(n),
          directed(directed),
          edge_count(0),
          built(false),
          start(-1) {
        assert(n >= 0);

        graph.resize(n);
        distance.assign(n, INF);
        reached.assign(n, false);
        parent_vertex.assign(n, -1);
        parent_edge.assign(n, -1);
    }

    // 辺追加: add_edge(始点, 終点, 重み) -> 追加した辺ID
    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        assert(cost >= T{});
        assert(edge_count < numeric_limits<int>::max());

        int edge_id = edge_count++;

        graph[from].push_back({to, cost, edge_id});

        if (!directed) {
            graph[to].push_back({from, cost, edge_id});
        }

        built = false;

        return edge_id;
    }

    // 最短経路を計算: build(始点)
    // 前提: 計算される最短距離が T の表現範囲内
    void build(int start) {
        assert(0 <= start && start < n);

        this->start = start;
        distance.assign(n, INF);
        reached.assign(n, false);
        parent_vertex.assign(n, -1);
        parent_edge.assign(n, -1);

        priority_queue<
            pair<T, int>,
            vector<pair<T, int>>,
            greater<pair<T, int>>
        > que;

        distance[start] = T{};
        reached[start] = true;
        que.push({T{}, start});

        while (!que.empty()) {
            auto [current_dist, v] = que.top();
            que.pop();

            // 古い距離情報は無視する
            if (current_dist != distance[v]) {
                continue;
            }

            for (const Edge& edge : graph[v]) {
                T new_dist;

                if (!add_without_overflow(distance[v], edge.cost, new_dist)) {
                    assert(false && "shortest distance is outside the range of T");
                    continue;
                }

                if (!reached[edge.to] || new_dist < distance[edge.to]) {
                    distance[edge.to] = new_dist;
                    reached[edge.to] = true;
                    parent_vertex[edge.to] = v;
                    parent_edge[edge.to] = edge.id;

                    que.push({new_dist, edge.to});
                }
            }
        }

        built = true;
    }

    // 最短距離: dist(頂点v) -> 始点からvまでの最短距離
    // 到達不能の場合は INF、判定には reachable() を使用
    T dist(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        return distance[v];
    }

    // 到達可能判定: reachable(頂点v) -> 始点から到達可能なら true
    bool reachable(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        return reached[v];
    }

    // 最短経路の頂点列: get_path(終点) -> 始点から終点までの頂点列
    vector<int> get_path(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        if (!reachable(v)) {
            return {};
        }

        vector<int> path;

        while (v != -1) {
            path.push_back(v);
            v = parent_vertex[v];
        }

        reverse(path.begin(), path.end());

        return path;
    }

    // 最短経路の辺ID列: get_path_edges(終点) -> 始点から終点までの辺ID列
    vector<int> get_path_edges(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        if (!reachable(v)) {
            return {};
        }

        vector<int> edges;

        while (parent_vertex[v] != -1) {
            edges.push_back(parent_edge[v]);
            v = parent_vertex[v];
        }

        reverse(edges.begin(), edges.end());

        return edges;
    }

private:
    static bool add_without_overflow(T a, T b, T& result) {
        if constexpr (is_integral_v<T>) {
            if constexpr (is_signed_v<T>) {
                if (b > 0 && a > numeric_limits<T>::max() - b) return false;
                if (b < 0 && a < numeric_limits<T>::lowest() - b) return false;
            } else {
                if (a > numeric_limits<T>::max() - b) return false;
            }
        }

        result = a + b;
        return true;
    }
};

// examples

void yosupo_judge_Shortest_Path() {
    int N, M, s, t;
    cin >> N >> M >> s >> t;
    Dijkstra<long long> g(N, true);
    for (int i = 0; i < M; i ++) {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        g.add_edge(a, b, c);
    }
    g.build(s);
    bool ok = g.reachable(t);
    if (!ok) {
        cout << -1 << endl;
        return;
    }
    vector<int> list = g.get_path(t);
    cout << g.dist(t) << " " << list.size() - 1 << endl;
    for (int i = 0; i < (int) list.size() - 1; i ++) {
        cout << list[i] << " " << list[i + 1] << endl;
    }
}

int main() {
    yosupo_judge_Shortest_Path();
}