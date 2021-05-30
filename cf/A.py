if __name__ == '__main__':
    n, m, k = map(int, input().split())
    cl = [int(i) for i in input().split()]
    ch = [0 for i in range(n)]

    pairs = [[cl[i], i] for i in range(n)]
    pairs.sort()
    d = [[] for i in range(k)]
    for i in range(n):
        d[i % k].append(pairs[i][1])
    for x in d:
        print(len(x), *sorted(map(lambda z: z + 1, x)))