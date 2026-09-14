#include <bits/stdc++.h>
using namespace std;
using LL = long long;
const int MAXN = 200005;

#include "../src/sections/String/assets/String/PAM_bidirectional.cpp"
#include "../src/sections/String/assets/String/hash.cpp"
#include "../src/sections/String/assets/String/hash_check.cpp"

namespace hash_test {
u64 direct_hash(const string& text, int left, int right,
                u64 base, u64 mod) {
    u64 result = 0;
    for (int i = left; i <= right; ++i) {
        u64 value = u64(static_cast<unsigned char>(text[i])) + 1;
        result = u64((u128(result) * base + value) % mod);
    }
    return result;
}

bool substring_queries() {
    constexpr u64 base = 911382323;
    constexpr u64 mod = StringHash::DEFAULT_MOD;
    StringHash default_first, default_second;
    if (default_first.B != default_second.B
            || default_first.M != default_second.M) return false;
    vector<string> cases = {
        "a", "banana", "abracadabra", "tcgstgzar", "gscizkiak",
        string({char(0), char(3), char(127), char(128), char(255)})
    };
    mt19937 generator(31415926);
    for (int length = 1; length <= 80; ++length) {
        string text(length, '\0');
        for (char& c : text) c = char(generator() & 255);
        cases.push_back(text);
    }
    for (const string& text : cases) {
        StringHash hash(base, mod);
        hash.build(text);
        for (int left = 0; left < int(text.size()); ++left) {
            for (int right = left; right < int(text.size()); ++right) {
                if (hash.get(left + 1, right + 1)
                        != direct_hash(text, left, right, base, mod))
                    return false;
            }
        }
        hash.init(base + 2, mod);
        if (hash.n != 0 || !hash.h.empty() || !hash.p.empty()) return false;
    }

    StringHash old_collision(base, mod);
    old_collision.build("tcgstgzar");
    u64 first = old_collision.get(1, 9);
    old_collision.build("gscizkiak");
    return first != old_collision.get(1, 9);
}

bool fast_mod_check() {
    auto expected = [](i128 a, i128 b) {
        u128 difference = a >= b ? u128(a) - u128(b) : u128(b) - u128(a);
        return difference % CHECK_MOD == 0;
    };
    const i128 maximum = i128((u128(1) << 127) - 1);
    const i128 minimum = -maximum - 1;
    vector<pair<i128, i128>> cases = {
        {0, 0}, {0, i128(CHECK_MOD)}, {-1, i128(CHECK_MOD) - 1},
        {123456789, -987654321}, {maximum, minimum},
        {maximum, maximum - i128(CHECK_MOD)}
    };
    for (auto [a, b] : cases)
        if (check(a, b) != expected(a, b)) return false;
    return true;
}
} // namespace hash_test

namespace manacher_test {
constexpr int max_length = 32;
char s[2 * max_length + 3];
int h[2 * max_length + 3], n;

#include "../src/sections/String/assets/String/Manacher.cpp"

bool repeated_reads() {
    const vector<pair<string, int>> cases = {
        {"aaaa", 4}, {"a", 1}, {"abba", 4}, {"abacaba", 7},
        {"abcd", 1}, {string(max_length, 'a'), max_length}, {"b", 1}
    };
    FILE* input = tmpfile();
    if (input == nullptr) return false;
    for (const auto& item : cases) {
        fputs(item.first.c_str(), input);
        fputc('\n', input);
    }
    rewind(input);
    FILE* saved_stdin = stdin;
    stdin = input;

    // Reuse both buffers without clearing them between strings.
    fill(begin(s), end(s), 'a');
    n = 5;
    bool ok = true;
    for (const auto& item : cases) {
        read();
        if (n != 2 * static_cast<int>(item.first.size()) + 1) {
            ok = false;
            break;
        }
        manacher();
        if (*max_element(h + 1, h + n + 1) - 1 != item.second) ok = false;
        for (int i = 1; i <= n; ++i) {
            if (h[i] < 1 || h[i] > i || h[i] > n - i + 1) ok = false;
        }
        if (!ok) break;
    }
    stdin = saved_stdin;
    fclose(input);
    return ok;
}
} // namespace manacher_test

