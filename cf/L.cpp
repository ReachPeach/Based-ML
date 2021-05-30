//
// Created by maxim on 25.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using pii = std::pair<int, int>;
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> xs(n), ys(n);

    ld x_ = 0, y_ = 0;
    for (int i = 0; i < n; i++) {
        cin >> xs[i] >> ys[i];
        x_ += xs[i];
        y_ += ys[i];
    }
    x_ /= n;
    y_ /= n;

    ld up = 0, d1 = 0, d2 = 0;
    for (int i = 0; i < n; i++) {
        up += (xs[i] - x_) * (ys[i] - y_);
        d1 += (xs[i] - x_) * (xs[i] - x_);
        d2 += (ys[i] - y_) * (ys[i] - y_);
    }

    if (d1 == 0 || d2 == 0) cout << 0;
    else {
        cout.precision(10);
        cout << up / sqrt(d1 * d2) << '\n';
    }

}