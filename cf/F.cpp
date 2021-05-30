//
// Created by maxim on 24.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

ld pr_w(string &word, int &a, int &y, vector<map<string, int>> &data, vector<int> &counter) {
    return (0.0 + data[y][word] + a) / (counter[y] + 2 * a);
}

void solve(set<string> &mes, int &n, int &a, vector<int> &hs,
           vector<int> &class_counter, vector<map<string, int>> &class_dict, set<string> &all) {
    vector<ld> ps(hs.size());
    for (int i = 0; i < hs.size(); i++) {
        ld key = log(hs[i] * 1.0 * class_counter[i] / n);
        for (auto word : all) {
            ld p = pr_w(word, a, i, class_dict, class_counter);
            key += log(mes.count(word) ? p : 1 - p);
        }
        ps[i] = exp(key);
    }
    cout.precision(10);
    ld sum = 0;
    for (auto &p : ps) sum += p;
    for (auto &p : ps) cout << p / sum << " ";

    cout << "\n";
}

void test() {
    int k, a, n, m;
    cin >> k;
    vector<int> hk(k), class_counter(k, 0);
    vector<map<string, int>> dict(k, map<string, int>());
    set<string> all;
    for (auto &h : hk) cin >> h;
    cin >> a >> n;
    for (int i = 0, count, cl; i < n; i++) {
        cin >> cl >> count;
        cl--;
        class_counter[cl]++;
        vector<string> mes(count);
        set<string> used;
        for (auto &word : mes) {
            cin >> word;
            if (used.count(word)) continue;
            all.insert(word);
            dict[cl][word]++;
            used.insert(word);
        }
    }

    cin >> m;
    for (int i = 0, count; i < m; i++) {
        cin >> count;
        vector<string> mes(count);
        set<string> used;
        for (auto &word : mes) {
            cin >> word;
            used.insert(word);
        }
        mes.clear();
        for (auto &x : used) mes.emplace_back(x);
        solve(used, n, a, hk, class_counter, dict, all);
    }
}

int main() {
    test();
    return 0;
}