namespace lyndon_test {
#include "../src/sections/String/assets/String/Lyndon Word.cpp"

bool minimum_suffixes() {
    array<int, 11> positions;
    positions.fill(-1);
    auto check = [&](const string& text) {
        string padded = " " + text;
        int length = static_cast<int>(text.size());
        mnsuf(padded.data(), positions.data(), length);
        for (int end = 1; end <= length; ++end) {
            string_view prefix(text.data(), end);
            int best = 0;
            for (int start = 1; start < end; ++start) {
                if (prefix.substr(start) < prefix.substr(best)) best = start;
            }
            if (positions[end] != best + 1) return false;
        }
        return true;
    };
    for (int length = 1; length <= 10; ++length) {
        for (int mask = 0; mask < (1 << length); ++mask) {
            string text(length, 'a');
            for (int i = 0; i < length; ++i) text[i] += (mask >> i) & 1;
            if (!check(text)) return false;
        }
    }
    // Reuse the output array for shorter inputs without clearing it.
    for (const char* text : {"banana", "aaaa", "a", "abababa", "cba", "abacaba"}) {
        if (!check(text)) return false;
    }
    return true;
}
} // namespace lyndon_test

namespace runs_test {
#include "../src/sections/String/assets/String/Runs.cpp"

vector<Runs> brute_force(const string& text) {
    int n = static_cast<int>(text.size());
    vector<Runs> answer;
    for (int left = 0; left < n; ++left) {
        for (int right = left; right < n; ++right) {
            int length = right - left + 1;
            int period = length;
            for (int candidate = 1; candidate <= length; ++candidate) {
                bool valid = true;
                for (int i = left; i + candidate <= right; ++i) {
                    if (text[i] != text[i + candidate]) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    period = candidate;
                    break;
                }
            }
            if (length < 2 * period) continue;
            if (left > 0 && text[left - 1] == text[left - 1 + period]) continue;
            if (right + 1 < n && text[right + 1] == text[right + 1 - period])
                continue;
            answer.push_back({left + 1, right + 1, period});
        }
    }
    return answer;
}

bool exhaustive_runs() {
    for (int alphabet_size : {2, 3}) {
        int maximum_length = alphabet_size == 2 ? 12 : 8;
        for (int length = 0; length <= maximum_length; ++length) {
            int count = 1;
            for (int i = 0; i < length; ++i) count *= alphabet_size;
            for (int mask = 0; mask < count; ++mask) {
                string text(length, 'a');
                int value = mask;
                for (char& character : text) {
                    character += value % alphabet_size;
                    value /= alphabet_size;
                }
                if (get_runs(text) != brute_force(text)) return false;
            }
        }
    }
    // 检查同一进程中连续处理不同长度的字符串。
    for (const char* text : {"aaaa", "banana", "a", "abababa", "abcabcabc"}) {
        if (get_runs(text) != brute_force(text)) return false;
    }
    return true;
}
} // namespace runs_test

int main() {
    if (!hash_test::substring_queries() || !hash_test::fast_mod_check()) {
        cerr << "String Hash regression failed\n";
        return 1;
    }
    if (!manacher_test::repeated_reads()) {
        cerr << "Manacher repeated-read regression failed\n";
        return 1;
    }
    if (!lyndon_test::minimum_suffixes()) {
        cerr << "Lyndon minimum-suffix regression failed\n";
        return 1;
    }
    if (!runs_test::exhaustive_runs()) {
        cerr << "Runs exhaustive regression failed\n";
        return 1;
    }
    cerr << "string tests passed\n";
    return 0;
}
