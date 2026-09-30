#include <bits/stdc++.h>
using namespace std;

struct ProjectSelection {
    struct Penalty {
        int from;
        int to;
        long long penalty;
    };

    struct FlowGraph {
        struct Edge {
            int to;
            long long capacity;
            int reverse_index;
        };

        int n;
        vector<vector<Edge>> graph;
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

                long long pushed = dfs(edge.to, sink, min(flow_limit, edge.capacity));
                if (pushed == 0) continue;

                edge.capacity -= pushed;
                graph[edge.to][edge.reverse_index].capacity += pushed;

                return pushed;
            }

            return 0;
        }

    public:
        // 初期化: FlowGraph(頂点数)
        FlowGraph(int n) : n(n), graph(n), level(n), current_edge(n) {
            assert(n >= 0);
        }

        // 辺追加: add_edge(始点, 終点, 容量)
        void add_edge(int from, int to, long long capacity) {
            assert(0 <= from && from < n);
            assert(0 <= to && to < n);
            assert(capacity >= 0);

            int from_index = (int)graph[from].size();
            int to_index = (int)graph[to].size();

            if (from == to) to_index++;

            graph[from].push_back({to, capacity, to_index});
            graph[to].push_back({from, 0, from_index});
        }

        // 最大流: flow(始点, 終点) -> 最大流量
        long long flow(int source, int sink) {
            assert(0 <= source && source < n);
            assert(0 <= sink && sink < n);
            assert(source != sink);

            long long total_flow = 0;

            while (bfs(source, sink)) {
                fill(current_edge.begin(), current_edge.end(), 0);

                while (true) {
                    long long pushed = dfs(source, sink, numeric_limits<long long>::max());
                    if (pushed == 0) break;

                    assert(total_flow <= numeric_limits<long long>::max() - pushed);
                    total_flow += pushed;
                }
            }

            return total_flow;
        }

        // 最小カット: min_cut(始点) -> 残余グラフで始点から到達可能なら true
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
    };

    int n;
    vector<long long> profit;
    vector<Penalty> penalties;
    vector<pair<int, int>> dependencies;
    bool solved;
    long long answer;
    vector<bool> selected_result;

    // 初期化: ProjectSelection(要素数)
    ProjectSelection(int n) : n(n), profit(n, 0), solved(false), answer(0) {
        assert(n >= 0);
    }

    // 利益追加: add_profit(頂点, 選択したときの利益)
    void add_profit(int v, long long value) {
        assert(!solved);
        assert(0 <= v && v < n);

        __int128 next_profit = (__int128)profit[v] + value;

        assert(numeric_limits<long long>::min() <= next_profit);
        assert(next_profit <= numeric_limits<long long>::max());

        profit[v] = (long long)next_profit;
    }

    // ペナルティ追加: add_penalty(u, v, ペナルティ) -> u を選び v を選ばない場合に発生
    void add_penalty(int u, int v, long long penalty) {
        assert(!solved);
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        assert(penalty >= 0);

        penalties.push_back({u, v, penalty});
    }

    // 依存関係追加: add_dependency(u, v) -> u を選ぶなら v も選ぶ
    void add_dependency(int u, int v) {
        assert(!solved);
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);

        dependencies.push_back({u, v});
    }

    // 最大利益: solve() -> 制約を満たす選択による最大利益
    long long solve() {
        assert(!solved);
        solved = true;

        int source = n;
        int sink = n + 1;

        __int128 finite_capacity_sum = 0;
        __int128 base_profit = 0;

        // 利益・損失による有限容量を集計
        for (int v = 0; v < n; v++) {
            if (profit[v] > 0) {
                finite_capacity_sum += profit[v];
                base_profit += profit[v];
            } else if (profit[v] < 0) {
                __int128 capacity = -(__int128)profit[v];

                assert(capacity <= numeric_limits<long long>::max());
                finite_capacity_sum += capacity;
            }
        }

        // ペナルティによる有限容量を集計
        for (const auto& constraint : penalties) {
            finite_capacity_sum += constraint.penalty;
        }

        assert(finite_capacity_sum <= numeric_limits<long long>::max());

        // 依存関係を破ることが最適にならない容量を決定
        long long infinite_capacity = 0;

        if (!dependencies.empty()) {
            __int128 infinite_capacity_value = finite_capacity_sum + 1;

            assert(infinite_capacity_value <= numeric_limits<long long>::max());

            infinite_capacity = (long long)infinite_capacity_value;
        }

        FlowGraph flow_graph(n + 2);

        // 各頂点の利益・損失を辺に変換
        for (int v = 0; v < n; v++) {
            if (profit[v] > 0) {
                flow_graph.add_edge(source, v, profit[v]);
            } else if (profit[v] < 0) {
                flow_graph.add_edge(v, sink, (long long)(-(__int128)profit[v]));
            }
        }

        // u を選び v を選ばない場合のペナルティ
        for (const auto& constraint : penalties) {
            flow_graph.add_edge(constraint.from, constraint.to, constraint.penalty);
        }

        // u を選ぶなら v も選ぶ
        for (auto [u, v] : dependencies) {
            flow_graph.add_edge(u, v, infinite_capacity);
        }

        long long minimum_cut = flow_graph.flow(source, sink);
        __int128 answer_value = base_profit - minimum_cut;

        assert(numeric_limits<long long>::min() <= answer_value);
        assert(answer_value <= numeric_limits<long long>::max());

        answer = (long long)answer_value;

        // source 側に残った頂点が選択された頂点
        vector<bool> reachable = flow_graph.min_cut(source);

        selected_result.assign(n, false);

        for (int v = 0; v < n; v++) {
            selected_result[v] = reachable[v];
        }

        return answer;
    }

    // 選択結果: selected() -> 最大利益を達成する選択の1つ
    vector<bool> selected() const {
        assert(solved);
        return selected_result;
    }
};

/ examples

void ARC_085_E() {
    int N;
    cin >> N;
    vector<int> A(N);
    long long sum = 0;
    for (int i = 0; i < N; i ++) {
        cin >> A[i];
        sum += A[i];
    }
    ProjectSelection f(N);
    for (int i = 0; i < N; i ++) {
        f.add_profit(i, -A[i]);
    }
    for (int i = 0; i < N; i ++) {
        for (int j = i; j < N; j ++) {
            if ((j + 1) % (i + 1) == 0) f.add_dependency(i, j);
        }
    }
    cout << sum + f.solve() << endl;
}

int main() {
    ARC_085_E();
}