if __name__ == '__main__':
    k = int(input())
    CM = [[int(i) for i in input().split()] for i in range(k)]


    def prec(r):
        tp = CM[r][r]
        sum = 0
        for i in range(k):
            if i != r:
                sum += CM[i][r]
        return tp / (sum + tp) if sum + tp != 0 else 0


    def rec(r):
        tp = CM[r][r]
        sum = 0
        for i in range(k):
            if i != r:
                sum += CM[r][i]
        return tp / (sum + tp) if sum + tp != 0 else 0


    def sum(ind):
        ans = 0
        for x in CM[ind]:
            ans += x
        return ans


    C = [sum(i) for i in range(k)]

    all = 0
    for x in C:
        all += x


    def f_b(ind):
        preci = prec(ind)
        reci = rec(ind)
        return 2 * (preci * reci) / (preci + reci) if preci + reci != 0 else 0


    def micro():
        ans = 0
        for i in range(k):
            ans += C[i] * f_b(i)
        return ans / all


    def macro():
        precw = recw = 0
        for i in range(k):
            precw += C[i] * prec(i)
            recw += CM[i][i]
        precw /= all
        recw /= all

        return 2 * (precw * recw) / (precw + recw) if precw + recw != 0 else 0


    print(macro())
    print(micro())
