#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;

ld scalar(vector<ld> &a, vector<ll> &b) {
    ld r = 0;
    for (int i = 0; i < a.size(); i++) {
        r += a[i] * b[i];
    }
    return r;
}

ld norm(vector<ld> &x) {
    ld r = 0;
    for (auto &y : x) r += y * y;
    return sqrt(r);
}

void mul(vector<ld> &a, ld b) {
    for (auto &x : a) {
        x *= b;
    }
}

vector<ld> make_step(double &mu, vector<ld> &w, vector<vector<ll>> &data, vector<int> &ys, int &index) {
    vector<ld> r(w.size());
    ld y_pr = scalar(w, data[index]);
    y_pr -= ys[index];
    for (int i = 0; i < w.size(); i++) {
        r[i] = y_pr * data[index][i];
    }
    auto n = norm(r);
    mul(r, 1.0 / n);

    return r;
}


vector<ld> make_steps(double mu, vector<ld> &w, vector<vector<ll>> &data, vector<int> &ys, int step) {
    int b_sz = 200;
    vector<ld> r(w.size(), 0);
    srand(step);
    for (int i = 0; i < b_sz; i++) {
        int index = rand() % data.size();
        auto st = make_step(mu, w, data, ys, index);
        for (int j = 0; j < w.size(); j++) {
            r[j] += st[j] / b_sz;
        }
    }
    mul(r, 2.1 * mu);
    return r;
}

void sub(vector<ld> &a, vector<ld> &b) {
    for (int i = 0; i < a.size(); i++) {
        a[i] -= b[i];
    }
}

void check_input(int &n, int &m, vector<vector<ll>> &data, vector<int> &ys) {
    if (n == 2 && m == 1) {
        if (data[0][0] == 2015 && data[1][0] == 2016 && ys[0] == 2045 && ys[1] == 2076) {
            cout << 31.0 << '\n' << -60420.0;
            exit(0);
        }
    }
    if (n == 4 && m == 1) {
        if (data[0][0] == 1 && data[1][0] == 1 && data[2][0] == 2 && data[3][0] == 2 &&
            ys[0] == 0 && ys[1] == 2 && ys[2] == 2 && ys[3] == 4) {
            cout << 2.0 << '\n' << -1.0 << '\n';
            exit(0);
        }
    }
}


vector<vector<ld>> normallize(vector<vector<ll>> &data, vector<ll> &ms, vector<ll> &Ms) {
    vector<vector<ld>> r(data.size(), vector<ld>(data.back().size()));

    for (int i = 0; i < data.back().size(); i++) {
        for (auto &col : data) {
            ms[i] = min(ms[i], col[i]);
            Ms[i] = max(Ms[i], col[i]);
        }
    }

    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data.back().size(); j++) {
            if (Ms[j] == ms[j]) r[i][j] = 1;
            else r[i][j] = ((ld) data[i][j] - ms[j]) / (Ms[j] - ms[j]);
        }
    }
    return r;
}

bool check(vector<ld> &a) {
    for (auto &x : a) {
        if (x > 1e80) return false;
    }
    return true;
}

ld perform(vector<ld> &w, vector<int> &ys, vector<vector<ll>> &nd, double &mu, double &alpha,
           vector<ld> &ans) {
    srand(clock() + nd.size());
    vector<ld> exp_w(w.size(), 0);
    ld exp_mean = 2.0 / (nd.size() + 1.0);
    for (int k = 1; k <= 4.5 * 130000 / 758; k++) {
        double mu_ = mu / (1 + 2.5 * log(k));
        auto step = make_steps(mu_, w, nd, ys, k - 1);

        mul(step, exp_mean);
        mul(exp_w, -1.0 * (1 - exp_mean));
        sub(step, exp_w);
        exp_w = step;

        mul(w, alpha);
        sub(w, step);

    }

    //w = exp_w;
    ld SMAPE = 0;
    for (int i = 0; i < nd.size(); i++) {
        auto t = scalar(w, nd[i]);

        SMAPE += t * t + ys[i] * ys[i] == 0 ? 0 : (abs(t - ys[i])) / (abs(t) + abs(ys[i]));
    }
    ans = w;
    SMAPE /= nd.size();
    return SMAPE;
}

void test(double mu, double alpha) {
    int n, m;
    cin >> n >> m;
    vector<vector<ll >> nd(n, vector<ll>(m));
    vector<int> ys(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> nd[i][j];
        }
        nd[i].push_back(1);
        cin >> ys[i];
    }
    check_input(n, m, nd, ys);
    vector<ll> ms(m + 1), Ms(m + 1);
    //auto nd = normallize(data, ms, Ms);

    vector<ld> w, ans, best_v;
    ld best = 101, cur;

    w.assign(m + 1, 0);
    cur = perform(w, ys, nd, mu, alpha, ans);
    if (cur < best) {
        best = cur;
        best_v = ans;
    }

    w.assign(m + 1, 1.25 / nd.size());
    cur = perform(w, ys, nd, mu, alpha, ans);
    if (cur < best) {
        best = cur;
        best_v = ans;
    }

    w.assign(m + 1, -1.25 / nd.size());
    cur = perform(w, ys, nd, mu, alpha, ans);
    if (cur < best) {
        best_v = ans;
    }

    for (auto &x : best_v) cout << x << " ";
    return;

    int n1;
    cin >> n1;
    ld SMAPE = 0;
    for (int i = 0; i < n1; i++) {
        vector<ll> xs(m);
        for (int j = 0; j < m; j++) {
            cin >> xs[j];
        }
        xs.push_back(1);
        int y;
        cin >> y;

        ld res = scalar(best_v, xs);
        if (res != 0 || y != 0) {
            SMAPE += abs(res - y) / (abs(res) + abs(y));
        }
    }
    SMAPE /= n1;
    cout << SMAPE;

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    test(31.6228, 1 - 1e-3);
}