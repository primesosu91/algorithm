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
