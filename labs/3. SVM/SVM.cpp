//
// Created by maxim on 28.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using pdd = std::pair<double, double>;
using namespace std;

double u(int &j, vector<int> &ys, vector<vector<double>> &xs, vector<double> &hs) {
    double ans = 0;
    for (int i = 0; i < xs.size(); i++) {
        ans += ys[i] * hs[i] * xs[i][j];
    }
    return ans;
}

double cur_b;

double
perform(int n, vector<vector<double>> &ks, vector<int> &ys, double &C, vector<double> &h_ans, bool cross = false) {
    vector<double> hs(n, 0);
    double b = 0;


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
    cur_b = b;
    if (!cross) {
        int count = 0;
        for (int i = 0; i < n; i++) {
            double y_pr = (u(i, ys, ks, hs) + b);
            if (ys[i] * y_pr > 0) {
                count++;
            }
        }
        h_ans = hs;
        auto temp = 1.0 * count / n;
        return 1.0 * count / n;
    } else {
        return ys[n] * (u(n, ys, ks, hs) + b) > 0 ? 1 : 0;
    }
}

double scalar(pdd &x1, pdd &x2) {
    return x1.first * x2.first + x1.second * x2.second;
}

double squared_norm(pdd &x1, pdd &x2) {
    return (x1.first - x2.first) * (x1.first - x2.first) + (x1.second - x2.second) * (x1.second - x2.second);
}

function<double(pdd &x1, pdd &x2)> pol_gen(int n) {
    return [n](pdd &x1, pdd &x2) { return pow(1 + scalar(x1, x2), n); };
}

function<double(pdd &x1, pdd &x2)> rad_gen(double beta) {
    return [beta](pdd &x1, pdd &x2) { return exp(-beta * squared_norm(x1, x2)); };
}

double perform_cross(int &n, vector<vector<double>> &ks, vector<int> &ys, double &C) {
    int ok = 0;
    for (int i = 0; i < n; i++) {
        swap(ks[i], ks[n - 1]);
        swap(ys[i], ys[n - 1]);
        vector<double> not_used;
        if (perform(n - 1, ks, ys, C, not_used, true)) ok++;
        swap(ks[i], ks[n - 1]);
        swap(ys[i], ys[n - 1]);

    }
    return 1.0 * ok / n;
}

void solve(vector<function<double(pdd &xi, pdd &xj)>> &funcs, vector<double> &cs,
           vector<pdd> &xs, vector<int> &ys, int &n, bool testing = true) {
    double best_accuracy = 0, best_c;
    int best_ind;

    for (int k = 0; k < funcs.size(); k++) {
        auto kern_func = funcs[k];
        vector<vector<double>> ks(n, vector<double>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ks[i][j] = kern_func(xs[i], xs[j]);
            }
        }
        for (double &c : cs) {
            vector<double> not_used;
            auto accuracy = perform(n, ks, ys, c, not_used);
            if (accuracy > best_accuracy) {
                best_accuracy = accuracy;
                best_ind = k;
                best_c = c;
            }
        }
    }

    clog << "done. accuracy=" << best_accuracy << ", best_parametr_ind=" << best_ind << ", best c=" << best_c << '\n';
}

ofstream log_ans("../labs/3. SVM/graphics_info.txt");

double u_test(vector<int> &ys, vector<double> &xs, vector<double> &hs) {
    double ans = 0;
    for (int i = 0; i < xs.size(); i++) {
        ans += ys[i] * hs[i] * xs[i];
    }
    return ans + cur_b;
}

