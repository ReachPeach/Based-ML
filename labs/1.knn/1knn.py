import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import math

used_funcs = ['manhattan', 'euclidean', 'chebyshev']
used_kerns = ['uniform', 'triangular', 'epanechnikov', 'quartic', 'triweight', 'tricube',
              'gaussian', 'cosine', 'logistic', 'sigmoid']
used_winds = ['fixed', 'variable']
used_methods = ['naive', 'one_hot']


def use_func(fun, xs, ys):
    def manhattan(xs_, ys_):
        ans = 0
        for i in range(len(xs_) - 1):
            ans += abs(xs_[i] - ys_[i])
        return ans

    def euclidean(xs_, ys_):
        ans = 0
        for i in range(len(xs_) - 1):
            ans += (xs_[i] - ys_[i]) ** 2
        return ans ** 0.5

    def chebyshev(xs_, ys_):
        ans = 0
        for i in range(len(xs_) - 1):
            ans = max(ans, abs(xs_[i] - ys_[i]))
        return ans

    funcs = {'manhattan': lambda xs_, ys_: manhattan(xs_, ys_),
             'euclidean': lambda xs_, ys_: euclidean(xs_, ys_),
             'chebyshev': lambda xs_, ys_: chebyshev(xs_, ys_)}
    return funcs[fun].__call__(xs, ys)


def use_kern(kern, u):
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

    kerns = {'uniform': lambda u: uniform(u),
             'triangular': lambda u: triangular(u), 'epanechnikov': lambda u: epanechnikov(u),
             'quartic': lambda u: quartic(u), 'triweight': lambda u: triweight(u), 'tricube': lambda u: tricube(u),
             'gaussian': lambda u: gaussian(u), 'cosine': lambda u: cosine(u),
             'logistic': lambda u: logistic(u), 'sigmoid': lambda u: sigmoid(u)}
    return kerns[kern].__call__(u)


def minmax(dataset):
    minmax = list()
    for i in range(len(dataset[0])):
        if i == len(dataset[0]) - 1:
            continue
        value_min = dataset[:, i].min()
        value_max = dataset[:, i].max()
        minmax.append([value_min, value_max])
    return minmax


def normalize(dataset, minmax):
    for row in dataset:
        for i in range(len(row) - 1):
            row[i] = (row[i] - minmax[i][0]) / (minmax[i][1] - minmax[i][0])

    return dataset


def dist(fun, xs, ys):
    return use_func(fun, xs, ys)


def use_wind(source, neights, ind, fun, kern, wind, k):
    def fixed(h_):
        up = down = 0
        ok = ['gaussian', 'logistic', 'sigmoid']

        if neights[0][0] == 0:
            i = 0
            while i < len(source) and neights[i][0] == 0:
                up += source[neights[i][1]][-1]
                down += 1
                i += 1
        else:
            if h_ == 0:
                down = len(source) - 1
                for i in range(len(source)):
                    if i == ind:
                        continue
                    up += source[i][-1]
            else:
                for i in range(len(source)):
                    if i == ind:
                        continue
                    dist_i_ind = dist(fun, source[i], source[ind])
                    if dist_i_ind < h_ or kern in ok:
                        up += source[i][-1] * use_kern(kern, dist_i_ind / h_)
                        down += use_kern(kern, dist_i_ind / h_)
                if down == 0:
                    down = len(source) - 1
                    for i in range(len(source)):
                        if i == ind:
                            continue
                        up += source[i][-1]
        return up / down

    def variable(temp_h):
        return fixed(neights[temp_h][0])

    d = {'fixed': lambda h_: fixed(h_),
         'variable': lambda h_: variable(h_)}
    return d[wind].__call__(k)


def predict(source, ind, fun, kern, wind, k):
    neights = list()
    for i in range(len(source)):
        if i == ind:
            continue
        neights.append([dist(fun, source[i], source[ind]), i])
    neights.sort()
    return use_wind(source, neights, ind, fun, kern, wind, k)


