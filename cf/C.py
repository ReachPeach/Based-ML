import math

if __name__ == '__main__':
    n, m = map(int, input().split())
    D = [[int(j) for j in input().split()] for i in range(n)]
    q = [int(i) for i in input().split()]

    fun = input()
    kern = input()
    wind = input()

    h = int(input())


    def use_func(xs, ys):
        def manhattan(xs, ys):
            ans = 0
            for i in range(len(xs)):
                ans += abs(xs[i] - ys[i])
            return ans

        def euclidean(xs, ys):
            ans = 0
            for i in range(len(xs)):
                ans += (xs[i] - ys[i]) ** 2
            return ans ** 0.5

        def chebyshev(xs, ys):
            ans = 0
            for i in range(len(xs)):
                ans = max(ans, abs(xs[i] - ys[i]))
            return ans

        d = {'manhattan': lambda xs, ys: manhattan(xs, ys),
             'euclidean': lambda xs, ys: euclidean(xs, ys),
             'chebyshev': lambda xs, ys: chebyshev(xs, ys)}
        return d[fun].__call__(xs, ys)


    def use_kern(u):
        def uniform(u):
            return 1 / 2

        def triangular(u):
            return 1 - abs(u)

        def epanechnikov(u):
            return 3 * (1 - u ** 2) / 4

        def quartic(u):
            return 15 * ((1 - u ** 2) ** 2) / 16

        def triweight(u):
            return 35 * ((1 - u ** 2) ** 3) / 32

        def tricube(u):
            return 70 * ((1 - abs(u) ** 3) ** 3) / 81

        def gaussian(u):
            return (math.e ** (-0.5 * u ** 2)) / math.sqrt(2 * math.pi)

        def cosine(u):
            return (math.pi / 4) * math.cos((math.pi / 2) * u)

        def logistic(u):
            return 1 / (math.exp(u) + 2 + math.exp(-u))

        def sigmoid(u):
            return 2 / (math.pi * (math.exp(u) + math.exp(-u)))

        all = {'uniform': lambda u: uniform(u),
               'triangular': lambda u: triangular(u), 'epanechnikov': lambda u: epanechnikov(u),
               'quartic': lambda u: quartic(u), 'triweight': lambda u: triweight(u), 'tricube': lambda u: tricube(u),
               'gaussian': lambda u: gaussian(u), 'cosine': lambda u: cosine(u),
               'logistic': lambda u: logistic(u), 'sigmoid': lambda u: sigmoid(u)}
        return all[kern].__call__(u)


    def use_wind(h_):

        def dist(ind):
            cords = D[ind][0:m]
            return use_func(q, cords)

        neigth = [[dist(i), i] for i in range(n)]
        neigth.sort()

        def fixed(h_):
            up = down = 0
            ok = ['gaussian', 'logistic', 'sigmoid']

            if neigth[0][0] == 0:
                i = 0
                while i < n and neigth[i][0] == 0:
                    up += D[neigth[i][1]][m]
                    down += 1
                    i += 1
            else:
                if h_ == 0:
                    down = n
                    for i in range(n):
                        up += D[i][m]
                else:
                    for i in range(n):
                        if dist(i) < h_ or kern in ok:
                            up += D[i][m] * use_kern(dist(i) / h_)
                            down += use_kern(dist(i) / h_)
                    if down == 0:
                        down = n
                        for i in range(n):
                            up += D[i][m]
            return up / down

        def variable(temp_h):
            return fixed(neigth[temp_h][0])

        d = {'fixed': lambda h_: fixed(h_),
             'variable': lambda h_: variable(h_)}
        return d[wind].__call__(h_)


    print(use_wind(h))
