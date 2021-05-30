import random
import math

interesting_steps = [1, 2, 3, 5, 8, 13, 21, 34, 55]

class DT_node:
    def __init__(self):
        self.isLeaf = True
        self.stored = -1
        self.left = None
        self.right = None
        self.index = -1
        self.splitter = -1

    def makeLeaf(self, value):
        self.stored = value

    def makeInner(self, ind, splitter, left, right):
        self.isLeaf = False
        self.splitter = splitter
        self.index = ind
        self.left = left
        self.right = right

    def predict(self, x):
        if self.isLeaf:
            return self.stored
        elif x[self.index] < self.splitter:
            return self.left.predict(x)
        else:
            return self.right.predict(x)


def majority(vals):
    d = {}
    for val in vals:
        if val not in d:
            d[val] = 0
        d[val] += 1
    ans = [-1, 0]
    for item in d.items():
        if item[1] > ans[0]:
            ans = [item[1], item[0]]
    return ans


class RDT:
    def __init__(self, h):
        self.max_h = h

    def build(self, xs, ys, h):
        v = DT_node()
        state = majority(ys)
        if state[0] == len(ys) or self.max_h == h:
            v.makeLeaf(state[1])
            return v
        fi = random.randint(0, 1)
        fichs = set()
        sfichs = []
        for x in xs:
            fichs.add(x[fi])
        for elem in fichs:
            sfichs.append(elem)
        sfichs.sort()
        if len(sfichs) == 1:
            v.makeLeaf(state[1])
            return v

        rand_ind = random.randint(0, len(sfichs) - 2)
        splitter = (sfichs[rand_ind] + sfichs[rand_ind + 1]) / 2
        l_xs = []
        r_xs = []
        l_ys = []
        r_ys = []
        for i in range(len(xs)):
            if xs[i][fi] < splitter:
                l_xs.append(xs[i])
                l_ys.append(ys[i])
            else:
                r_xs.append(xs[i])
                r_ys.append(ys[i])

        left = self.build(l_xs, l_ys, h + 1)
        right = self.build(r_xs, r_ys, h + 1)
        v.makeInner(fi, splitter, left, right)
        return v


class Boost:
    def __init__(self, count, xs, ys):
        self.trees = []
        self.h = None
        self.count = count
        self.xs = xs
        self.ys = ys
        self.T = 0
        self.Bs = []
        self.As = []

    def make_train(self):
        rx = self.xs.copy()
        ry = self.ys.copy()
        n = len(self.xs)

        while 10 * len(rx) >= 4 * n:
            rand_ind = random.randint(0, len(rx) - 2)
            rx[rand_ind] = rx.pop()
            ry[rand_ind] = ry.pop()

        while len(rx) < n:
            rand_ind = random.randint(0, len(rx) - 1)
            rx.append(rx[rand_ind])
            ry.append(ry[rand_ind])

        return [rx, ry]

    def train(self, h):
        RF = RDT(h)
        self.trees.clear()
        for i in range(self.count):
            tr_xs, tr_ys = self.make_train()
            self.trees.append(RF.build(tr_xs, tr_ys, 0))

    def boost_iteration(self, w):
        Ns = [0 for _ in range(len(self.trees))]
        for i in range(self.count):
            for j in range(len(self.xs)):
                y_pr = self.trees[i].predict(self.xs[j])
                if y_pr != self.ys[j]:
                    Ns[i] += w[j]
        r = [Ns[0], 0]
        for i in range(len(Ns)):
            if Ns[i] < r[0]:
                r = [Ns[i], i]
        return r

    def boost(self, iterations):
        self.Bs.clear()
        self.As.clear()
        self.T = 1
        w = [1 / len(self.xs) for _ in range(len(self.xs))]
        for t in range(1, iterations + 1):
            Nt, at = self.boost_iteration(w)
            if Nt == 0:
                break

            self.As.append(at)
            bt = 0.5 * math.log((1 - Nt) / Nt)
            self.Bs.append(bt)
            s = 0
            for i in range(len(self.xs)):
                tf_mul = 1 if self.ys[i] == self.trees[at].predict(self.xs[i]) else -1
                w[i] *= math.exp(-bt * tf_mul)
                s += w[i]
            for i in range(len(w)):
                w[i] /= s
            self.T = t

    def predict(self, x):
        s = 0
        for i in range(self.T):
            tf_mul = 2 * self.trees[self.As[i]].predict(x) - 1
            s += self.Bs[i] * tf_mul
        return 1 if s > 0 else 0


