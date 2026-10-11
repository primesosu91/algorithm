#include "library/algorithm/RunLengthEncoding.cpp"

int main() {
    mt19937 rng(20261012);
    for (int trial = 0; trial < 2000; ++trial) {
        int n = rng() % 100;
        string text;
        vector<int> values;
        for (int i = 0; i < n; ++i) {
            text.push_back((char)('a' + rng() % 3));
            values.push_back((int)(rng() % 5) - 2);
        }
        auto encoded_text = run_length_encoding(text);
        auto encoded_values = run_length_encoding(values);
        string restored_text;
        vector<int> restored_values;
        for (int i = 0; i < (int)encoded_text.size(); ++i) {
            auto [c, count] = encoded_text[i];
            assert(count > 0 && (i == 0 || encoded_text[i - 1].first != c));
            restored_text.append(count, c);
        }
        for (int i = 0; i < (int)encoded_values.size(); ++i) {
            auto [value, count] = encoded_values[i];
            assert(count > 0 && (i == 0 || encoded_values[i - 1].first != value));
            restored_values.insert(restored_values.end(), count, value);
        }
        assert(restored_text == text && restored_values == values);
    }
    cout << "RunLengthEncoding: lossless restoration and maximal runs passed\n";
}
