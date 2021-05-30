//
// Created by maxim on 25.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

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
        u2[y]++;
    }

    ld ans = n;


    for (auto &a : cr1) {
        for (auto &b : a.second) {
            ld mn = 1.0 * u1[a.first] * u2[b.first] / n;
            ld up = mn - b.second;
            ans += up * up / mn;
            ans -= mn;
        }
    }

    cout.precision(10);
    cout << ans;
    return 0;
}