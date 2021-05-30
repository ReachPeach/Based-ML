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
    vector<vector<ll>> store(k, vector<ll>());
    vector<ll> all(n);
    for (int i = 0, x, y; i < n; i++) {
        cin >> x >> y;
        y--;
        store[y].push_back(x);
        all[i] = x;
    }

    //между всеми
    sort(all.begin(), all.end());
    ll all_sum = 0;
    vector<ll> pref_all(n);
    pref_all[0] = all[0];
    for (int i = 1; i < n; i++) {
        pref_all[i] = pref_all[i - 1] + all[i];
    }
    for (int i = 0; i < n; i++) {
        all_sum += pref_all.back() - pref_all[i] - (n - i - 1) * all[i];
        if (i > 0) {
            all_sum += all[i] * i - pref_all[i - 1];
        }
    }
    //


    for (auto &cl: store) sort(cl.begin(), cl.end());
    vector<vector<ll>> prefix_sum(k, vector<ll>());
    for (int i = 0; i < k; i++) {
        auto &cur_pr = prefix_sum[i];
        if (store[i].size() <= 1) continue;
        cur_pr.resize(store[i].size());
        cur_pr[0] = store[i][0];
        for (int j = 1; j < cur_pr.size(); j++) {
            cur_pr[j] = cur_pr[j - 1] + store[i][j];
        }
    }

    //внутриклассовое
    ll ans = 0;
    for (int i = 0; i < k; i++) {
        int sz = prefix_sum[i].size();
        if (sz <= 1) continue;
        for (int j = 0; j < sz; j++) {
            ans += prefix_sum[i].back() - prefix_sum[i][j] - (sz - j - 1) * store[i][j];
            if (j > 0) {
                ans += store[i][j] * (j) - prefix_sum[i][j - 1];
            }
        }
    }


    cout << ans << '\n' << all_sum - ans;
    return 0;
}