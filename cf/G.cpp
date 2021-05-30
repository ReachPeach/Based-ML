//
// Created by maxim on 25.11.2020.
//

#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using pii = std::pair<int, int>;
using namespace std;

struct DT_node {
    int num = -1;

    bool isLeaf = true;
    int stored_class = -1;

    int f_index = -1;
    double splitter = -1;
    DT_node *L = nullptr, *R = nullptr;


    void makeLeaf(int &y) {
        stored_class = y;
        isLeaf = true;
    }

    void makeInner(int fi, double split, DT_node *l, DT_node *r) {
        this->isLeaf = false;
        this->f_index = fi;
        this->splitter = split;
        this->L = l;
        this->R = r;
    }

    int dfs(int counted = 0) {
        if (!isLeaf) {
            num = ++counted;
            counted = L->dfs(counted);
            counted = R->dfs(counted);
        } else {
            num = ++counted;
        }
        return counted;
    }

    void print() const {
        if (isLeaf) {
            cout << "C " << stored_class << '\n';
        } else {
            cout << "Q " << f_index + 1 << " " << splitter << " " << L->num << " " << R->num << '\n';
            L->print();
            R->print();
        }
    }
};

vector<int> countClasses(vector<int> &a, int &M) {
    vector<int> count(M + 1, 0);
    for (int &k : a) count[k]++;
    return count;
}

pii majoranta(vector<int> &a, int &M) {
    auto count = countClasses(a, M);
    int r = -1, m = -1;
    for (int i = 1; i <= M; i++) {
        if (count[i] > m) {
            m = count[i];
            r = i;
        }
    }
    return {r, m};
}

double entropy(vector<int> &a, int &M) {
    auto count = countClasses(a, M);
    double ans = 0;
    for (int i = 1; i <= M; i++) {
        if (count[i]) {
            double pi = 1.0 * count[i] / a.size();
            ans -= pi * log(pi);
        }
    }
    return ans;
}

double single_val_entr(double x) {
    return x == 0 ? 0 : x * log(x);
}

struct DT {
    int k, h;

    DT(int &k, int &h) : k(k), h(h) {}

    DT_node build(vector<vector<int>> &xs, int &m, vector<int> &ys, int cur_h) {
        DT_node v{};
        pii leaf_ans = majoranta(ys, k);
        auto counter = countClasses(ys, k);

        if (cur_h == h || leaf_ans.second == ys.size()) {
            v.makeLeaf(leaf_ans.first);
        } else {
            double best_entr = 1e8, best_splitter;
            int best_fi;

            double cur_entr = entropy(ys, k);
            for (int fi = 0; fi < m; fi++) {

                vector<pii> xs_fi(xs.size());
                for (int i = 0; i < xs.size(); i++) {
                    xs_fi[i] = {xs[i][fi], ys[i]};
                }
                sort(xs_fi.begin(), xs_fi.end());


                double l_enrt = 0, r_entr = cur_entr;
                vector<int> countL(k + 1, 0), countR = counter;

                int n = xs_fi.size();
                for (int i = 0, lsz = 0, rsz = n; i < n;) {
                    do {
                        i++;

                        int rc = countR[xs_fi[i - 1].second];
                        int lc = countL[xs_fi[i - 1].second];

                        if (rsz == 1) {
                            r_entr = 0;
                        } else {
                            r_entr += (rsz - rc) * log((rsz - 1) * 1.0 / rsz) / n;
                            if (rc != 1) {
                                double sum_without =
                                        rc * log(rc * (rsz - 1) * 1.0 / (rsz * (rc - 1))) / n;
                                double sum_with = log((rc - 1) * 1.0 / (rsz - 1)) / n;
                                r_entr += sum_without + sum_with;
                            } else {
                                r_entr += log(1.0 / (rsz)) / n;
                            }
                        }

                        if (lsz) {
                            l_enrt += (lsz - lc) * log((lsz + 1) * 1.0 / lsz) / n;
                            if (lc) {
                                double sum_without =
                                        lc * log(lc * (lsz + 1) * 1.0 / (lsz * (lc + 1))) / n;
                                double sum_with = log((lc + 1) * 1.0 / (lsz + 1)) / n;
                                l_enrt += sum_without - sum_with;
                            } else {
                                l_enrt -= log(1.0 / (lsz + 1)) / n;
                            }
                        } else {
                            l_enrt = 0;
                        }

                        rsz--;
                        lsz++;
                        countL[xs_fi[i - 1].second]++;
                        countR[xs_fi[i - 1].second]--;
                    } while (i < xs.size() && xs_fi[i].first == xs_fi[i - 1].first);

                    if (i == n) break;

                    double splitter = (1.0 * xs_fi[i].first + xs_fi[i - 1].first) / 2;
                    double minimize = l_enrt + r_entr;
                    if (minimize < best_entr) {
                        best_entr = minimize;
                        best_fi = fi;
                        best_splitter = splitter;
                    }
                }
            }

            auto *left = new DT_node, *right = new DT_node;

            vector<vector<int>> l_xs, r_xs;
            vector<int> l_ys, r_ys;
            for (int i = 0; i < xs.size(); i++) {
                if (xs[i][best_fi] < best_splitter) {
                    l_xs.emplace_back(xs[i]);
                    l_ys.emplace_back(ys[i]);
                } else {
                    r_xs.emplace_back(xs[i]);
                    r_ys.emplace_back(ys[i]);
                }
            }
            *left = build(l_xs, m, l_ys, cur_h + 1);
            *right = build(r_xs, m, r_ys, cur_h + 1);
            v.makeInner(best_fi, best_splitter, left, right);
        }
        return v;
    }
};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n, k, h;
    cin >> m >> k >> h >> n;
    vector<vector<int>> xs(n, vector<int>(m));
    vector<int> ys(n);
    for (int i = 0; i < n; i++) {
        for (auto &x : xs[i]) cin >> x;
        cin >> ys[i];
    }
    DT res(k, h);

    DT_node tree = res.build(xs, m, ys, 0);
    cout << tree.dfs() << "\n";
    tree.print();

}