def do_task(name):
    xs = []
    ys = []
    with open(name, 'r') as ins:
        s = ins.readline()
        for s in ins.readlines():
            x1, x2, y = s.rstrip().split(',')
            x1, x2 = map(float, [x1, x2])
            y = 1 if y == 'P' else 0
            xs.append([x1, x2])
            ys.append(y)

    AdaBoost = Boost(1000, xs, ys)
    for h in range(1, 4):
        AdaBoost.train(h)
        for steps in interesting_steps:
            AdaBoost.boost(steps)
            acc = 0
            for i in range(len(xs)):
                if AdaBoost.predict(xs[i]) == ys[i]:
                    acc += 1
            print("For max-height=" + str(h), "with", steps, "iterations accuracy is", acc / len(xs))
        print()


if __name__ == '__main__':
    fnames = ["chips.csv", "geyser.csv"]
    for fname in fnames:
        do_task(fname)
        print(fname)

'''
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib
from matplotlib import ticker


def draw(steps, name):
    xs = []
    ys = []
    with open(name + '.csv', 'r') as ins:
        s = ins.readline()
        for s in ins.readlines():
            x1, x2, y = s.rstrip().split(',')
            x1, x2 = map(float, [x1, x2])
            y = 1 if y == 'P' else 0
            xs.append([x1, x2])
            ys.append(y)

    xs_min, xs_max, ys_min, ys_max = 1000, -1000, 1000, -1000
    for x in xs:
        xs_min = min(xs_min, x[0])
        xs_max = max(xs_max, x[0])
        ys_min = min(ys_min, x[1])
        ys_max = max(ys_max, x[1])

    adaBoost = AdaBoost.Boost(1000, xs, ys)
    h = 3
    adaBoost.train(h)
    adaBoost.boost(steps)

    x = [xs_min + i * (xs_max - xs_min) / 500 for i in range(501)]
    y = [ys_min + i * (ys_max - ys_min) / 500 for i in range(501)]

    xs_ok, xs_nok, ys_ok, ys_nok = [], [], [], []
    for i in range(501):
        for j in range(501):
            if adaBoost.predict([x[i], y[j]]) == 1:
                xs_ok.append(x[i])
                ys_ok.append(y[j])
            else:
                xs_nok.append(x[i])
                ys_nok.append(y[j])
    originX_ok, originX_nok, originY_ok, originY_nok = [], [], [], []

    for i in range(len(xs)):
        if adaBoost.predict(xs[i]) == ys[i]:
            originX_ok.append(xs[i][0])
            originY_ok.append(xs[i][1])
        else:
            originX_nok.append(xs[i][0])
            originY_nok.append(xs[i][1])

    plt.scatter(xs_ok, ys_ok, color="mistyrose")
    plt.scatter(xs_nok, ys_nok, color="lavender")
    plt.scatter(originX_ok, originY_ok, color="crimson")
    plt.scatter(originX_nok, originY_nok, color="blue")
    plt.title(name + " with used method " + str(steps))
    print(name + "_" + str(steps))
    plt.show()
    plt.savefig(name + "_" + str(steps))


def draw_accuracy(name):
    xs = []
    ys = []
    with open(name + 'csv', 'r') as ins:
        s = ins.readline()
        for s in ins.readlines():
            x1, x2, y = s.rstrip().split(',')
            x1, x2 = map(float, [x1, x2])
            y = 1 if y == 'P' else 0
            xs.append([x1, x2])
            ys.append(y)

    accs = []
    adaBoost = AdaBoost.Boost(1000, xs, ys)
    h = 3
    adaBoost.train(h)
    for steps in AdaBoost.interesting_steps:
        adaBoost.boost(steps)
        acc = 0
        for i in range(len(xs)):
            if adaBoost.predict(xs[i]) == ys[i]:
                acc += 1
        accs.append(100 * acc / len(xs))

    fig, ax = plt.subplots()
    plt.plot(AdaBoost.interesting_steps, accs, label=f"accuracy on train", color='r')
    plt.xlabel('iterations')
    plt.ylabel('accuracy, %')
    plt.show()
    plt.savefig(name + "_accuracy")


if __name__ == '__main__':
    import AdaBoost

    fname = 'chips'
    for step in AdaBoost.interesting_steps:
        draw(step, fname)
    draw_accuracy(fname)


'''