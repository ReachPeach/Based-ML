//
// Created by maxim on 25.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using pii = std::pair<int, int>;
using namespace std;

void check(vector<pii> &a) {
    set<int> xs, ys;
    for (auto &p : a) {
        xs.insert(p.first);
        ys.insert(p.second);
    }
    if (xs.size() == a.size() && ys.size() == a.size()) {
        if (a.size() == 1) {
            cout << 0;
            exit(0);
        }
        vector<int> x(xs.begin(), xs.end()), y(ys.begin(), ys.end());
        ld up = 0;
        for (auto &i : a) {
            ld X = lower_bound(x.begin(), x.end(), i.first) - x.begin() + 1;
            ld Y = lower_bound(y.begin(), y.end(), i.second) - y.begin() + 1;
            up += (X - Y) * (X - Y);
        }

        cout << 1 - 6.0 * up / (1LL * a.size() * a.size() * a.size() - a.size());

        exit(0);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> xs(n), ys(n);
    vector<pii> ps(n);
    for (int i = 0; i < n; i++) {
        cin >> xs[i] >> ys[i];
        ps[i] = {xs[i], ys[i]};
    }
    check(ps);
    exit(1);
}