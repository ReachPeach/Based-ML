//
// Created by maxim on 26.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

ld entropy(ld x) {
    return x == 0 ? 0 : x * log(x);
}

int main() {
    int k1, k2, n;
    cin >> k1 >> k2 >> n;
    map<int, map<int, int>> cr1, cr2;
    vector<int> u1(k1, 0), u2(k2, 0);
    vector<int> ok(k1, n);
    for (int i = 0, x, y; i < n; i++) {
        cin >> x >> y;
        x--;
        y--;
        cr1[x][y]++;
        u1[x]++;
    }


    ld ans = 0;

    for (auto &t : cr1) {
        int x = t.first;
        ld pi = 1.0 * u1[x] / n;
        ld s = 0;
        for (auto &p : t.second) {
            s += 1.0 * p.second * log(1.0 * p.second / u1[x]) / u1[x];
        }
        ans += s * pi;
    }

    cout.precision(10);
    cout << -ans;
    return 0;
}