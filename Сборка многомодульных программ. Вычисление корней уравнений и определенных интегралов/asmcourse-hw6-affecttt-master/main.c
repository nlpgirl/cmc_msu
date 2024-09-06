#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"
#include "roots.h"
#include "integral.h"
#include "test.h"

void *testfuncs[4] = {NULL, f1test, f2test, f3test};

int main(int argc, char* argv[]) {
    const char *short_opt = "hriR:I:";
    const struct option long_opt[] = {
        {"help", no_argument, NULL, 'h'},
        {"root", no_argument, NULL, 'r'},
        {"iterations", no_argument, NULL, 'i'},
        {"test-root", required_argument, NULL, 'R'},
        {"test-integral", required_argument, NULL, 'I'},
        {NULL, 0, NULL, 0}
    };
    int option, num1, num2, num;
    double A, B, R, tested_ans, err, eps1, eps2;
    char *letter;
    
    if(argc == 1) {
        double x1 = root(f1, f2, 1, 2, 0.0001);
        double x2 = root(f1, f3, -3, -2, 0.0001);
        double x3 = root(f2, f3, 0, 1, 0.0001);
        double s1 = integral(f1, x2, x1, 0.00001);
        double s2 = integral(f3, x2, x3, 0.00001);
        double s3 = integral(f2, x3, x1, 0.00001);
        printf("%lf\n", s1 - s2 - s3);
        return 0;
    }
    while((option=getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1){

        switch(option) {

            case 'h':
                printf("--root and -r print the intersection points\n");
                printf("--iterations and -i print iterations needed for finding intersection points\n");
                printf("--test-root and -R print the intersection points of testing functions\n");
                printf("--test-integral and -I tests the integral functions\n");
                break;

            case 'r':
                double x;
                x = root(f1, f2, 1, 2, 0.0001);
                printf("The intersection point of f1 and f2 is (%lf,%lf)\n", x, f1(x));
                x = root(f1, f3, -3, -2, 0.0001);
                printf("The intersection point of f1 and f3 is(%lf,%lf)\n", x, f1(x));
                x = root(f2, f3, 0, 1, 0.0001);
                printf("The intersection point of f2 and f3 is (%lf,%lf)\n", x, f2(x));
                break;

            case 'i':
                root(f1, f2, 1, 2, 0.0001);
                printf("The number of iterations for f1 and f2 intersection is %d\n", iter);
                root(f1, f3, -3, -2, 0.0001);
                printf("The number of iterations for f1 and f2 intersection is %d\n", iter);
                root(f2, f3, 0, 1, 0.0001);
                printf("The number of iterations for f1 and f2 intersection is %d\n", iter);
                break;

            case 'R':
                letter = strtok(optarg, ":");
                num1 = atoi(letter);
                letter = strtok(NULL, ":");
                num2 = atoi(letter);
                letter = strtok(NULL, ":");
                A = atof(letter);
                letter = strtok(NULL, ":");
                B = atof(letter);
                letter = strtok(NULL, ":");
                eps1 = atof(letter);
                letter = strtok(NULL, ":");
                R = atof(letter);
                tested_ans = root(testfuncs[num1], testfuncs[num2], A, B, eps1);
                printf("%lf\n", tested_ans);
                err = fabs(R-tested_ans);
                printf("%lf\n", err);
                printf("%lf\n", err/fabs(R));
                break;

            case 'I':
                letter = strtok(optarg, ":");
                num = atoi(letter);
                letter = strtok(NULL, ":");
                A = atof(letter);
                letter = strtok(NULL, ":");
                B = atof(letter);
                letter = strtok(NULL, ":");
                eps2 = atof(letter);
                letter = strtok(NULL, ":");
                R = atof(letter);
                tested_ans = integral(testfuncs[num], A, B, eps2);
                printf("%lf\n", tested_ans);
                err = fabs(R-tested_ans);
                printf("%lf\n", err);
                printf("%lf\n", err/fabs(R));
                break;

            default:
                break;
            }
    }
    return 0;
}
