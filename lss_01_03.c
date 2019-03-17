#include "lss_01_03.h"

#define GET(x, y, n) (A+x*n+y)

int lss_01_03(int n, double* A, double* B, double* X, double* tmp){
    printf("%d ", n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%lf ", *GET(i, j, n));
        }
    }

    /*
     * CHECK MATRIX TO EXCEPTION CASES
     */

    /*
     * for(i = 0; i < n; i++)
     *      i, j = choose MAX A[i][j] in subMatrix [(i, i), (n, n)]
     *      SWAP(i, 0)
     *      SWAP(j, 0)
     *      do GAUSS on it
     */
}