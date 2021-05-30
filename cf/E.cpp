//
// Created by maxim on 20.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

double u(int &j, vector<int> &ys, vector<vector<int>> &xs, vector<double> &hs) {
    double ans = 0;
    for (int i = 0; i < xs.size(); i++) {
        ans += ys[i] * hs[i] * xs[i][j];
    }
    return ans;
}


void test() {
    int n;
    cin >> n;
    vector<vector<int>> ks(n, vector<int>(n));
    vector<int> ys;
    for (auto &row : ks) {
        for (auto &elem : row) {
            cin >> elem;
        }
        int y;
        cin >> y;
        ys.emplace_back(y);
    }
    vector<double> hs(n, 0);
    double b = 0;
    int C;
    cin >> C;


    int upper = 5e4;
    double eps = 5e-6;
    for (int k = 0; k < upper;) {
        int start = rand() % n;
        for (int i = 0; i < n && k < upper; i++, k++) {
            int i1 = (start + i) % n, i2 = rand() % n;
            while (i2 == i1) {
                i2 = rand() % n;
            }
            int y1 = ys[i1], y2 = ys[i2];

            double E1 = u(i1, ys, ks, hs) - y1,
                    E2 = u(i2, ys, ks, hs) - y2;
            double h1 = hs[i1], h2 = hs[i2];

            double L, H;
            if (y1 == y2) {
                L = max(0.0, h2 + h1 - C);
                H = min((double) C, h2 + h1);
            } else {
                L = max(0.0, h2 - h1);
                H = min((double) C, C + h2 - h1);
            }
            if (abs(L - H) < eps) continue;

            double eta = ks[i1][i1] + ks[i2][i2] - 2 * ks[i1][i2];
            double a2;
            if (eta > eps) {
                a2 = h2 + y2 * (E1 - E2) / eta;
                if (a2 < L) a2 = L;
                else if (a2 > H) a2 = H;

                if (abs(a2 - h2) < eps) continue;
            } else continue;

            hs[i2] = a2;
            hs[i1] += y1 * y2 * (h2 - a2);
        }
    }

    int st = -1;
    for (int i = 0; i < n; i++) {
        if (hs[i] > eps) {
            if (hs[i] < C - eps) {
                st = i;
                break;
            }
        }
    }
    if (st != -1) {
        b = 1.0 / ys[st] - u(st, ys, ks, hs);
    } else {
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (hs[i] > eps) {
                b += 1.0 / ys[i] - u(i, ys, ks, hs);
                count++;
            }
        }
        b /= count;
    }
    cout.precision(10);
    for (auto &h : hs) cout << h << '\n';
    cout << b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    test();
    return 0;
}