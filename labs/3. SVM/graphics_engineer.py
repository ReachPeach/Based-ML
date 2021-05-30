import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib
from matplotlib import ticker


def draw(method, name, ifs):
    n = int(ifs.readline())
    xs_ok = []
    xs_nok = []
    ys_ok = []
    ys_nok = []
    for i in range(n):
        all = ifs.readline().split()
        x = float(all[0])
        y = float(all[1])
        verd = all[2]
        if verd == 'True':
            xs_ok.append(x)
            ys_ok.append(y)
        else:
            xs_nok.append(x)
            ys_nok.append(y)
    count = int(ifs.readline())
    xs1_ok = []
    xs1_nok = []
    ys1_ok = []
    ys1_nok = []
    xs1_n1 = []
    ys1_n1 = []

    xs1_n2 = []
    ys1_n2 = []

    for i in range(count):
        x, y, pr = map(float, ifs.readline().split())
        if pr >= 1:
            xs1_ok.append(x)
            ys1_ok.append(y)
        else:
            if pr <= -1:
                xs1_nok.append(x)
                ys1_nok.append(y)
            else:
                if pr > 0:
                    xs1_n1.append(x)
                    ys1_n1.append(y)
                else:
                    xs1_n2.append(x)
                    ys1_n2.append(y)
    plt.scatter(xs1_ok, ys1_ok, color="mistyrose")
    plt.scatter(xs1_nok, ys1_nok, color="lavender")
    plt.scatter(xs1_n1, ys1_n1, color="linen")
    plt.scatter(xs1_n2, ys1_n2, color="moccasin")
    plt.scatter(xs_ok, ys_ok, color="crimson")
    plt.scatter(xs_nok, ys_nok, color="saddlebrown")
    plt.title(name + " with used method " + method)
    print(name + "_" + method)
    plt.show()


if __name__ == '__main__':
    methods = ["linear", "polynomic", "radical"]
    with open("graphics_info.txt", "r") as ins:
        for j in range(3):
            draw(methods[j], "chips", ins)
        for j in range(3):
            draw(methods[j], "geyser", ins)
