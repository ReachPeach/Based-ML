import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib
from matplotlib import ticker

names = ["10_dots.txt", "15_dots.txt"]
for name in names:
    with open(name, 'r') as ins:
        hs = []
        vals_train = []
        vals_test = []
        n = int(ins.readline())
        for i in range(n):
            all = ins.readline().split()
            h = int((all[0]))
            val1 = 100 * float(all[1])
            val2 = 100 * float(all[2])
            hs.append(h)
            vals_train.append(val1)
            vals_test.append(val2)
        fig, ax = plt.subplots()
        plt.plot(hs, vals_train, label=f"accuracy on train", color='r')
        plt.plot(hs, vals_test, label=f"accuracy on test", color='b')
        plt.xlabel('max height of tree')
        plt.ylabel('accuracy, %')
        plt.show()
