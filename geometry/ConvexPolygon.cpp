#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x;
    long long y;
};

// 外積: cross(点a, 点b, 点c) -> (b - a) × (c - a)
// 前提: 計算結果が __int128 の表現範囲内
__int128 cross(const Point& a, const Point& b, const Point& c) {
    __int128 ab_x = (__int128)b.x - a.x;
    __int128 ab_y = (__int128)b.y - a.y;
    __int128 ac_x = (__int128)c.x - a.x;
    __int128 ac_y = (__int128)c.y - a.y;

    return ab_x * ac_y - ab_y * ac_x;
}

// 凸判定: is_convex(多角形) -> 単純多角形が凸なら true
// 境界上の連続する3点が一直線の場合も凸として扱う
bool is_convex(const vector<Point>& polygon) {
    assert(polygon.size() >= 3);
    assert(polygon.size() <= (size_t)numeric_limits<int>::max());

    int n = (int)polygon.size();

    bool positive = false;
    bool negative = false;

    for (int i = 0; i < n; ++i) {
        __int128 value = cross(
            polygon[i],
            polygon[(i + 1) % n],
            polygon[(i + 2) % n]
        );

        if (value > 0) positive = true;
        if (value < 0) negative = true;

        if (positive && negative) return false;
    }

    return positive || negative;
}

// examples

void ABC_226_C() {
    vector<Point> p(4);
    for (int i = 0; i < 4; i ++) {
        int x, y;
        cin >> x >> y;
        p[i] = {x, y};
    }
    bool ok = is_convex(p);
    if (ok) cout << "Yes" << endl;
    else cout << "No" << endl;
}

int main() {
    ABC_226_C();
}