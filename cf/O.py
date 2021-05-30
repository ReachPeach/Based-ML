if __name__ == '__main__':
    k = int(input())
    n = int(input())
    cls = [[] for _ in range(k)]
    for i in range(n):
        x, y = map(int, input().split())
        cls[x - 1].append(y)
    ans = 0
    for i in range(k):
        for y in cls[i]:
            ans += y * y
    ans /= n

    res = 0
    for i in range(k):
        if len(cls[i]) == 0:
            continue
        s = 0
        for y in cls[i]:
            s += y
        s /= n
        res += s * s * n / len(cls[i])
    print(ans - res)
