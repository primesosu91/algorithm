#include <bits/stdc++.h>
using namespace std;

template<class T>
struct BellmanFord {
    struct Edge {
        int from;
        int to;
        T cost;
        int id;
    };

    int n;
    bool directed;
    int edge_count;

    vector<Edge> edges;

    vector<T> distance;
    vector<bool> reached;
    vector<bool> negative;
    vector<int> prev;
    vector<int> prev_edge;

    bool built;
    bool negative_cycle;

    // 初期化: BellmanFord(頂点数, 有向グラフならtrue)
    BellmanFord(int n, bool directed = true)
        : n(n),
          directed(directed),
          edge_count(0),
          built(false),
          negative_cycle(false) {
        assert(n >= 0);
    }

    // 辺追加: add_edge(始点, 終点, コスト) -> 追加した辺ID
    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        assert(edge_count < numeric_limits<int>::max());

        int id = edge_count++;

        edges.push_back({from, to, cost, id});

        if (!directed) {
            edges.push_back({to, from, cost, id});
        }

        built = false;

        return id;
    }

    // 最短距離計算: build(始点)
    // 前提: 緩和で計算される距離が T の表現範囲内
    void build(int start) {
        assert(0 <= start && start < n);

        distance.assign(n, T{});
        reached.assign(n, false);
        negative.assign(n, false);
        prev.assign(n, -1);
        prev_edge.assign(n, -1);

        negative_cycle = false;

        distance[start] = T{};
        reached[start] = true;

        // 通常の Bellman-Ford
        for (int i = 0; i < n - 1; ++i) {
            bool updated = false;

            for (const auto& e : edges) {
                if (!reached[e.from]) continue;

                T nd;

                if (!add_without_overflow(distance[e.from], e.cost, nd)) {
                    assert(false && "distance is outside the range of T");
                    continue;
                }

                if (!reached[e.to] || nd < distance[e.to]) {
                    distance[e.to] = nd;
                    reached[e.to] = true;

                    prev[e.to] = e.from;
                    prev_edge[e.to] = e.id;

                    updated = true;
                }
            }

            if (!updated) break;
        }

        // 負閉路の影響を受ける頂点を検出
        for (const auto& e : edges) {
            if (!reached[e.from]) continue;

            T nd;

            if (!add_without_overflow(distance[e.from], e.cost, nd)) {
                assert(false && "distance is outside the range of T");
                continue;
            }

            if (!reached[e.to] || nd < distance[e.to]) {
                negative[e.to] = true;
            }
        }

        // 負閉路の影響を伝播
        for (int i = 0; i < n - 1; ++i) {
            bool updated = false;

            for (const auto& e : edges) {
                if (negative[e.from] && !negative[e.to]) {
                    negative[e.to] = true;
                    updated = true;
                }
            }

            if (!updated) break;
        }

        for (int v = 0; v < n; ++v) {
            if (negative[v]) {
                negative_cycle = true;
                break;
            }
        }

        built = true;
    }

    // 距離取得: dist(頂点) -> 到達不能の場合は T{}、判定には reachable() を使用
    T dist(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        return distance[v];
    }

    // 到達判定: reachable(頂点) -> 到達可能なら true
    bool reachable(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        return reached[v];
    }

    // 負閉路影響判定: is_negative(頂点) -> 最短距離が負閉路の影響を受けるなら true
    bool is_negative(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        return negative[v];
    }

    // 負閉路判定: has_negative_cycle() -> 始点から到達可能な負閉路が存在するなら true
    bool has_negative_cycle() const {
        assert(built);

        return negative_cycle;
    }

    // 経路復元: get_path(終点) -> 始点から終点までの頂点列
    vector<int> get_path(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        if (!reachable(v) || is_negative(v)) return {};

        vector<int> path;

        while (v != -1) {
            path.push_back(v);
            v = prev[v];
        }

        reverse(path.begin(), path.end());

        return path;
    }

    // 経路復元: get_path_edges(終点) -> 始点から終点までの辺ID列
    vector<int> get_path_edges(int v) const {
        assert(built);
        assert(0 <= v && v < n);

        if (!reachable(v) || is_negative(v)) return {};

        vector<int> path_edges;

        while (prev_edge[v] != -1) {
            path_edges.push_back(prev_edge[v]);
            v = prev[v];
        }

        reverse(path_edges.begin(), path_edges.end());

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