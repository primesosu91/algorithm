#include <bits/stdc++.h>
using namespace std;

struct MinCostFlow {
    struct EdgeInfo {
        int from;
        int to;
        long long capacity;
        long long flow;
        long long cost;
    };

    struct FlowResult {
        long long flow;
        long long cost;
    };

    struct Edge {
        int to;
        long long capacity;
        long long cost;
        int reverse_index;
    };

    int n;
    vector<vector<Edge>> graph;
    vector<pair<int, int>> edge_positions;
    vector<long long> potential;
    vector<long long> distance;
    vector<int> previous_vertex;
    vector<int> previous_edge;
    bool started;
    bool has_flow;

private:
    // 初期ポテンシャル構築: initialize_potential(始点)
    // 既にフローがある場合は現在の残余グラフ上で Bellman-Ford を行う
    void initialize_potential(int source) {
        fill(potential.begin(), potential.end(), 0);
        if (!has_flow) return;

        const long long inf = numeric_limits<long long>::max() / 4;

        fill(distance.begin(), distance.end(), inf);
        // 大きな距離と未到達を区別し、番兵の値で到達性を判定しない
        vector<bool> reached(n, false);
        distance[source] = 0;
        reached[source] = true;

        for (int iteration = 0; iteration < n - 1; iteration++) {
            bool updated = false;

            for (int v = 0; v < n; v++) {
                if (!reached[v]) continue;

                for (const auto& edge : graph[v]) {
                    if (edge.capacity == 0) continue;

                    __int128 next_distance = (__int128)distance[v] + edge.cost;

                    assert(numeric_limits<long long>::min() <= next_distance);
                    assert(next_distance <= numeric_limits<long long>::max());

                    if (reached[edge.to] && (long long)next_distance >= distance[edge.to]) continue;

                    distance[edge.to] = (long long)next_distance;
                    reached[edge.to] = true;
                    updated = true;
                }
            }

            if (!updated) break;
        }

        for (int v = 0; v < n; v++) {
            if (reached[v]) {
                potential[v] = distance[v];
            }
        }
    }

    // 最短増加路探索: shortest_path(始点, 終点) -> 到達可能なら true
    bool shortest_path(int source, int sink) {
        const long long inf = numeric_limits<long long>::max() / 4;

        fill(distance.begin(), distance.end(), inf);
        // 大きな距離と未到達を区別し、番兵の値で到達性を判定しない
        vector<bool> reached(n, false);
        fill(previous_vertex.begin(), previous_vertex.end(), -1);
        fill(previous_edge.begin(), previous_edge.end(), -1);

        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> que;

        distance[source] = 0;
        reached[source] = true;
        que.push({0, source});

        while (!que.empty()) {
            auto [current_distance, v] = que.top();
            que.pop();

            if (current_distance != distance[v]) continue;

            for (int edge_index = 0; edge_index < (int)graph[v].size(); edge_index++) {
                const Edge& edge = graph[v][edge_index];

                if (edge.capacity == 0) continue;

                __int128 reduced_cost = (__int128)edge.cost + potential[v] - potential[edge.to];

                assert(reduced_cost >= 0);
                assert(reduced_cost <= numeric_limits<long long>::max());

                __int128 next_distance = (__int128)distance[v] + reduced_cost;

                assert(next_distance <= numeric_limits<long long>::max());

                if (reached[edge.to] && (long long)next_distance >= distance[edge.to]) continue;

                distance[edge.to] = (long long)next_distance;
                reached[edge.to] = true;
                previous_vertex[edge.to] = v;
                previous_edge[edge.to] = edge_index;

                que.push({distance[edge.to], edge.to});
            }
        }

        if (!reached[sink]) return false;

        // 到達可能な頂点のポテンシャルを更新
        for (int v = 0; v < n; v++) {
            if (!reached[v]) continue;

            __int128 next_potential = (__int128)potential[v] + distance[v];

            assert(numeric_limits<long long>::min() <= next_potential);
            assert(next_potential <= numeric_limits<long long>::max());

            potential[v] = (long long)next_potential;
        }

        return true;
    }

public:
    // 初期化: MinCostFlow(頂点数)
    MinCostFlow(int n) : n(n), graph(n), potential(n), distance(n), previous_vertex(n), previous_edge(n), started(false), has_flow(false) {
        assert(n >= 0);
    }

