#include <bits/stdc++.h>
using namespace std;

// ランレングス圧縮: run_length_encoding(文字列) -> {文字, 連続数} の列
vector<pair<char, int>> run_length_encoding(const string& s) {
    vector<pair<char, int>> result;

    for (char c : s) {
        if (result.empty() || result.back().first != c) {
            result.push_back({c, 1});
        } else {
            ++result.back().second;
        }
    }

    return result;
}

// ランレングス圧縮: run_length_encoding(配列) -> {値, 連続数} の列
template<class T>
vector<pair<T, int>> run_length_encoding(const vector<T>& values) {
    vector<pair<T, int>> result;

    for (const T& value : values) {
        if (result.empty() || result.back().first != value) {
            result.push_back({value, 1});
        } else {
            ++result.back().second;
        }
    }

    return result;
}

// examples

void ABC_259_C() {
    string S, T;
    cin >> S >> T;
    auto list_s = run_length_encoding(S);
    auto list_t = run_length_encoding(T);
    bool ok = true;
    if (list_s.size() != list_t.size()) ok = false;
    else {
        for (int i = 0; i < list_s.size(); i ++) {
            if (list_s[i].first != list_t[i].first) ok = false;
            if (list_s[i].second == 1 && list_t[i].second != 1) ok = false;
            if (list_s[i].second >= 2 && list_t[i].second < list_s[i].second) ok = false;
        }
    }
    if (ok) cout << "Yes" << endl;
    else cout << "No" << endl;
}

int main() {
    ABC_259_C();
}