void solve1(const function<double(pdd &xi, pdd &xj)> &kern_func, vector<pdd> &xs, vector<int> &ys, double c) {
    int n = xs.size();
    vector<vector<double>> ks(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ks[i][j] = kern_func(xs[i], xs[j]);
        }
    }
    vector<double> ans;
    perform(n, ks, ys, c, ans);
    log_ans << n << '\n';
    for (int i = 0; i < n; i++) {
        auto pred = u(i, ys, ks, ans) + cur_b;
        log_ans << xs[i].first << " " << xs[i].second << " " << (pred * ys[i] > 0 ? "True" : "False")
                << "\n";
    }
    double min_y = INT32_MAX, max_y = INT32_MIN, min_x = INT32_MAX, max_x = INT32_MIN;
    for (int i = 0; i < n; i++) {
        max_x = max(max_x, xs[i].first);
        max_y = max(max_y, xs[i].second);
        min_x = min(min_x, xs[i].first);
        min_y = min(min_y, xs[i].second);
    }
    vector<double> x_generated, y_generated;
    set<pdd> cont(xs.begin(), xs.end());
    for (int i = 0; i <= 500; i++) {
        auto n_x = min_x + i * (max_x - min_x) / 500, n_y = min_y + i * (max_y - min_y) / 500;
        if (!cont.count({n_x, n_y})) {
            x_generated.emplace_back(n_x);
            y_generated.emplace_back(n_y);
        }
    }
    log_ans << x_generated.size() * y_generated.size() << "\n";
    for (double &i1 : x_generated) {
        for (double &i2 : y_generated) {
            vector<double> kss(n);
            for (int j = 0; j < n; j++) {
                pdd cur = {i1, i2};
                kss[j] = kern_func(cur, xs[j]);
            }
            auto pred = u_test(ys, kss, ans);
            log_ans << i1 << " " << i2 << " " << pred << '\n';
        }
    }

}

void test(const char *name, bool testing = true) {
    ifstream in(name);
    int n;
    vector<pdd> xs;
    vector<int> ys;
    string p;
    in >> p;
    while (in >> p) {
        int pos1 = p.find(',');
        ld x1 = stof(p.substr(0, pos1));
        int pos2 = p.find(',', pos1 + 1);
        ld x2 = stof(p.substr(pos1 + 1, pos2));
        int y = p.back() == 'N' ? -1 : 1;
        xs.emplace_back(x1, x2);
        ys.emplace_back(y);
    }
    n = xs.size();

    if (testing) {
        vector<int> ns = {2, 3, 4, 5};
        vector<double> betas;
        for (int i = 2; i <= 10; i++) {
            betas.emplace_back(i / 2.0);
        }
        vector<double> cs = {0.05, 0.1, 0.5, 1.0, 5.0, 10.0, 50.0, 100.0};

        vector<function<double(pdd &xi, pdd &xj)>> funcs_1, funcs_2, funcs_3;
        funcs_1.emplace_back(scalar);
        for (int &used_n : ns) funcs_2.emplace_back(pol_gen(used_n));
        for (double &beta : betas) funcs_3.emplace_back(rad_gen(beta));

        solve(funcs_1, cs, xs, ys, n);
        solve(funcs_2, cs, xs, ys, n);
        solve(funcs_3, cs, xs, ys, n);
    } else {

        if (!strcmp(name, "../labs/3. SVM/chips.csv")) {
            solve1(scalar, xs, ys, 100.0);
            solve1(pol_gen(4), xs, ys, 5.0);
            solve1(rad_gen(5), xs, ys, 100.0);
        } else {
            solve1(scalar, xs, ys, 0.1);
            solve1(pol_gen(2), xs, ys, 0.5);
            solve1(rad_gen(5), xs, ys, 100.0);
        }
    }
}

/*
chips-lin. accuracy=0.559322, best c=100
chips-pol. accuracy=0.864407, best_n=4, best c=5
chips-rad. accuracy=0.889831, best_beta=5, best c=100
geyser-lin. accuracy=0.905405, best c=0.1
geyser-pol. accuracy=0.90991, best_n=2, best c=1
geyser-rad. accuracy=0.95045, best_beta=5, best c=100
 */

int main() {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    bool testing = false;
    vector<string> files = {"../labs/3. SVM/chips.csv", "../labs/3. SVM/geyser.csv"};
    for (auto &file : files) {
        test(file.c_str(), testing);
    }
    return 0;
}