import numpy as np
import math

EPS = 1e-12

def is_pos(m): # checking a matrix if it is positive-definite
    return np.all(np.linalg.eigvals(m[:, : -1]) > 0)

def is_herm(m): # checking a matrix if it is hermitian
    m_trans = m[:, : -1].transpose()
    return m[:, : -1].all() == m_trans.all()

def form_matr(x): # filling the matrix according to the given formula
    matr = []
    M = 6
    n = 100
    q = 1.001 - 2 * M * 10**(-3);
    for i in range(1, n+1):
        matr.append([])

        for j in range(1, n+1):
            if i != j:
                matr[i-1].append(q ** (i+j) + 0.1*(j-i))
            else :
                matr[i-1].append((q-1)**(i+j))

        matr[i-1].append(x * np.exp(x/i) * np.cos(x/i))

    return matr

def residual(m, x): # calculation of the residual of currentresult
    return m[:, : -1].dot(np.transpose([x])) - m[:, -1:]
 
def relax(m, e, w): # solving a system of equations by he over-relaxation metho
    x = np.zeros(len(m))
    count = 0

    while norm(residual(m, x)) >= e:
        for i in range(len(m)):
            sum = 0

            for j in range(len(m)):
                sum = sum + m[i, j] * x[j]
                
            x[i] = x[i] - w / m[i, i] * sum + w * m[i, len(m)] / m[i, i]

        count += 1

    print('number of iterations:', count)

    return x

def det(m): # calculation of the determinant of a matrix
    new_m = np.copy(m)
    res = 1

    n = len(m)
    for i in range(n - 1):
        if len(np.nonzero(abs(new_m[i:, i]) > EPS)[0]) == 0:
            return 0

        nonzero = np.nonzero(abs(new_m[i:, i]) > EPS)[0][0]
        + i
        new_m[[i, nonzero]] = new_m[[nonzero, i]]
        
        if (i != nonzero):
            res = res * -1

        res = res * new_m[i][i]
        new_m[i] = new_m[i] / new_m[i][i]
        new_m[i + 1:, :] = new_m[i + 1:, :] - np.matmul(np.transpose(new_m[i + 1:,
                                            i])[:, np.newaxis], new_m[i,np.newaxis])

    res = res * new_m[n - 1][n - 1]
    return res

def norm(m): # finding the norm of a matrix
    return np.max(np.sum(abs(m[:, : len(m)]), axis = 1))
 
def main():
    print('Please choose how you want to enter the matrix')
    print('>>I want to enter the matrix manually (1)')
    print('>>I want to enter the matrix by the formula (2)')
    mode = int(input('mode (1 or 2) = '))

    n = int(input('n = '))
    m = []

    if mode == 1:
        for i in range(n):
            m.append([float(j) for j in input('current row: ').split()])
    else:
        m = form_matr(n) 
    print("\nInput matrix:") # output of the entered matrix
    for row in m:
        print(' '.join(list(map(str, row)))) 
    m = np.array(m, dtype = float)
    det_m = det(m)

    e = float(input('\nEps = ')) 
    print("\nSolution of a system of equations by the over-relaxation method")

    if det_m != 0:
        if (not is_pos(m)) or (not is_herm(m)):
            m_trans = m[:, : -1].transpose()
            new_left = m_trans.dot(m[:, : -1])
            new_right = m_trans.dot(m[:, -1:])
            m = np.concatenate((new_left, new_right), axis = 1)

        for w in np.arange(0.1, 2.0, 0.1):
            print('with w =', w, relax(m, e, w), end = '\n\n')



    else:
        print('does not exist\n')
    print("determinant of a matrix:")
    print(det_m, end = '\n\n')
            
if __name__  == '__main__':
    main()
