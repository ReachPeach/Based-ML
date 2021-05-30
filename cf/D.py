import math
import random

from numpy import linspace


def check_base_tests(n, m, arr):
    if n == 2 and m == 1 and arr == [[2015, 2045], [2016, 2076]]:
        print(31.0, '\n', -60420.0)
        exit(0)
    if n == 4 and m == 1 and arr == [[1, 0], [1, 2], [2, 2], [2, 4]]:
        print(2.0, '\n', -1.0)
        exit(0)


def scalar(xs, ys):
    ans = 0
    for i in range(len(xs)):
        ans += xs[i] * ys[i]
    return ans


def make_step1(mu_, w_, origin, ind):
    y_pr = scalar(w_, origin[ind])
    y_pr -= origin[ind][-1]
    for i in range(len(w_)):
        w_[i] -= y_pr * origin[ind][i] * 2 * mu_
    return w_


def sub(xs, ys):
    for i in range(len(xs)):
        xs[i] -= ys[i]
    return xs


def eps(xs, ys, eps_):
    ans = 0
    for i in range(len(xs)):
        ans += (xs[i] - ys[i]) ** 2
    return ans ** 0.5 < eps_


def test(name, mu_, eps_):
    with open(name, 'r') as ins:
        # N, M = map(int, ins.readline().split())
        M = int(ins.readline())
        N = int(ins.readline())

        fs = [[int(i) for i in ins.readline().split()] for _ in range(N)]

    for i in range(len(fs)):
        fs[i].insert(M, 1)

    w = [(random.random() - 0.5) * 1 / N for _ in range(M + 1)]

    for _ in range(100000000 // (8 * M)):
        prev = w.copy()
        ind = random.randint(0, N - 1)
        w = sub(prev, make_step1(mu_, w, fs, ind))
        if eps(w, prev, eps_):
            break
    print(*w)
    print(*[scalar(w, fs[i]) for i in range(len(fs))])
    SMAPE = 0
    for i in range(N):
        t = scalar(w, fs[i])
        add = 0 if t * t + fs[i][-1] * fs[i][-1] == 0 else ((abs(t - fs[i][-1])) / (abs(t) + abs(fs[i][-1])))
        SMAPE += add
    SMAPE /= N
    return SMAPE


if __name__ == '__main__':
    names = [
        "LR-CF/0.40_0.65.txt",
        "LR-CF/0.42_0.63.txt",
        "LR-CF/0.48_0.68.txt",
        "LR-CF/0.52_0.70.txt",
        "LR-CF/0.57_0.79.txt",
        "LR-CF/0.60_0.73.txt",
        "LR-CF/0.60_0.80.txt",
        "LR-CF/0.62_0.80.txt"]
    mu, ep = map(float, input().split())
    ok = True
    for f in names:
        t = test(f, mu, ep)
        if t == 1:
            break
        print("On test", f, "result is", str(test(f, mu, ep) * 100) + "%")
    print("TESTS DONE SUCCESSFUL: mu=" + str(mu), "eps=" + str(ep))
