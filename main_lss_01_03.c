#include "lss_01_03.h"

/*
 *  System of linear equations solver
 *  Alimov Davron. Problem 3. Gauss Method (Main Matrix)
 *
 *  Exceptions:
 *  1 - ValidationError: No such file or directory
 *
 */

char fl_d = NULL,
        fl_e = NULL,
        fl_p = NULL,
        fl_t = NULL,
        fl_h = NULL;

int strlen(char const *input){
    int length = 0;
    while(input[length]!='\0')
        length++;
    return length;
}

int validateFile(char* filename)
{
    FILE *f = fopen(filename, "rb");
    return f == NULL;
}

int validateParameters(int argc, char **argv){
    int i = 0;
    if(argc == 1)
        return 0;

    if(validateFile(argv[1]) || validateFile(argv[2]))
        return 1;

    for(i = 3; i < argc; i++)
        if(argv[i][0] != '-' || strlen(argv[i]) != 2)
            return 2;

    return 0;
}

int readInputData(char *inputFile, double** A, double** B, double** X, int *n)
{
    int i;
    FILE *in = fopen(inputFile, "r");
    fscanf(in, "%d", n);
    *A = malloc((*n) * (*n) * sizeof(double));
    *B = malloc((*n) * sizeof(double));
    *X = malloc((*n) * sizeof(double));

    for(i = 0; i < (*n)*(*n); i++)
        fscanf(in, "%lf", (*A+i));

    for(i = 0; i < (*n)*(*n); i++)
        fscanf(in, "%lf", (*B+i));

    return 0;
}

void writeAnswer(char *outputFile, int n, double* X){
    int i;
    FILE *out = fopen(outputFile, "w");
    fprintf(out, "%d\n", n);
    for(i = 0; i < n; i++)
        fprintf(out, "%1.9lf\n", *(X+i));
}

int main(int argc, char* argv[]) {
    int i = 0, n;
    double *A, *B, *X, *tmp;
    char* inputFile = "lss_01_03_in.txt";
    char* outputFile = "lss_01_03_out.txt";

    switch (validateParameters(argc, argv))
    {
        case 0:
            if(argc > 1){
                inputFile = argv[1];
                outputFile = argv[2];
                for(i = 3; i < argc; i++)
                    switch (argv[i][1])
                    {
                        case 'd':
                            fl_d = 1;
                            break;
                        case 'h':
                            fl_h = 1;
                            break;
                        case 'e':
                            fl_e = 1;
                            break;
                        case 'p':
                            fl_p = 1;
                            break;
                        case 't':
                            fl_t = 1;
                            break;
                        default:
                            printf("ValidationError. Wrong syntax of parameters. There "
                                   "is no such parameter.\n");
                            return 2;
                    }
                break;
            }
            else break;
        case 1:
            printf("ValidationError: No such file or directory.\n");
            return 1;
        case 2:
            printf("ValidationError. Wrong syntax of parameters.\n");
            return 2;
        default: break;
    }

    switch (readInputData(inputFile, &A, &B, &X, &n))
    {
        case 0:
            break;

        case 1:
            printf("ValidationError. Incorrect input.\n");
            return 3;

        default: break;
    }

    tmp = malloc(n * sizeof(double));

    clock_t begin = clock();
    lss_01_03(n, A, B, X, tmp);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    if(fl_t) printf("Execution time: %1.9lf\n", time_spent);

    writeAnswer(outputFile, n, X);

    return 0;
}