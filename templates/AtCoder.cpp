// https://github.com/primesosu91/algorithm
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using pll = pair<ll, ll>;
#define rep(i, n) for (ll i = 0; i < (ll)(n); i++)
#define rep1(i, n) for (ll i = 1; i <= (ll)(n); i++)
#define rrep(i, n) for (ll i = n - 1; i >= (ll)(0); i--)
#define rrep1(i, n) for (ll i = n; i >= (ll)(1); i--)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define el '\n'
#define sp ' '
#define YN(text) cout << (text ? "Yes" : "No") << el;
#define ANS cout << ans << el;
#define pqmax(...) priority_queue<__VA_ARGS__>
#define pqmin(...) priority_queue<__VA_ARGS__, vector<__VA_ARGS__>, greater<__VA_ARGS__>>
const ll inf = 4e18;
const ll MOD = 998244353;
const ll dx[4] = {1, 0, -1, 0};
const ll dy[4] = {0, 1, 0, -1};
const ll DX[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const ll DY[8] = {0, 1, 1, 1, 0, -1, -1, -1};
template <class T> bool chmax(T& a, const T& b) { if (a < b) { a = b; return true; } return false; }
template <class T> bool chmin(T& a, const T& b) { if (a > b) { a = b; return true; } return false; }

void solve() {
    
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll t = 1;
    // cin >> t;
    rep(i, t) solve();
}
