import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib
from matplotlib import ticker

xs = []
smape_teach = []
smape_test = []
with open("dots.txt", 'r') as ins:
    for _ in range(50):
        s = ins.readline()
        x, teach, test_gradient = s.split()
        xs.append(int(x))
        smape_teach.append(float(teach))
        smape_test.append(float(test_gradient))

xs = np.array(xs)
fig, ax = plt.subplots()
plt.plot(xs, smape_test, label=f"test_gradient smape")
plt.plot(xs, smape_teach, label=f"teach smape")

ax.xaxis.set_major_locator(ticker.MultipleLocator(200))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(400))

ax.yaxis.set_major_locator(ticker.MultipleLocator(10))
ax.yaxis.set_minor_locator(ticker.MultipleLocator(20))

plt.xlabel('count of iterations')
plt.ylabel('smape of prediction, %')

plt.show()
