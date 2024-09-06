import numpy as np
from scipy.integrate import odeint
import math
import matplotlib.pyplot as plt

def sec_order(x0, h, x1, y0):
    n = int((x1 - x0) / h + 1)
    y = np.zeros(n)
    print('x =', x0, 'y =', y0)
    y[0] = y0

    for i in range(1, n):
        y[i] = y[i - 1] + h * f(x0, y[i - 1]) / 2 + h * f(x0 + h, y[i - 1] + h * f(x0, y[i - 1])) / 2
        x0 = x0 + h
        print('x =', x0, 'y =', y[i])
	
    return y
	
def system_sec_order(x0, h, x1, y10, y20):
    n = int((x1 - x0) / h + 1)
    y1 = np.zeros(n)
    y2 = np.zeros(n)
    print('x =', x0, 'y1 =', y10, 'y2 =', y20)
	
    y1[0] = y10
    y2[0] = y20
	
    for i in range(1, n):
        y1[i] = y1[i - 1] + h * f1(x0, y1[i - 1], y2[i - 1]) / 2 + h * f1(x0 + h, y1[i - 1]
        + h * f1(x0, y1[i - 1], y2[i - 1]), y2[i - 1] + h * f2(x0, y1[i - 1], y2[i - 1])) / 2
        y2[i] = y2[i - 1] + h * f2(x0, y1[i - 1], y2[i - 1]) / 2 + h * f2(x0 + h, y1[i - 1]
        + h * f1(x0, y1[i - 1], y2[i - 1]), y2[i - 1] + h * f2(x0, y1[i - 1], y2[i - 1])) / 2
        x0 = x0 + h
        print('x =', x0, 'y1 =', y1[i], 'y2 =', y2[i])
 	
    return y1, y2
		
def fourth_order(x0, h, x1, y0):
    n = int((x1 - x0) / h + 1)
    y = np.zeros(n)
    print('x =', x0, 'y =', y0)
		
    y[0] = y0
		
    for i in range(1, n):
        k1 = f(x0, y[i - 1])
        k2 = f(x0 + h / 2, y[i - 1] + k1 * h / 2)
        k3 = f(x0 + h / 2, y[i - 1] + k2 * h / 2)
        k4 = f(x0 + h, y[i - 1] + k3 * h)
        y[i] = y[i - 1] + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        x0 = x0 + h
        print('x =', x0, 'y =', y[i])
	
    return y
	
def system_fourth_order(x0, h, x1, y10, y20):
    n = int((x1 - x0) / h + 1)
    y1 = np.zeros(n)
    y2 = np.zeros(n)
    print('x =', x0, 'y1 =', y10, 'y2 =', y20)
	
    y1[0] = y10
    y2[0] = y20
	
    for i in range(1, n):
        k1 = f1(x0, y1[i - 1], y2[i - 1])
        m1 = f2(x0, y1[i - 1], y2[i - 1])
        k2 = f1(x0 + h / 2, y1[i - 1] + k1 * h / 2, y2[i - 1] + m1 * h / 2)
        m2 = f2(x0 + h / 2, y1[i - 1] + k1 * h / 2, y2[i - 1] + m1 * h / 2)
        k3 = f1(x0 + h / 2, y1[i - 1] + k2 * h / 2, y2[i - 1] + m2 * h / 2)
        m3 = f2(x0 + h / 2, y1[i - 1] + k2 * h / 2, y2[i - 1] + m2 * h / 2)
        k4 = f1(x0 + h, y1[i - 1] + k3 * h, y2[i - 1] + m3 * h)
        m4 = f2(x0 + h, y1[i - 1] + k3 * h, y2[i - 1] + m3 * h)
        y1[i] = y1[i - 1] + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        y2[i] = y2[i - 1] + h * (m1 + 2 * m2 + 2 * m3 + m4) / 6
        
        x0 = x0 + h
        print('x =', x0, 'y1 =', y1[i], 'y2 =', y2[i])
		
    return y1, y2
		
def solution(x):
    tmp = (-1/6) * x*x *(-3+2*x)
    return np.exp(tmp)
                    
def system_solution(y, x):
    y1, y2 = y
    return [math.sin(1.4 * y1 * y1)- x + y2, x - y1 - 2.2 * y2 * y2 + 1]

def f(x, y):
    return (x - x * x) * y

def f1(x, y1, y2):
    return math.sin(1.4 * y1 * y1)- x + y2

def f2(x, y1, y2):
    return x - y1 - 2.2 * y2 * y2 + 1

def main():
    print('Please choose a Cauchy problem you want to solve')
    print(">>y' = (x - x^2)y; y(0) = 1; (1)")
    print(">>y1' = sin(1.4 * y1^2) - x + y2; y2' = x - y1 - 2.2 * y2^2 + 1;
          "y1(0) = 1; y2(0) = 0.5 (2)")
	
    mode = int(input('mode (1 or 2) = '))	
    h = float(input('h (step) = '))	
    x_max = float(input('x_max = '))	
		
    n = int((x_max - 0.0) / h + 1)	
		
    x = np.linspace(0.0, x_max, n)	
		
    if mode == 1:	
        print('Runge-Kutta method of the second order of accuracy')
        y_2 = sec_order(0, h, x_max, 1)
        print('Runge-Kutta method of the fourth order of accuracy')
        y_4 = fourth_order(0, h, x_max, 1)
        plt.plot(x, y_2, c='r', label='second order')
        plt.plot(x, y_4, c='b', label='fourth order')
        plt.plot(np.linspace(0.0, x_max, 10000), solution(np.linspace(0.0, x_max, 10000)),
                 c='y',label='real solution')
        plt.legend()	
        plt.show()	
		
    else:	
        print('Runge-Kutta method of the second order of accuracy')
        y1_2, y2_2 = system_sec_order(0, h, x_max, 1, 0.5)
        print('Runge-Kutta method of the fourth order of accuracy')

        y1_4, y2_4 = system_fourth_order(0, h, x_max, 1, 0.5)
                    
        ax, pic = plt.subplots(1, 2, figsize=(20, 10))
                    
        pic[0].plot(x, y1_2, c='r', label='y1 second order')
        pic[0].plot(x, y1_4, c='b', label='y1 fourth order')
        pic[0].plot(np.linspace(0.0, x_max, 10000), odeint(system_solution, [0.5, 1.0],
         np.linspace(0.0, x_max, 10000))[:, 0], c='y', label='y1 real solution')
        pic[0].legend()
            
        pic[1].plot(x, y2_2, c='r', label='y2 second order')
        pic[1].plot(x, y2_4, c='b', label='y2 fourth order')
        pic[1].plot(np.linspace(0.0, x_max, 10000), odeint(system_solution, [0.5, 1.0],
         np.linspace(0.0, x_max, 10000))[:, 1],	c='y',	label='y2 real solution')
        pic[1].legend()
        plt.show()
		
if __name__ == '__main__':
    main()
	
