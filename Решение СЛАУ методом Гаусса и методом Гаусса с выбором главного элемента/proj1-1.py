import numpy as np

EPS = 1e-12

def create_arr(x): # заполнение матрицы по заданной формуле
    arr = []
    M = 6
    n = 100
    q = 1.001 - 2 * M * 10**(-3);
    for i in range(1, n+1):
        arr.append([])

        for j in range(1, n+1):
            if i != j:
                arr[i-1].append(q ** (i+j) + 0.1*(j-i))
            else :
                arr[i-1].append((q-1)**(i+j))

        arr[i-1].append(x * np.exp(x/i) * np.cos(x/i))

    return arr

def print_right_side(m): # вывод правой части СЛАУ
    n = len(m)

    for i in range(n):
        print('x', end = '')
        print(i + 1, '=', m[i][n])

    print()

def det(m): # вычисление определителя матрицы
    new_m = np.copy(m)
    res = 1
    n = len(m)
    for i in range(n - 1):
        if len(np.nonzero(abs(new_m[i:, i]) > EPS)[0]) == 0:
            return 0
        nonzero = np.nonzero(abs(new_m[i:, i]) > EPS)[0][0] + i
        new_m[[i, nonzero]] = new_m[[nonzero, i]]
        if (i != nonzero):
            res = res * -1
        res = res * new_m[i][i]
        new_m[i] = new_m[i] / new_m[i][i]
        new_m[i + 1:, :] = new_m[i + 1:, :] - np.matmul(np.transpose(new_m[i + 1:,
                                        i])[:, np.newaxis], new_m[i, np.newaxis])
    res = res * new_m[n - 1][n - 1]
    return res


def gauss(m): # нахождение решения системы методом Гаусса
    new_m = np.copy(m)
    n = len(m)

    for i in range(n - 1):
        nonzero = np.nonzero(abs(new_m[i:, i]) > EPS)[0][0] + i

        new_m[[i, nonzero]] = new_m[[nonzero, i]]
        new_m[i] = new_m[i] / new_m[i][i]
        new_m[i + 1:, :] = new_m[i + 1:, :]
        - np.matmul(np.transpose(new_m[i + 1:, i])[:, np.newaxis],
        new_m[i, np.newaxis])

    for i in range(n - 1, -1, -1):
        new_m[i] = new_m[i] / new_m[i][i]
        new_m[:i, :] = new_m[:i, :] - np.matmul(np.transpose(new_m[:i, i])[:
                                        , np.newaxis], new_m[i, np.newaxis])

    return new_m

def pivot_gauss(m): # решение системы методом Гаусса с выбором главного элемента
    new_m = np.copy(m)
    n = new_m.shape[0]

    for i in range(n - 1):
        max = np.argmax(abs(new_m[i:, i]) > EPS) + i

        new_m[[i, max]] = new_m[[max, i]]

        new_m[i] = new_m[i] / new_m[i][i]
        new_m[i + 1:, :] = new_m[i + 1:, :]
        - np.matmul(np.transpose(new_m[i + 1:, i])[:, np.newaxis], new_m[i, np.newaxis])

    for i in range(n - 1, -1, -1):
        new_m[i] = new_m[i] / new_m[i][i]
        new_m[:i, :] = new_m[:i, :]
        - np.matmul(np.transpose(new_m[:i, i])[:, np.newaxis], new_m[i, np.newaxis])

    return new_m

def inverse_array(m): # нахождение обратной матрицы методом Гаусса-Жордана
    n = len(m)
    return gauss(np.concatenate((m[:, :-1], np.eye(n, n)), axis = 1))[:, n:]

def norm(m): # нахождение нормы матрицы
    return np.max(np.sum(abs(m[:, : len(m)]), axis = 1))

def condition(m): # нахождение числа обусловленности матрицы
    return norm(m) * norm(inverse_array(m))

def main():
    print('How you want to enter the matrix?')
    print('>>Manually (1)')
    print('>>Using the formula (2)')

    mode = int(input('Choose mode 1 or 2 = '))

    if mode == 1:
        n = int(input('n = '))
        m = []
        for i in range(n):
            m.append([float(j) for j in input('current row: ').split()])
            
    else:
        n = 100
        x = int(input('x = '))
        m = create_arr(x)

    m = np.array(m, dtype = float)
    det_m = det(m)

    print("\nSolution using the Gauss method")

    if det_m != 0:
        print_right_side(gauss(m))
    else:
        print('Determinant is equal to 0\n')

    print("Solution using the pivot Gauss method")

    if det_m != 0:
        print_right_side(pivot_gauss(m))
    else:
        print('Determinant is equal to 0\n')

    print("Determinant of a matrix:")
    print(det_m, end = '\n\n')

    print("Inverse matrix:")

    if det_m != 0:
        print(inverse_array(m), end = '\n\n')
    else:
        print('Determinant is equal to 0\n')

    print("Condition number:")

    if det_m != 0:
        print(condition(m))
    else:
        print('Determinant is equal to 0\n')

if __name__ == '__main__':
    main()
