//
// Created by maxim on 25.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

int main() {
    int k, n;
    cin >> k >> n;
    vector<vector<ll>> cls(k, vector<ll>());
    for (int i = 0, x, y; i < n; i++) {
        cin >> x >> y;
        x--;
        cls[x].push_back(y);
    }

    //M(M(y^2|x)) = M(y^2)

    ld ans = 0;

    for (int i = 0; i < k; i++) {
        ld sum_sq = 0;
        for (auto &y : cls[i]) {
            sum_sq += (1.0 * y / n) * y;
        }
        ans += sum_sq;
    }

    //M( M(y|x)^2)
    ld res = 0;
    for (int i = 0; i < k; i++) {
        if (cls.empty()) continue;
        ld sum = 0;
        for (auto &y : cls[i]) {
            sum += y;
        }
        res += sum / (1.0 * n * cls[i].size()) * sum;
    }

    cout << ans - res;


    return 0;
}