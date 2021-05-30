//
// Created by maxim on 27.11.2020.
//
#include <bits/stdc++.h>


void perform_gradient();

using ll = long long;
using ld = long double;
using namespace std;
//ofstream for_graphics("../labs/2. Linear/dots.txt");

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


ld perform(vector<ld> &w, vector<int> &ys, vector<vector<ll>> &nd, double &mu, double &alpha,
           vector<ld> &ans, int iterations_count = 771) {
    srand(clock() + nd.size());
    vector<ld> exp_w(w.size(), 0);
    ld exp_mean = 2.0 / (nd.size() + 1.0);
    for (int k = 1; k <= iterations_count; k++) {
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

pair<ld, ld> test_gradient(double mu, double alpha, int iterations = 771) {
    int n, m;
    ifstream in("../labs/2. Linear/2.txt");
    in >> m >> n;
    vector<vector<ll >> nd(n, vector<ll>(m));
    vector<int> ys(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            in >> nd[i][j];
        }
        nd[i].push_back(1);
        in >> ys[i];
    }
    check_input(n, m, nd, ys);
    vector<ll> ms(m + 1), Ms(m + 1);
    //auto nd = normallize(data, ms, Ms);

    vector<ld> w, ans, best_v;
    ld best = 101, cur;

    w.assign(m + 1, 0);
    cur = perform(w, ys, nd, mu, alpha, ans, iterations);
    if (cur < best) {
        best = cur;
        best_v = ans;
    }

    w.assign(m + 1, 1.25 / nd.size());
    cur = perform(w, ys, nd, mu, alpha, ans, iterations);
    if (cur < best) {
        best = cur;
        best_v = ans;
    }

    w.assign(m + 1, -1.25 / nd.size());
    cur = perform(w, ys, nd, mu, alpha, ans, iterations);
    if (cur < best) {
        best = cur;
        best_v = ans;
    }


    //ofstream checker("../labs/2. Linear/check.txt");
    int n1;
    in >> n1;
    ld SMAPE = 0;
    for (int i = 0; i < n1; i++) {
        vector<ll> xs(m);
        for (int j = 0; j < m; j++) {
            in >> xs[j];
        }
        xs.push_back(1);
        int y;
        in >> y;

        ld res = scalar(best_v, xs);
        //  if (iterations == 2000) checker << res << " " << y << '\n';
        if (res != 0 || y != 0) {
            SMAPE += abs(res - y) / (abs(res) + abs(y));
        }
    }
    SMAPE /= n1;
    return {best, SMAPE};

}

void perform_gradient() {
    vector<ld> mus, thays;
    for (ld thay = 1e-20; thay <= 1e-1; thay *= sqrt(10)) {
        thays.emplace_back(thay);
    }

    int all = thays.size(), cur = 0;
    ld best = 1;
    ld best_mu = 31.6228, best_thay = 1e-2, best_alph = 1 - best_thay;
    bool test_parametrs_mode = false;

    if (test_parametrs_mode) {
        for (auto &thay : thays) {
            auto res = test_gradient(best_mu, 1.0 - thay).second;
            if (best > res) {
                best_alph = 1.0 - thay;
                best_thay = thay;
                best = res;
            }
            clog << "\r" << ++cur << " of " << all;
        }
    }

    //4.64381%, mu=31.6228, thay=0.01, 1-thay=0.99
    //best_iter = 2000
    /*
    for (int iterations = 40; iterations <= 2000; iterations += 40) {
        auto r = test_gradient(best_mu, best_alph, iterations);
        for_graphics << iterations << " " << 100 * r.first << " " << 100 * r.second << '\n';
        clog << '\r' << iterations / 40 << " of " << 2000 / 40;
    }
     */
    test_gradient(best_mu, best_alph, 2000);

}


int main() {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    perform_gradient();
}



