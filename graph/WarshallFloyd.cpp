#include <bits/stdc++.h>
using namespace std;

template<class T>
struct WarshallFloyd {
    int n;
    bool directed;
    bool built;
    int edge_count;

    static constexpr T INF = numeric_limits<T>::max() / 4;

    vector<vector<T>> initial_distance;
    vector<vector<bool>> initial_reached;
    vector<vector<int>> initial_edge;

    vector<vector<T>> distance;
    vector<vector<bool>> reached;
    vector<vector<int>> next_vertex;
    vector<vector<int>> next_edge;
    vector<vector<bool>> negative_infinite;

    bool has_negative_cycle_flag;

    // 初期化: WarshallFloyd(頂点数, 有向グラフならtrue)
    WarshallFloyd(int n, bool directed = true)
        : n(n),
          directed(directed),
          built(false),
          edge_count(0),
          has_negative_cycle_flag(false) {
        assert(n >= 0);

        initial_distance.assign(n, vector<T>(n, INF));
        initial_reached.assign(n, vector<bool>(n, false));
        initial_edge.assign(n, vector<int>(n, -1));
        distance.assign(n, vector<T>(n, INF));
        reached.assign(n, vector<bool>(n, false));
        next_vertex.assign(n, vector<int>(n, -1));
        next_edge.assign(n, vector<int>(n, -1));
        negative_infinite.assign(n, vector<bool>(n, false));

        for (int v = 0; v < n; ++v) {
            initial_distance[v][v] = 0;
            initial_reached[v][v] = true;
        }
    }

    // 辺追加: add_edge(始点, 終点, コスト) -> 追加した辺ID
    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        assert(edge_count < numeric_limits<int>::max());

        int edge_id = edge_count++;

        if (!initial_reached[from][to] || cost < initial_distance[from][to]) {
            initial_distance[from][to] = cost;
            initial_reached[from][to] = true;
            initial_edge[from][to] = edge_id;
        }

        if (!directed &&
            (!initial_reached[to][from] || cost < initial_distance[to][from])) {
            initial_distance[to][from] = cost;
            initial_reached[to][from] = true;
            initial_edge[to][from] = edge_id;
        }

        built = false;

        return edge_id;
    }

    // 全点対最短距離を構築: O(N^3)
    // 前提: 緩和で計算される距離が T の表現範囲内
    void build() {
        distance = initial_distance;
        reached = initial_reached;

        next_vertex.assign(n, vector<int>(n, -1));
        next_edge.assign(n, vector<int>(n, -1));
        negative_infinite.assign(n, vector<bool>(n, false));

        for (int from = 0; from < n; ++from) {
            for (int to = 0; to < n; ++to) {
                if (initial_edge[from][to] != -1) {
                    next_vertex[from][to] = to;
                    next_edge[from][to] = initial_edge[from][to];
                }
            }
        }

        for (int mid = 0; mid < n; ++mid) {
            for (int from = 0; from < n; ++from) {
                if (!reached[from][mid]) continue;

                for (int to = 0; to < n; ++to) {
                    if (!reached[mid][to]) continue;

                    T new_distance;

                    if (!add_without_overflow(
                            distance[from][mid],
                            distance[mid][to],
                            new_distance)) {
                        assert(false && "distance is outside the range of T");
                        continue;
                    }

                    if (!reached[from][to] ||
                        new_distance < distance[from][to]) {
                        distance[from][to] = new_distance;
                        reached[from][to] = true;
                        next_vertex[from][to] =
                            next_vertex[from][mid];
                        next_edge[from][to] =
                            next_edge[from][mid];
                    }
                }
            }
        }

        has_negative_cycle_flag = false;

        for (int mid = 0; mid < n; ++mid) {
            if (distance[mid][mid] >= 0) continue;

            has_negative_cycle_flag = true;

            for (int from = 0; from < n; ++from) {
                if (!reached[from][mid]) continue;

                for (int to = 0; to < n; ++to) {
                    if (!reached[mid][to]) continue;

                    negative_infinite[from][to] = true;
                }
            }
        }

        built = true;
    }

    // 最短距離: dist(始点, 終点)
    // 到達不能の場合は INF、判定には reachable() を使用
    T dist(int from, int to) const {
        assert(built);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        return distance[from][to];
    }

    // 到達可能判定: reachable(始点, 終点)
    bool reachable(int from, int to) const {
        assert(built);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        return reached[from][to];
    }

    // 負閉路が1つでも存在するなら true
    bool has_negative_cycle() const {
        assert(built);

        return has_negative_cycle_flag;
    }

    // 最短距離が負の無限大なら true
    bool is_negative_infinite(int from, int to) const {
        assert(built);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        return negative_infinite[from][to];
    }

    // 最短経路の頂点列: get_path(始点, 終点) -> 始点から終点までの頂点列
    vector<int> get_path(int from, int to) const {
        assert(built);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        if (!reachable(from, to)) return {};
        if (is_negative_infinite(from, to)) return {};

        vector<int> path = {from};

        if (from == to) return path;

        while (from != to) {
            from = next_vertex[from][to];
            path.push_back(from);
        }

        return path;
    }

    // 最短経路の辺ID列: get_path_edges(始点, 終点) -> 対応する辺ID列
    vector<int> get_path_edges(int from, int to) const {
        assert(built);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        if (!reachable(from, to)) return {};
        if (is_negative_infinite(from, to)) return {};

        vector<int> path_edges;

        while (from != to) {
            path_edges.push_back(next_edge[from][to]);
            from = next_vertex[from][to];
        }

        return path_edges;
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