import numpy as np
import matplotlib.pyplot as plt
import math

def test(n, x0, xn, sigma1, sigma2, gamma1, gamma2, delta1, delta2, p, q, f):
    x = np.linspace(x0, xn, n + 1)
    y = np.zeros(n + 1)
    h = (xn - x0) / n
    A = np.zeros(n + 1)
    A[n] = - gamma2 * h
    B = np.zeros(n + 1)
    B[0] = gamma1 * h    
    C = np.zeros(n + 1)    
    C[0] = - sigma1 * h * h + gamma1 * h    
    C[n] = - (sigma2 * h * h + gamma2 * h)
    F = np.zeros(n + 1)    
    F[0] = delta1 * h * h    
    F[n] = delta2 * h * h    
    
    for i in range(1, n):    
        A[i] = 1 - p(x[i]) * h / 2
        B[i] = 1 + p(x[i]) * h / 2
        C[i] = 2 - q(x[i]) * h * h
        F[i] = h * h * f(x[i])    
    
    xi = np.zeros(n)    
    xi[0] = B[0] / C[0]    
    nu = np.zeros(n)    
    nu[0] = - F[0] / C[0]    
    
    for i in range(1, n):    
        xi[i] = B[i] / (C[i] - xi[i - 1] * A[i])
        nu[i] = (A[i] * nu[i - 1] - F[i]) / (C[i] - xi[i - 1] * A[i])    
    
    y[n] = (F[n] - A[n] * nu[n - 1]) / (A[n] * xi[n - 1] - C[n])    
    
    for i in range(n - 1, -1, -1):
        y[i] = xi[i] * y[i + 1] + nu[i]    
    
    return y

def p0(x):
    return -0.5

def q0(x):
    return 3

def f0(x):
    return 2*x*x

def solution1(x):
    return np.exp(-x) + np.exp(3 * x) + 0.2 * np.exp(4 * x)

def p1(x):
    return 2

def q1(x):
    return -1 / x

def f1(x):
    return 3

def solution2(x):
    return np.exp(-x) - np.exp(-1) + x

def p2(x):
    return 1.0

def q2(x):
    return 0.0

def f2(x):
    return 1.0

def main():
    print('Which problem do you want to solve?')
    print("(1) y'' - 0.5y' + 3y = 2x^2, y(1)-2y'(1) = 0.6, y(1.3) = 1")        
    print("(2) y''-2y'-3y=e^(4x), -y(0.0)+y'(0.0)=0.6, y(1)+y'(1)=4e^3+e^4")
    print("(3) y''+y'=1.0, y'(0.0)=0.0, y(1)=1.0")
    
    mode = int(input('problem number: '))
    n = int(input('n = '))
    
    if mode == 1:
        y = test(n, 1.0, 1.3, 1.0, 1.0, -2.0, 0.0, 0.6, 1.0, p0, q0, f0)
        x = np.linspace(1.0, 1.3, n + 1)
        
        plt.plot(x, y, c='r', label='y(x)')

    elif mode == 2:
        y = test(n, 0.0, 1.0, -1.0, 1.0, 1.0, 1.0, 0.6, 4 * math.exp(3) + math.exp(4), p1, q1, f1)
        x = np.linspace(0.0, 1.0, n + 1)

        plt.plot(x, y, c='r', label='y(x)')
        plt.plot(x, solution1(x), c='b', label='real solution')

    elif mode == 3:
        y = test(n, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, p2, q2, f2)
        
        x = np.linspace(0.0, 1.0, n + 1)
        
        plt.plot(x, y, c='r', label='y(x)')
        plt.plot(x, solution2(x), c='b', label='real solution')
    
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main()
