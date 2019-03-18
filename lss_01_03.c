#include "lss_01_03.h"
#include <math.h>

#define GET(A, x, y, n) (A+x*n+y)

double const EPS = 1e-6;

extern char fl_d;

struct pair{
    int i, j;
};

struct pair getMax(int n, double *A, int k){
    struct pair result = {-1, -1};
    double maximumValue = 0.;
    for(int i = k; i < n; i++)
    {
        for(int j = k; j < n; j++)
        {
            if(fabs(maximumValue) < fabs(*GET(A, i, j, n)))
            {
                maximumValue = fabs(*GET(A, i, j, n));
                result.i = i; result.j = j;
            }
        }
    }
    return result;
}

void setReplacement(struct pair replacement, double* pMem){
    int* ptr = (int *) pMem;
    *ptr = replacement.i;
    *(ptr+1) = replacement.j;
}

void replaceColumn(int n, int k, int column, double* A){
    for(int i = 0; i < n; i++)
    {
        double tmp = *GET(A, i, k, n);
        *GET(A, i, k, n) = *GET(A, i, column, n);
        *GET(A, i, column, n) = tmp;
    }
}

void replaceRow(int n, int k, int row, double *A, double *B){
    double tmp;
    for(int i = 0; i < n; i++)
    {
        tmp = *GET(A, k, i, n);
        *GET(A, k, i, n) = *GET(A, row, i, n);
        *GET(A, row, i, n) = tmp;
    }
    tmp = *(B+k);
    *(B+k) = *(B + row);
    *(B+row) = tmp;
}

void pp(int n, double* A, double* B){
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%lf ", *GET(A, i, j, n));
        } printf("\n");
    }
    for(int i = 0; i < n; i++)
        printf("%lf ", *(B+i));
    printf("\n\n");
}

void printOrderOfReplacemets(double* tmp, int n){
    int i = 0;
    int *ptr = (int*)tmp;
    for(i = 0; i < 2*n; i+=2)
        printf("%d %d\n", *(ptr+i), *(ptr+i+1));
}


int lss_01_03(int n, double* A, double* B, double* X, double* tmp){
    struct pair mx; int i, k, j; double divider;
    for(k = 0; k < n; k++)
    {
        if(fl_d)
            pp(n, A, B);

        mx = getMax(n, A, k);

        if(fl_d)
            printf("The Biggest coefficient is at (r:%d c:%d)\n\n", mx.i, mx.j);

        setReplacement(mx, tmp+k);
        replaceColumn(n, k, mx.j, A);
        replaceRow(n, k, mx.i, A, B);

        divider = *GET(A, k, k, n);
        for(int j = k; j < n; j++)
            *GET(A, k, j, n) /= divider;
        *(B+k) /= divider;
        for(i = k+1; i < n; i++)
        {
            double multiplier = *GET(A, i, k, n);
            for(j = k; j < n; j++)
                *GET(A, i, j, n) = *GET(A, i, j, n) - *GET(A, k, j, n) * multiplier;
            *(B+i) = *(B+i) - *(B+k) * multiplier;
        }
    }
    if(fl_d)
        printOrderOfReplacemets(tmp, n);
}