    // 辺追加: add_edge(始点, 終点, 容量, 単位コスト) -> 追加した辺番号
    // 前提: flow または slope を呼ぶ前、かつ cost >= 0
    int add_edge(int from, int to, long long capacity, long long cost) {
        assert(!started);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        assert(capacity >= 0);
        assert(cost >= 0);

        int from_index = (int)graph[from].size();
        int to_index = (int)graph[to].size();

        if (from == to) {
            to_index++;
        }

        graph[from].push_back({to, capacity, cost, to_index});
        graph[to].push_back({from, 0, -cost, from_index});

        edge_positions.push_back({from, from_index});

        return (int)edge_positions.size() - 1;
    }

    // 費用曲線: slope(始点, 終点, 流量上限) -> (今回の追加流量, 今回の追加費用) の折れ点
    // 先頭は必ず {0, 0}、同じ限界費用の区間はまとめる
    // 前提: 流量・費用・途中計算が long long の範囲に収まる
    vector<FlowResult> slope(int source, int sink, long long flow_limit) {
        assert(0 <= source && source < n);
        assert(0 <= sink && sink < n);
        assert(source != sink);
        assert(flow_limit >= 0);

        started = true;
        initialize_potential(source);

        vector<FlowResult> result;
        result.push_back({0, 0});

        long long total_flow = 0;
        long long total_cost = 0;
        long long previous_marginal_cost = 0;
        bool has_previous_cost = false;

        while (total_flow < flow_limit) {
            if (!shortest_path(source, sink)) break;

            // 最短増加路に流せる最大量を求める
            long long pushed = flow_limit - total_flow;

            for (int v = sink; v != source; v = previous_vertex[v]) {
                int u = previous_vertex[v];
                int edge_index = previous_edge[v];

                assert(u != -1);
                assert(edge_index != -1);

                pushed = min(pushed, graph[u][edge_index].capacity);
            }

            // 残余グラフを更新
            for (int v = sink; v != source; v = previous_vertex[v]) {
                int u = previous_vertex[v];
                int edge_index = previous_edge[v];

                Edge& edge = graph[u][edge_index];

                edge.capacity -= pushed;
                graph[edge.to][edge.reverse_index].capacity += pushed;
            }

            __int128 marginal_cost_value = (__int128)potential[sink] - potential[source];

            assert(numeric_limits<long long>::min() <= marginal_cost_value);
            assert(marginal_cost_value <= numeric_limits<long long>::max());

            long long marginal_cost = (long long)marginal_cost_value;

            __int128 next_cost = (__int128)total_cost + (__int128)pushed * marginal_cost;

            assert(numeric_limits<long long>::min() <= next_cost);
            assert(next_cost <= numeric_limits<long long>::max());

            total_flow += pushed;
            total_cost = (long long)next_cost;

            // 同じ限界費用なら直前の区間とまとめる
            if (has_previous_cost && previous_marginal_cost == marginal_cost) {
                result.back() = {total_flow, total_cost};
            } else {
                result.push_back({total_flow, total_cost});
                previous_marginal_cost = marginal_cost;
                has_previous_cost = true;
            }
        }

        if (total_flow > 0) {
            has_flow = true;
        }

        return result;
    }

    // 最小費用流: flow(始点, 終点, 流量上限) -> 今回追加で流せた流量と、その最小費用
    FlowResult flow(int source, int sink, long long flow_limit) {
        return slope(source, sink, flow_limit).back();
    }

    // 辺取得: get_edge(辺番号) -> 元の容量・現在の流量・単位コスト
    EdgeInfo get_edge(int edge_id) const {
        assert(0 <= edge_id && edge_id < (int)edge_positions.size());

        auto [from, index] = edge_positions[edge_id];

        const Edge& edge = graph[from][index];
        const Edge& reverse_edge = graph[edge.to][edge.reverse_index];

        return {from, edge.to, edge.capacity + reverse_edge.capacity, reverse_edge.capacity, edge.cost};
    }

    // 全辺取得: edges() -> add_edge で追加した順の辺情報
    vector<EdgeInfo> edges() const {
        vector<EdgeInfo> result;
        result.reserve(edge_positions.size());

        for (int edge_id = 0; edge_id < (int)edge_positions.size(); edge_id++) {
            result.push_back(get_edge(edge_id));
        }

        return result;
    }
};
