#ifndef INTEGRAL_H_INCLUDED
#define INTEGRAL_H_INCLUDED

extern double f1(double);
extern double f2(double);
extern double f3(double);

double integral(double(*f)(double), double a, double b, double eps2);

#endif // INTEGRAL_H_INCLUDED
