#include <bits/stdc++.h>


using ll = long long;
using ld = long double;
using namespace std;
const int M = 1e9 + 7;

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n);
    vector<int> bit_counter(64, 0);
    for (auto &x : a) {
        cin >> x;
        for (int i = 0; i < 60; i++) {
            ll cur = 1LL << i;
            if (cur & x) {
                bit_counter[cur]++;
            }
        }
    }
    ll sum = 0;
    for (auto &x : a) {

    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cout << "hello world";
    return 0;
    cin >> n;
    while (n--) solve();
    return 0;
}
