#include "geometry/ConvexPolygon.cpp"

int main() {
    vector<pair<vector<Point>, bool>> examples = {
        {{{0, 0}, {2, 0}, {0, 2}}, true},
        {{{0, 0}, {1, 0}, {2, 0}, {2, 2}, {0, 2}}, true},
        {{{0, 0}, {2, 0}, {1, 1}, {2, 2}, {0, 2}}, false},
        {{{0, 0}, {1, 0}, {2, 0}}, false},
        {{{0, 0}, {0, 0}, {0, 0}}, false},
        {{{-1000000000000000000LL, -1000000000000000000LL},
          {1000000000000000000LL, -1000000000000000000LL},
          {1000000000000000000LL, 1000000000000000000LL},
          {-1000000000000000000LL, 1000000000000000000LL}}, true}
    };
    for (auto [polygon, expected] : examples) {
        for (int orientation = 0; orientation < 2; ++orientation) {
            for (int rotation = 0; rotation < (int)polygon.size(); ++rotation) {
                assert(is_convex(polygon) == expected);
                rotate(polygon.begin(), polygon.begin() + 1, polygon.end());
            }
            reverse(polygon.begin(), polygon.end());
        }
    }
    assert(cross({0, 0}, {2, 0}, {0, 2}) == 4);
    cout << "ConvexPolygon: both orientations, collinearity, concavity, and wide cross products passed\n";
}
