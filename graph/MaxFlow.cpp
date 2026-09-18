#include <bits/stdc++.h>
using namespace std;

struct MaxFlow {
    struct EdgeInfo {
        int from;
        int to;
        long long capacity;
        long long flow;
    };

    struct Edge {
        int to;
        long long capacity;
        int reverse_index;
    };

    int n;
    vector<vector<Edge>> graph;
    vector<pair<int, int>> edge_positions;
    vector<int> level;
    vector<int> current_edge;

private:
    // レベルグラフ構築: bfs(始点, 終点) -> 終点に到達可能なら true
    bool bfs(int source, int sink) {
        fill(level.begin(), level.end(), -1);

        queue<int> que;
        level[source] = 0;
        que.push(source);

        while (!que.empty()) {
            int v = que.front();
            que.pop();

            for (const auto& edge : graph[v]) {
                if (edge.capacity == 0) continue;
                if (level[edge.to] != -1) continue;

                level[edge.to] = level[v] + 1;
                que.push(edge.to);
            }
        }

        return level[sink] != -1;
    }

    // 増加路探索: dfs(現在地, 終点, 流せる上限) -> 実際に流した量
    long long dfs(int v, int sink, long long flow_limit) {
        if (v == sink) return flow_limit;

        for (int& i = current_edge[v]; i < (int)graph[v].size(); i++) {
            Edge& edge = graph[v][i];

            if (edge.capacity == 0) continue;
            if (level[edge.to] != level[v] + 1) continue;

            long long pushed = dfs(
                edge.to,
                sink,
                min(flow_limit, edge.capacity)
            );

            if (pushed == 0) continue;

            edge.capacity -= pushed;
            graph[edge.to][edge.reverse_index].capacity += pushed;

            return pushed;
        }

        return 0;
    }

public:
    // 初期化: MaxFlow(頂点数)
    MaxFlow(int n)
        : n(n),
          graph(n),
          level(n),
          current_edge(n) {
        assert(n >= 0);
    }

    // 辺追加: add_edge(始点, 終点, 容量) -> 追加した辺番号
    int add_edge(int from, int to, long long capacity) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        assert(capacity >= 0);

        int from_index = (int)graph[from].size();
        int to_index = (int)graph[to].size();

        if (from == to) {
            to_index++;
        }

        graph[from].push_back({to, capacity, to_index});
        graph[to].push_back({from, 0, from_index});

        edge_positions.push_back({from, from_index});

        return (int)edge_positions.size() - 1;
    }

    // 最大流: flow(始点, 終点) -> 今回追加で流せた流量
    // 前提: 流量の合計が long long の範囲に収まる
    long long flow(int source, int sink) {
        assert(0 <= source && source < n);
        assert(0 <= sink && sink < n);
        assert(source != sink);

        long long total_flow = 0;

        while (bfs(source, sink)) {
            fill(current_edge.begin(), current_edge.end(), 0);

            while (true) {
                long long pushed = dfs(
                    source,
                    sink,
                    numeric_limits<long long>::max()
                );

                if (pushed == 0) break;

                total_flow += pushed;
            }
        }

        return total_flow;
    }

    // 辺取得: get_edge(辺番号) -> 元の容量と現在の流量
    EdgeInfo get_edge(int edge_id) const {
        assert(0 <= edge_id && edge_id < (int)edge_positions.size());

        auto [from, index] = edge_positions[edge_id];

        const Edge& edge = graph[from][index];
        const Edge& reverse_edge = graph[edge.to][edge.reverse_index];

        return {
            from,
            edge.to,
            edge.capacity + reverse_edge.capacity,
            reverse_edge.capacity
        };
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

    // 最小カット: min_cut(始点) -> 残余グラフで始点から到達可能なら true
    // 同じ始点から最大流を求めた後に呼ぶと最小カットのS側になる
    vector<bool> min_cut(int source) const {
        assert(0 <= source && source < n);

        vector<bool> reachable(n, false);
        queue<int> que;

        reachable[source] = true;
        que.push(source);

        while (!que.empty()) {
            int v = que.front();
            que.pop();

            for (const auto& edge : graph[v]) {
                if (edge.capacity == 0) continue;
                if (reachable[edge.to]) continue;

                reachable[edge.to] = true;
                que.push(edge.to);
            }
        }

        return reachable;
    }

    // S側頂点取得: source_side(始点) -> 現在の残余グラフで始点から到達可能な頂点
    vector<int> source_side(int source) const {
        vector<bool> reachable = min_cut(source);

        vector<int> result;

        for (int v = 0; v < n; v++) {
            if (reachable[v]) {
                result.push_back(v);
            }
        }

        return result;
    }
};

// example

void yosupo_judge_Matching_on_Bipartite_Graph() {
    int L, R, M;
    cin >> L >> R >> M;
    MaxFlow f(L + R + 2);
    int s = L + R, t = s + 1;
    for (int i = 0; i < M; i ++) {
        int a, b;
        cin >> a >> b;
        f.add_edge(a, L + b, 1);
    }
    for (int i = 0; i < L; i ++) {
        f.add_edge(s, i, 1);
    }
    for (int i = 0; i < R; i ++) {
        f.add_edge(L + i, t, 1);
    }
    cout << f.flow(s, t) << endl;
    for (int i = 0; i < M; i ++) {
        if (f.get_edge(i).flow) {
            cout << f.get_edge(i).from << " " << f.get_edge(i).to - L << endl;
        }
    }
}

int main() {
    yosupo_judge_Matching_on_Bipartite_Graph();
}