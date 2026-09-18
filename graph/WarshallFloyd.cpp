#include <bits/stdc++.h>
using namespace std;

template<class T>
struct WarshallFloyd {
    int n;
    bool directed;
    bool built;
    int edge_count;

    const T INF = numeric_limits<T>::max() / 4;

    vector<vector<T>> initial_distance;
    vector<vector<int>> initial_edge;

    vector<vector<T>> distance;
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
          initial_distance(n, vector<T>(n, INF)),
          initial_edge(n, vector<int>(n, -1)),
          distance(n, vector<T>(n, INF)),
          next_vertex(n, vector<int>(n, -1)),
          next_edge(n, vector<int>(n, -1)),
          negative_infinite(n, vector<bool>(n, false)),
          has_negative_cycle_flag(false) {
        assert(n >= 0);

        for (int v = 0; v < n; v++) {
            initial_distance[v][v] = 0;
        }
    }

    // 辺追加: add_edge(始点, 終点, コスト)
    void add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        assert(-INF < cost && cost < INF);

        int edge_id = edge_count++;

        if (cost < initial_distance[from][to]) {
            initial_distance[from][to] = cost;
            initial_edge[from][to] = edge_id;
        }

        if (!directed && cost < initial_distance[to][from]) {
            initial_distance[to][from] = cost;
            initial_edge[to][from] = edge_id;
        }

        built = false;
    }

    // 構築: build() -> 全点対最短距離、O(n^3)
    // 前提: 単純パスの重みの絶対値が INF 未満
    void build() {
        distance = initial_distance;

        next_vertex.assign(n, vector<int>(n, -1));
        next_edge.assign(n, vector<int>(n, -1));
        negative_infinite.assign(n, vector<bool>(n, false));

        for (int from = 0; from < n; from++) {
            for (int to = 0; to < n; to++) {
                if (initial_edge[from][to] != -1) {
                    next_vertex[from][to] = to;
                    next_edge[from][to] = initial_edge[from][to];
                }
            }
        }

        for (int mid = 0; mid < n; mid++) {
            for (int from = 0; from < n; from++) {
                if (distance[from][mid] == INF) continue;

                for (int to = 0; to < n; to++) {
                    if (distance[mid][to] == INF) continue;

                    // 負閉路による距離の減少は -INF で止める
                    T new_distance = max(
                        -INF, distance[from][mid] + distance[mid][to]
                    );

                    if (new_distance < distance[from][to]) {
                        distance[from][to] = new_distance;
                        next_vertex[from][to] =
                            next_vertex[from][mid];
                        next_edge[from][to] =
                            next_edge[from][mid];
                    }
                }
            }
        }

        has_negative_cycle_flag = false;

        for (int mid = 0; mid < n; mid++) {
            if (distance[mid][mid] >= 0) continue;

            has_negative_cycle_flag = true;

            for (int from = 0; from < n; from++) {
                if (distance[from][mid] == INF) continue;

                for (int to = 0; to < n; to++) {
                    if (distance[mid][to] == INF) continue;

                    negative_infinite[from][to] = true;
                }
            }
        }

        built = true;
    }

    // 最短距離: dist(始点, 終点) -> 到達不能なら INF、負の無限大の場合は無効
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

        return distance[from][to] != INF;
    }

    // 負閉路判定: has_negative_cycle() -> 負閉路が存在するなら true
    bool has_negative_cycle() const {
        assert(built);

        return has_negative_cycle_flag;
    }

    // 負閉路影響判定: is_negative_infinite(始点, 終点) -> 最短距離が負の無限大なら true
    bool is_negative_infinite(int from, int to) const {
        assert(built);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);

        return negative_infinite[from][to];
    }

    // 経路復元: get_path(始点, 終点) -> 頂点列、到達不能・負の無限大なら空
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

    // 経路復元: get_path_edges(始点, 終点) -> 辺ID列、到達不能・負の無限大なら空
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
};