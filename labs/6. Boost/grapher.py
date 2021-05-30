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
