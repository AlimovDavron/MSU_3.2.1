#include "lss_01_03.h"
#include <math.h>

#define GET(A, x, y, n) (A+(x)*(n)+(y))

double const EPS = 1e-25;
extern int fl_d;

struct pair{
    int i, j;
};

size_t lss_memsize_SS_NN(int n){
    return n*sizeof(double);
}

struct pair getMax(int n, const double *A, int k){
    struct pair result = {-1, -1};
    double maximumValue = 0.;
    for(int i = k; i < n; i++)
    {
        for(int j = k; j < n; j++)
        {
            if(fabs(*GET(A, i, j, n)) - fabs(maximumValue) > EPS)
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

void prettyPrint(int n, int m, const double* A){
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            printf("%lf ", *GET(A, i, j, n));
        } printf("\n");
    }
}

void pp(int n, const double* A, const double* B){
    printf("A:\n");
    prettyPrint(n, n, A);
    printf("B:\n");
    prettyPrint(1, n, B);
    printf("\n\n");
}

void printOrderOfReplacements(double *tmp, int n){
    int i = 0;
    int *ptr = (int*)tmp;
    for(i = 0; i < 2*n; i+=2)
        printf("%d %d\n", *(ptr+i), *(ptr+i+1));
}


int lss_01_03(int n, double* A, double* B, double* X, double* tmp){
    struct pair mx; int i, k, j, infinity = -1; double divider;
    for(k = 0; k < n; k++)
    {
        if(fl_d)
            pp(n, A, B);

        mx = getMax(n, A, k);

        if(fl_d)
            printf("The biggest coefficient is at (r:%d c:%d)\n\n", mx.i, mx.j);

        if(mx.i == -1) {
            for(i = k; i < n; i++)
                if (fabs(*(B + i)) > EPS) {
                    if(fl_d) printf("No answer exists.\n");
                    return 1;
                }
            infinity = k;
            for(i = k; i < n; i++)
                setReplacement(mx, tmp+i);
            break;
        }

        setReplacement(mx, tmp+k);
        replaceColumn(n, k, mx.j, A);
        replaceRow(n, k, mx.i, A, B);

        divider = *GET(A, k, k, n);
        for(j = k; j < n; j++)
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
        pp(n, A, B);


    if(infinity != -1){
        for(i = infinity; i < n; i++)
            *(X+i) = 0;
        for(i = k-1; i >= 0; i--){
            double sum = *(B+i);
            for(j = i+1; j <= n-1; j++)
                sum -= *(X + j) * *GET(A, i, j, n);
            *(X+i) = sum;
        }
    }
    else {
        for(i = n-1; i >= 0; i--){
            double sum = *(B+i);
            for(j = i+1; j <= n-1; j++)
                sum -= *(X + j) * *GET(A, i, j, n);
            *(X+i) = sum;
        }
    }

    if(fl_d)
        printOrderOfReplacements(tmp, n);

    int* iPtr = (int*)tmp;
    if(infinity == -1)
        for(i = 2*n - 2, k = n-1; k >= 0; i-=2, k--){
            int column = *(iPtr+i+1);
            replaceColumn(1, k, column, X);
        }
    else{
        for(i = 2*infinity - 2, k = infinity-1; k >= 0; i-=2, k--){
            int column = *(iPtr+i+1);
            replaceColumn(1, k, column, X);
        }
    }

    if(fl_d){
        printf("Answer: ");
        prettyPrint(1, n, X);
    }

    return 0;
}