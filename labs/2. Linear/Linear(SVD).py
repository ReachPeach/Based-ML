import numpy as np

if __name__ == '__main__':
    xs_teach = np.array([])
    ys_teach = np.array([])

    xs_test = np.array([])
    ys_test = np.array([])

    with open("2.txt", "r") as ins:
        m = int(ins.readline())
        n_teach = int(ins.readline())
        xs = []
        for i in range(n_teach):
            all = ins.readline().split()
            xs.append([int(all[x]) for x in range(len(all) - 1)])
            ys_teach = np.insert(ys_teach, i, int(all[-1]))
        xs_teach = np.array(xs)
        n_test = int(ins.readline())
        xs = []
        for i in range(n_test):
            all = ins.readline().split()
            xs.append([int(all[x]) for x in range(len(all) - 1)])
            ys_test = np.insert(ys_test, i, int(all[-1]))
        xs_test = np.array(xs)


    def SMAPE(y1, y2):
        if y1 == 0 and y2 == 0:
            return 0
        return abs(y1 - y2) / (abs(y1) + abs(y2))


    best_score = 1
    best_h = "heh nan"

    h = 1e-60
    while h <= 1e60:
        F_TF = xs_teach.transpose() @ xs_teach
        step1 = F_TF.copy() + np.dot(h, np.eye(m))
        step2 = np.linalg.pinv(step1)
        step3 = step2 @ xs_teach.transpose()
        w = step3 @ ys_teach

        cur_SMAPE = 0
        for i in range(n_test):
            x_test = xs_test[i]

            y_test = ys_test[i]
            y_pr = np.dot(x_test, w)
            cur_SMAPE += SMAPE(y_pr, y_test)
        cur_SMAPE /= n_test
        if cur_SMAPE < best_score:
            best_score = cur_SMAPE
            best_h = h

        h *= 10 ** (1 / 5)

    print(best_h, best_score * 100)

#3.981071705535084e+17 49.87277811775707