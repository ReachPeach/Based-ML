import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib
from matplotlib import ticker

# ROC
with open("dots.txt", 'r') as ins:
    n = int(ins.readline())
    xs = []
    ys = []
    c = [0, 0]
    for i in range(n):
        x, y = map(int, ins.readline().split())
        xs.append(x)
        ys.append(y)
        c[y] += 1
    c0, c1 = c
    pltx = [0]
    plty = [0]
    i = 0
    while i < n:
        c = [0, 0]
        while i + 1 < n and xs[i + 1] == xs[i]:
            c[ys[i]] += 1
            i += 1
        pltx.append(pltx[-1] + c[0] / c0)
        plty.append(plty[-1] + c[1] / c1)
        i += 1

    plt.xlabel("1-Specificity")
    plt.ylabel("Sensitivity")
    plt.title("ROC-кривая")
    plt.plot(pltx, plty)
    plt.show()

with open("graps.txt", "r") as ins:
    n = int(ins.readline())
    xs = []
    ys = []
    for i in range(n - 1):
        x, y = map(float, ins.readline().split())
        xs.append(x)
        ys.append(100 * y)
    plt.xlabel("значение h_legit")
    plt.ylabel("accuracy, %")
    plt.title("График точности от значения h_legit, h_legit \\in [h_legit_special; h_spam]")
    plt.plot(xs, ys)
    plt.show()
