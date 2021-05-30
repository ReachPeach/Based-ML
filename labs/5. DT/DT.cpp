//
// Created by maxim on 29.11.2020.
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

    int predict(vector<int> &xs) const {
        if (isLeaf) return stored_class;
        if (xs[f_index] < splitter) {
            return L->predict(xs);
        } else {
            return R->predict(xs);
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

    DT_node build_for_forest(vector<vector<int>> &xs, int &m, vector<int> &ys, int cur_h, vector<int> &inds_for_fs) {
        DT_node v{};
        pii leaf_ans = majoranta(ys, k);
        auto counter = countClasses(ys, k);

        if (cur_h == h || leaf_ans.second == ys.size()) {
            v.makeLeaf(leaf_ans.first);
        } else {
            double best_entr = 1e8, best_splitter;
            int best_fi;

            double cur_entr = entropy(ys, k);
            for (int &fi : inds_for_fs) {
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
            *left = build_for_forest(l_xs, m, l_ys, cur_h + 1, inds_for_fs);
            *right = build_for_forest(r_xs, m, r_ys, cur_h + 1, inds_for_fs);
            v.makeInner(best_fi, best_splitter, left, right);
        }
        return v;
    }

    DT_node build(vector<vector<int>> &xs, int &m, vector<int> &ys, int cur_h) {
        vector<int> all_inds(m, 0);
        for (int i = 1; i < m; i++) all_inds[i] = all_inds[i - 1] + 1;
        return build_for_forest(xs, m, ys, cur_h, all_inds);
    }
};

pair<double, int> get_opt_height(const string &name_teach, const string &name_test) {
    ifstream teach(name_teach), test(name_test);
    int m, n_teach, n_test, k;
    teach >> m >> k >> n_teach;
    test >> m >> k >> n_test;
    vector<vector<int>> xs_train(n_teach, vector<int>(m)), xs_test(n_test, vector<int>(m));
    vector<int> ys_train(n_teach), ys_test(n_test);
    for (int i = 0; i < n_teach; i++) {
        for (auto &x : xs_train[i]) teach >> x;
        teach >> ys_train[i];
    }
    for (int i = 0; i < n_test; i++) {
        for (auto &x : xs_test[i]) test >> x;
        test >> ys_test[i];
    }
    pair<double, int> r = {0, 0};
    for (int h = 1; h <= 100; h++) {
        DT dt(k, h);
        DT_node tree = dt.build(xs_train, m, ys_train, 0);
        int count_ok = 0;
        for (int i = 0; i < xs_test.size(); i++) {
            auto x_test = xs_test[i];
            int pr_y = tree.predict(x_test);
            if (pr_y == ys_test[i]) {
                count_ok++;
            }
        }
        r = max(r, {1.0 * count_ok / xs_test.size(), h});
    }
    return r;
}


pair<double, double> get_with_forest(const string &name_teach, const string &name_test) {
    ifstream teach(name_teach), test(name_test);
    int m, n_teach, n_test, k;
    teach >> m >> k >> n_teach;
    test >> m >> k >> n_test;
    vector<vector<int>> xs_train(n_teach, vector<int>(m)), xs_test(n_test, vector<int>(m));
    vector<int> ys_train(n_teach), ys_test(n_test);
    for (int i = 0; i < n_teach; i++) {
        for (auto &x : xs_train[i]) teach >> x;
        teach >> ys_train[i];
    }
    for (int i = 0; i < n_test; i++) {
        for (auto &x : xs_test[i]) test >> x;
        test >> ys_test[i];
    }

    vector<DT_node> forest;
    int max_h = INT32_MAX;
    DT dt(k, max_h);
    int m1 = (int) sqrt(m);
    int count_teach = 0, count_test = 0;

    for (int i = 0; i < 333; i++) {
        set<int> used_fis;
        while (used_fis.size() < m1) {
            used_fis.insert(rand() % m);
        }
        vector<int> inds(used_fis.begin(), used_fis.end());
        forest.push_back(dt.build_for_forest(xs_train, m, ys_train, 0, inds));
    }

    for (int i = 0; i < n_teach; i++) {
        vector<int> preds(k + 1, 0);
        for (auto tree : forest) {
            preds[tree.predict(xs_train[i])]++;
        }
        int best = 0, r = -1;
        for (int j = 0; j <= k; j++) {
            if (preds[j] > best) {
                best = preds[j];
                r = j;
            }
        }
        if (r == ys_train[i]) {
            count_teach++;
        }
    }

    for (int i = 0; i < n_test; i++) {
        vector<int> preds(k + 1, 0);
        for (auto tree : forest) {
            preds[tree.predict(xs_test[i])]++;
        }
        int best = 0, r = -1;
        for (int j = 0; j <= k; j++) {
            if (preds[j] > best) {
                best = preds[j];
                r = j;
            }
        }
        if (r == ys_test[i]) {
            count_test++;
        }
    }
    return {1.0 * count_teach / n_teach, 1.0 * count_test / n_test};
}

void make_dots(string &name) {
    string train_name = name + "train.txt";
    string test_name = name + "test.txt";
    string dots_name = name + "dots.txt";
    ifstream teach(train_name), test(test_name);
    ofstream dots_inf(dots_name);
    int m, n_teach, n_test, k;
    teach >> m >> k >> n_teach;
    test >> m >> k >> n_test;
    vector<vector<int>> xs_train(n_teach, vector<int>(m)), xs_test(n_test, vector<int>(m));
    vector<int> ys_train(n_teach), ys_test(n_test);
    for (int i = 0; i < n_teach; i++) {
        for (auto &x : xs_train[i]) teach >> x;
        teach >> ys_train[i];
    }
    for (int i = 0; i < n_test; i++) {
        for (auto &x : xs_test[i]) test >> x;
        test >> ys_test[i];
    }
    pair<double, int> r = {0, 0};
    dots_inf << 100 << '\n';
    for (int h = 1; h <= 100; h++) {
        DT dt(k, h);
        DT_node tree = dt.build(xs_train, m, ys_train, 0);
        int count_ok_train = 0, count_ok_test = 0;
        for (int i = 0; i < xs_test.size(); i++) {
            auto x_test = xs_test[i];
            int pr_y = tree.predict(x_test);
            if (pr_y == ys_test[i]) {
                count_ok_test++;
            }
        }
        for (int i = 0; i < xs_train.size(); i++) {
            auto x_train = xs_train[i];
            int pr_y = tree.predict(x_train);
            if (pr_y == ys_train[i]) {
                count_ok_train++;
            }
        }

        dots_inf << h << " " << 1.0 * count_ok_train / xs_train.size() << " " << 1.0 * count_ok_test / xs_test.size()
                 << '\n';
    }

}


/* Optimal heights for tests
For test 1 optimal height is 3 with accuracy 0.999743
For test 2 optimal height is 9 with accuracy 0.703817
For test 3 optimal height is 3 with accuracy 1
For test 4 optimal height is 5 with accuracy 0.992
For test 5 optimal height is 3 with accuracy 0.995671
For test 6 optimal height is 3 with accuracy 0.998896
For test 7 optimal height is 3 with accuracy 0.996744
For test 8 optimal height is 4 with accuracy 0.997921
For test 9 optimal height is 5 with accuracy 0.852941
For test 10 optimal height is 4 with accuracy 0.997988
For test 11 optimal height is 3 with accuracy 0.999195
For test 12 optimal height is 10 with accuracy 0.874544
For test 13 optimal height is 7 with accuracy 0.657798
For test 14 optimal height is 5 with accuracy 0.990351
For test 15 optimal height is 2 with accuracy 1
For test 16 optimal height is 3 with accuracy 1
For test 17 optimal height is 7 with accuracy 0.843104
For test 18 optimal height is 5 with accuracy 0.942666
For test 19 optimal height is 7 with accuracy 0.835709
For test 20 optimal height is 7 with accuracy 0.969493
For test 21 optimal height is 9 with accuracy 0.811034
 */


/* Random forest results:
For test 1 accuracy for train is 1, test is 0.809818
For test 2 accuracy for train is 1, test is 0.208397
For test 3 accuracy for train is 1, test is 0.865959
For test 4 accuracy for train is 1, test is 0.514783
For test 5 accuracy for train is 1, test is 0.922078
For test 6 accuracy for train is 1, test is 0.590508
For test 7 accuracy for train is 1, test is 0.435814
For test 8 accuracy for train is 1, test is 0.970894
For test 9 accuracy for train is 1, test is 0.35098
For test 10 accuracy for train is 1, test is 0.806036
For test 11 accuracy for train is 1, test is 0.78833
For test 12 accuracy for train is 1, test is 0.487791
For test 13 accuracy for train is 1, test is 0.306422
For test 14 accuracy for train is 1, test is 0.470089
For test 15 accuracy for train is 1, test is 0.95015
For test 16 accuracy for train is 1, test is 0.953894
For test 17 accuracy for train is 1, test is 0.281965
For test 18 accuracy for train is 1, test is 0.546538
For test 19 accuracy for train is 1, test is 0.447862
For test 20 accuracy for train is 1, test is 0.644612
For test 21 accuracy for train is 1, test is 0.31896*/
int main() {
    bool test_heigth = false, test_random = true, draw_graph = false;
    if (test_heigth)
        for (int i = 1; i <= 21; i++) {
            string file_path = "../labs/5. DT/";
            string num = to_string(i);
            if (num.size() == 1) num = "0" + num;
            string train_name = file_path + num + "_train.txt";
            string test_name = file_path + num + "_test.txt";
            auto res = get_opt_height(train_name, test_name);
            clog << "For test " << i << " optimal height is " << res.second << " with accuracy " << res.first << '\n';
        }

    if (test_random) {
        for (int i = 1; i <= 21; i++) {
            string file_path = "../labs/5. DT/";
            string num = to_string(i);
            if (num.size() == 1) num = "0" + num;
            string train_name = file_path + num + "_train.txt";
            string test_name = file_path + num + "_test.txt";
            auto res = get_with_forest(train_name, test_name);
            clog << "For test " << i << " accuracy for train is " << res.first << ", test is " << res.second << '\n';
        }
    }
    if (draw_graph) {
        string file_path_min = "../labs/5. DT/15_";
        string file_path_max = "../labs/5. DT/10_";
        make_dots(file_path_min);
        make_dots(file_path_max);
    }
}