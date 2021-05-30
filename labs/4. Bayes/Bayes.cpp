//
// Created by maxim on 29.11.2020.
//

#include <bits/stdc++.h>
#include <dirent.h>


using ll = long long;
using ld = long double;
namespace fs = std::filesystem;
using namespace std;
vector<pair<double, double>> for_roc;

struct n_gram {
    vector<string> data;

    n_gram(int n, vector<string> &text) {
        if (text.size() >= n)
            for (int i = 0; i <= text.size() - n; i++) {
                string n_word;
                for (int j = 0; j < n; j++) {
                    n_word += text[i + j] + "$";
                }
                data.emplace_back(n_word);
            }
    }
};

vector<string> split_on(string &origin, char splitter) {
    vector<string> r;
    string temp;
    for (char &c : origin) {
        if (c == splitter) {
            r.emplace_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) r.emplace_back(temp);
    return r;
}

ld pr_w(string &word, double &a, int &y, vector<unordered_map<string, int>> &data, vector<int> &counter) {
    return (0.0 + data[y][word] + a) / (counter[y] + 2 * a);
}

int solve(unordered_set<string> &mes, int &n, double &a, vector<double> &hs, vector<int> &class_counter,
          vector<unordered_map<string, int>> &class_dict, unordered_set<string> &all) {
    vector<double> r(hs.size());
    for (int i = 0; i < hs.size(); i++) {
        ld key = log(hs[i] * 1.0 * class_counter[i] / n);
        for (auto word : all) {
            ld p = pr_w(word, a, i, class_dict, class_counter);
            key += log(mes.count(word) ? p : 1 - p);
        }
        r[i] = key;
    }
    pair<double, int> best = {r[0], 0};
    for (int i = 0; i < r.size(); i++) best = max(best, {r[i], i});
    return best.second;
}

//return accuracy
double test(double a, int n_gramm, int k_fold_index, vector<double> &hk, bool save_legit) {
    int k = 2, n;
    for_roc.clear();
    //reading files
    vector<string> dir_names(10);
    for (int i = 1; i <= 10; i++) dir_names[i - 1] = "./../labs/4. Bayes/part" + to_string(i);
    string test_dir = dir_names[k_fold_index];
    dir_names.erase(dir_names.begin() + k_fold_index);

    vector<string> teach_paths, test_paths;
    for (int i = 0; i < 9; i++) {
        string path = dir_names[i];
        for (const auto &entry : filesystem::directory_iterator(path))
            teach_paths.push_back(entry.path());
    }
    for (const auto &entry : filesystem::directory_iterator(test_dir))
        test_paths.push_back(entry.path());


    vector<int> class_counter(k, 0);
    vector<unordered_map<string, int>> dict(k, unordered_map<string, int>());
    unordered_set<string> all;
    for (string &teach_path : teach_paths) {
        ifstream in(teach_path);
        int cls = teach_path.find("legit") == string::npos ? 0 : 1;

        class_counter[cls]++;
        string title, text;
        getline(in, title);
        getline(in, text);
        getline(in, text);
        if (title.back() == '\n') title.pop_back();
        if (text.back() == '\n') text.pop_back();

        title = title.substr(title.find(':') + 2);
        auto title_words = split_on(title, ' '), text_words = split_on(text, ' ');
        unordered_set<string> used;
        vector<string> file_ngrams = n_gram(n_gramm, title_words).data;
        auto temp = n_gram(n_gramm, text_words).data;
        file_ngrams.insert(file_ngrams.end(), temp.begin(), temp.end());
        for (auto &n_word : file_ngrams) {
            if (used.count(n_word)) continue;
            all.insert(n_word);
            dict[cls][n_word]++;
            used.insert(n_word);
        }
    }

    n = teach_paths.size();
    int up = 0, down = 0;
    int ncorrect_legit = 0;
    for (string &test_path : test_paths) {
        ifstream in(test_path);
        int cls = test_path.find("legit") == string::npos ? 0 : 1;
        string title, text;
        getline(in, title);
        getline(in, text);
        getline(in, text);
        if (title.back() == '\n') title.pop_back();
        if (text.back() == '\n') text.pop_back();

        title = title.substr(title.find(':') + 2);
        auto title_words = split_on(title, ' '), text_words = split_on(text, ' ');
        unordered_set<string> used;
        vector<string> file_ngrams = n_gram(n_gramm, title_words).data;
        auto temp = n_gram(n_gramm, text_words).data;
        file_ngrams.insert(file_ngrams.end(), temp.begin(), temp.end());
        for (auto &n_word : file_ngrams) {
            used.insert(n_word);
        }
        int pred = solve(used, n, a, hk, class_counter, dict, all);
        if (pred == cls) {
            up++;
        }
        if (cls && !pred) ncorrect_legit++;
        down++;
        for_roc.emplace_back(pred, cls);
    }
    if (save_legit) {
        return ncorrect_legit;
    } else {
        return 1.0 * up / down;
    }
}

vector<double> calc_parametrs(int n_gramm) {
    double h_spam = 1;
    double best_h_legit, best_a, best_acc = -1;
    for (double h_ = 1e-32; h_ <= 1e32; h_ *= 10) {
        //for (double a = 1e-8; a <= 1e1; a *= 10) {
        double a = 1e-8;
        double mean_acc = 0;
        vector<double> hs = {h_spam, h_};
        for (int i = 0; i < 10; i++) {
            mean_acc += test(a, n_gramm, i, hs, false);
        }
        mean_acc /= 10;
        if (mean_acc > best_acc) {
            best_acc = mean_acc;
            best_h_legit = h_;
            best_a = a;
        }
        clog << '\n' << a << " " << h_ << " " << mean_acc;
        //}
    }
    return {best_acc, best_a, best_h_legit};
}

double find_h_legit(int n, double a) {
    double h_spam = 1, h_legit_first = -1;
    for (double h_legit = 1e-32; h_legit <= 1e32; h_legit *= 10) {
        vector<double> hs = {h_spam, h_legit};
        auto count = test(a, n, 0, hs, true);
        if (!count) {
            h_legit_first = h_legit;
        }
        clog << "Whith h_legit=" << h_legit << " failed in " << count << " legits\n";
    }
    return h_legit_first;
}

int main() {
    bool test_params = false, draw_roc = true, search_h_legit = false, draw_accuracy = true;
    if (test_params) {
        auto res = calc_parametrs(1);
        cout << "Best params are: alpha=" << res[1] << ", h_legit=" << res[2] << ", accuracy is " << 100 * res[0]
             << "%\n";
    }
    if (search_h_legit) {
        cout << "If i found such value, it is positive and=" << find_h_legit(1, 1e-8) << '\n';
    }
    if (draw_roc) {
        ofstream dots("../labs/4. Bayes/dots.txt");
        double a = 1e-8;
        vector<double> hs = {1, 1e-21};
        test(a, 1, 0, hs, false);
        dots << for_roc.size() << '\n';
        sort(for_roc.begin(), for_roc.end());
        reverse(for_roc.begin(), for_roc.end());
        for (auto &ps : for_roc) dots << ps.first << " " << ps.second << '\n';
    }
    if (draw_accuracy) {
        ofstream dots("../labs/4. Bayes/graps.txt");
        double a = 1e-8;
        dots << "100\n";
        for (double i = 1e-2; i <= 1; i += 0.01) {
            vector<double> hs = {1, i};
            dots << i << " " << test(a, 1, 0, hs, false) << '\n';
        }
    }
    return 0;
}

/*
    Best params are: alpha=1e-08, h_legit=1e-21, accuracy is 97.9817%
    If i found such value, it is positive and=0.01

 */

