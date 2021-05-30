//
// Created by maxim on 05.12.2020.
//

#include <bits/stdc++.h>

#include <random>


using ll = long long;
using ld = long double;
using pii = std::pair<int, int>;
using namespace std;

struct DT_node {
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

    int predict(vector<double> &xs) const {
        if (isLeaf) return stored_class;
        if (xs[f_index] < splitter) {
            return L->predict(xs);
        } else {
            return R->predict(xs);
        }
    }
};

map<int, int> countClasses(vector<int> &a, int &M) {
    map<int, int> count;
    for (int &k : a) count[k]++;
    return count;
}

pii majoranta(vector<int> &a, int &M) {
    auto count = countClasses(a, M);
    int r = -1, m = -1;
    for (auto &p : count) {
        if (p.second > m) {
            m = p.second;
            r = p.first;
        }
    }
    return {r, m};
}

struct DT {
    int k, h;

    DT(int k, int &h) : k(k), h(h) {}

    DT_node build_for_random(vector<vector<double>> &xs, vector<int> &ys, int cur_h) {
        DT_node v{};
        pii leaf_ans = majoranta(ys, k);
        auto counter = countClasses(ys, k);

        if (cur_h == h || leaf_ans.second == ys.size()) {
            v.makeLeaf(leaf_ans.first);
        } else {
            int fi = rand() & 1;
            set<double> fichs;
            for (auto &x : xs) fichs.insert(x[fi]);
            vector<double> to_sort(fichs.begin(), fichs.end());
            if (to_sort.size() == 1) {
                v.makeLeaf(leaf_ans.first);
                return v;
            }
            int rand_ind = rand() % (to_sort.size() - 1);
            double splitter = (to_sort[rand_ind] + to_sort[rand_ind + 1]) / 2;
            vector<vector<double>> l_xs, r_xs;
            vector<int> l_ys, r_ys;
            for (int i = 0; i < xs.size(); i++) {
                if (xs[i][fi] < splitter) {
                    l_xs.emplace_back(xs[i]);
                    l_ys.emplace_back(ys[i]);
                } else {
                    r_xs.emplace_back(xs[i]);
                    r_ys.emplace_back(ys[i]);
                }
            }
            auto *left = new DT_node, *right = new DT_node;

            *left = build_for_random(l_xs, l_ys, cur_h + 1);
            *right = build_for_random(r_xs, r_ys, cur_h + 1);
            v.makeInner(fi, splitter, left, right);
        }
        return v;
    }

};

struct AdaBoost {
    vector<vector<double>> xs;
    vector<int> ys;
    vector<DT_node *> DTs;
    vector<double> bs;
    vector<int> as;

    int T;

    pair<vector<vector<double>>, vector<int>> make_train() {
        int n = xs.size();
        vector<vector<double>> xs_copy(xs.begin(), xs.end());
        vector<int> ys_copy = ys;

        while (10 * xs_copy.size() >= 3 * n) {
            int n1 = xs_copy.size();
            int rand_ind = rand() % n1;

            swap(xs_copy[rand_ind], xs_copy[n1 - 1]);
            xs_copy.pop_back();
            swap(ys_copy[rand_ind], ys_copy[n1 - 1]);
            ys_copy.pop_back();
        }

        while (2 * xs_copy.size() < n) {
            int n1 = xs_copy.size();
            int rand_ind = rand() % n1;

            xs_copy.emplace_back(xs_copy[rand_ind]);
            ys_copy.emplace_back(ys_copy[rand_ind]);
        }

        return {xs_copy, ys_copy};
    }

    AdaBoost(int h, int A_size, vector<vector<double>> &data, vector<int> &cls) : xs(data), ys(cls) {
        DTs.assign(A_size, new DT_node());
        DT base(1, h);
        for (int i = 0; i < A_size; i++) {
            auto[x_tr, y_tr] = make_train();
            *DTs[i] = base.build_for_random(x_tr, y_tr, 0);
        }
        bs = vector<double>();
        as = vector<int>();
    }

    pair<double, int> boost_iteration(vector<double> &w) {
        vector<double> Ns(xs.size(), 0);
        for (int i = 0; i < DTs.size(); i++) {
            for (int j = 0; j < xs.size(); j++) {
                auto y_pr = DTs[i]->predict(xs[j]);
                if (y_pr ^ ys[j]) {
                    Ns[i] += w[i];
                }
            }
        }
        pair<double, int> r = {Ns[0], 0};
        for (int i = 0; i < Ns.size(); i++) {
            r = min(r, {Ns[i], i});
        }
        return r;
    }


    void boost(int iterations) {
        bs.clear();
        as.clear();
        T = 1;
        vector<double> w(xs.size(), 1.0 / xs.size());
        for (int t = 1; t <= iterations; t++) {
            auto[Nt, at] = boost_iteration(w);

            if (!Nt) break;
            as.push_back(at);
            double bt = 0.5 * log((1 - Nt) / Nt);
            bs.push_back(bt);

            double sum = 0;
            for (int i = 0; i < xs.size(); i++) {
                w[i] *= exp(-bt * (ys[i] == DTs[at]->predict(xs[i]) ? 1 : -1));
                sum += w[i];
            }
            for (auto &ws : w) ws /= sum;
            T = t;
        }
    }

    int predict(vector<double> &x) {
        double sum = 0;
        for (int i = 0; i < T; i++) {
            sum += bs[i] * DTs[as[i]]->predict(x);
        }
        return sum > 0 ? 1 : 0;
    }

    void setMaxHeigth(int &h, int A_size) {
        DTs.assign(A_size, new DT_node());
        DT base(1, h);
        for (int i = 0; i < A_size; i++) {
            auto[x_tr, y_tr] = make_train();
            *DTs[i] = base.build_for_random(x_tr, y_tr, 0);
        }

    }
};

const vector<int> interesting_steps = {1, 2, 3, 5, 8, 13, 21, 34, 55};

void test(const string &name) {
    ifstream in(name);
    string p;
    vector<vector<double>> xs;
    vector<int> ys;
    in >> p;
    while (in >> p) {
        int pos1 = p.find(',');
        double x1 = stof(p.substr(0, pos1));
        int pos2 = p.find(',', pos1 + 1);
        double x2 = stof(p.substr(pos1 + 1, pos2));
        int y = p.back() == 'N' ? 0 : 1;
        xs.push_back({x1, x2});
        ys.emplace_back(y);
    }
    int n = xs.size();
    AdaBoost boost(0, 333, xs, ys);
    for (int h = 2; h <= 2; h++) {
        boost.setMaxHeigth(h, 333);
        for (int iterations : interesting_steps) {
            boost.boost(iterations);
            int acc = 0;
            for (int i = 0; i < n; i++) {
                if (boost.predict(xs[i]) == ys[i]) acc++;
            }
            clog << "For max-height " << h << " with " << iterations << " iterations accuracy is " << acc * 1.0 / n
                 << '\n';
        }
    }
}

int main() {
    test("../labs/6. Boost/chips.csv");
}