def make_cm(source, fun, kern, method, wind, k):
    CM = [[0 for i in range(6)] for i in range(6)]
    for i in range(len(source)):
        if method == 'naive':
            predicted = predict(source, i, fun, kern, wind, k)
        else:
            vals = []
            for pos in range(6):
                nsource = source.copy()
                for row in nsource:
                    row[-1] = int(row[-1] == pos)
                predicted = predict(nsource, i, fun, kern, wind, k)
                vals.append(predicted)
            predicted = np.argmax(vals)
        CM[int(source[i][-1])][int(np.round(predicted))] += 1
    return CM


def cross_val(source, fun, kern, method, wind, k):
    CM = make_cm(source, fun, kern, method, wind, k)
    classes = 6

    def prec(r):
        tp = CM[r][r]
        sum = 0
        for i in range(classes):
            if i != r:
                sum += CM[i][r]
        return tp / (sum + tp) if sum + tp != 0 else 0

    def rec(r):
        tp = CM[r][r]
        sum = 0
        for i in range(classes):
            if i != r:
                sum += CM[r][i]
        return tp / (sum + tp) if sum + tp != 0 else 0

    def sum(ind):
        ans = 0
        for x in CM[ind]:
            ans += x
        return ans

    C = [sum(i) for i in range(classes)]

    all_count = 0
    for x in C:
        all_count += x

    def f_b(ind):
        preci = prec(ind)
        reci = rec(ind)
        return 2 * (preci * reci) / (preci + reci) if preci + reci != 0 else 0

    def micro():
        ans = 0
        for i in range(classes):
            ans += C[i] * f_b(i)
        return ans / all_count

    return micro()


def generate_set(n):
    ans = list()
    for fun in used_funcs:
        for kern in used_kerns:
            for wind in used_winds:
                for method in used_methods:
                    if wind == 'fixed':
                        for k in np.linspace(0, 2, 25):
                            ans.append([fun, kern, method, wind, k])
                    if wind == 'variable':
                        for k in range(0, n, n // 25):
                            ans.append([fun, kern, method, wind, k])
    print(len(ans))
    return ans


def approx(source):
    best_set = []
    M = 0
    all_in_all = []
    cur = 0
    h = 0
    generated_sets = generate_set(len(source))
    for var_set in generated_sets:
        prob_ans = cross_val(source, *var_set)
        all_in_all.append([prob_ans, var_set])
        if M < prob_ans:
            M = prob_ans
            best_set = var_set
        cur += 1
        if cur / len(generated_sets) > h:
            print(str(100 * h) + '%')
            h += 0.02
    return [best_set, M]


if __name__ == '__main__':
    df = pd.read_csv('dataset_41_glass.csv')

    vals = np.unique(df.Class)
    d = {}
    for i in range(len(vals)):
        d[vals[i]] = i
    for val in vals:
        df.loc[df['Class'] == val, 'Class'] = d[val]

    min_max = minmax(df.values)
    normalized_dataset_values = normalize(df.values, min_max)
    #ans = approx(normalized_dataset_values)
    ans = [['manhattan', 'triweight', 'one_hot'], 0.7537193018350691]
    metrics = ans[0][:3]
    '''
    window_values = [i for i in np.linspace(0, 2, 50)]
    f_values = [cross_val(normalized_dataset_values, wind='fixed', k=wind_val, *metrics) for wind_val in window_values]
    # print(f_values[int(np.argmax(f_values))]) 0.7027333050186164
    plt.plot(window_values, f_values)
    plt.show()
    '''

    window_values = [i for i in np.linspace(0, 8, 50)]
    f_values = [cross_val(normalized_dataset_values, wind='fixed', k=wind_val, *metrics) for wind_val in window_values]
    # print(f_values[int(np.argmax(f_values))]) 0.7027333050186164
    plt.plot(window_values, f_values)
    plt.show()
