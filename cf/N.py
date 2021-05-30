if __name__ == '__main__':
    k = int(input())
    n = int(input())
    store = [[] for _ in range(k)]
    all = []
    for i in range(n):
        x, y = map(int, input().split())
        store[y - 1].append(x)
        all.append(x)

    all.sort()
    all_sum = 0
    pref_sum = [0 for i in range(n)]
    pref_sum[0] = all[0]
    for i in range(1, n):
        pref_sum[i] = pref_sum[i - 1] + all[i]

    for i in range(n):
        all_sum += pref_sum[-1] - pref_sum[i] - all[i] * (n - i - 1)
        if i > 0:
            all_sum += i * all[i] - pref_sum[i - 1]

    for p_s in store:
        p_s.sort()

    p_s = [[] for _ in range(k)]
    for i in range(k):
        p_s[i].append(store[i][0])
        for j in range(1, len(store[i])):
            p_s[i].append(p_s[i][j - 1] + store[i][j])

    ans = 0
    for i in range(k):
        sz = len(p_s[i])
        for j in range(sz):
            ans += p_s[i][-1] - p_s[i][j] - store[i][j] * (sz - j - 1)
            if j > 0:
                ans += store[i][j] * j - p_s[i][j - 1]

    print(ans)
    print(all_sum - ans)
