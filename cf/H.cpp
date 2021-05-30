//
// Created by maxim on 04.01.2021.
//
#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

int calc(vector<int> &x) {
    int res = 0;
    for (int i = 0; i < x.size(); i++) res |= x[i] << i;
    return res;
}

void increase(vector<int> &x, int pos = 0, bool flag = true) {
    if (flag)
        reverse(x.begin(), x.end());
    if (pos == x.size()) x.assign(x.size(), 0);
    else {
        if (!x[pos]) x[pos] = 1;
        else {
            x[pos] = 0;
            increase(x, pos + 1, false);
        }
    }
    if (flag)
        reverse(x.begin(), x.end());
}

vector<int> uncalc(int &x, int &M) {
    vector<int> r(M);
    for (int i = 0; i < M; i++) if ((x & (1 << i)) > 0) r[i] = 1;
    return r;
}

void solve(vector<int> x, int val, int &M) {
    if (x.empty()) {
        cout << "1\n1\n";
        for (int i = 0; i < M; i++) cout << "0 ";
        cout << -2 * val + 1;
    } else {
        double mul = 2 * val - 1;
        cout << "2\n" << x.size() << " 1\n";
        for (int &cur : x) {
            auto st = uncalc(cur, M);

            int count = 0;
            for (int i = M - 1; i >= 0; i--) {
                if (st[i]) {
                    cout << 1 << " ";
                    count++;
                } else {
                    cout << -1 << " ";
                }
            }
            cout << -(count - 0.5) << '\n';
        }
        for (int i = 0; i < x.size(); i++) cout << mul << " ";
        cout << -0.5 * mul << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int M;
    cin >> M;
    vector<vector<int>> fs(2, vector<int>());
    vector<int> cur(M, 0);
    for (int i = 0, val; i < (1 << M); i++) {
        cin >> val;
        fs[val].emplace_back(calc(cur));
        increase(cur);
    }
    int fs0 = fs[0].size(), fs1 = fs[1].size();
    solve(fs0 < fs1 ? fs[0] : fs[1], fs0 < fs1 ? 0 : 1, M);
}

