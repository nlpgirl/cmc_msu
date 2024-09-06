#ifndef ROOTS_H_INCLUDED
#define ROOTS_H_INCLUDED

extern int iter;
extern double f1(double);
extern double f2(double);
extern double f3(double);

double root(double(*f1)(double), double(*f2)(double), double a, double b, double eps1);

#endif // ROOTS_H_INCLUDED
