#include "lss_01_03.h"

#define AT(A, x, y, n) (A+(x)*(n)+(y))

int fl_d = 0,
        fl_e = 0,
        fl_p = 0,
        fl_t = 0,
        fl_h = 0,
        fl_q = 0;

int _strlen(char const *input){
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

    int validationError = 0;
    for(i = 3; i < argc; i++)
        if(argv[i][0] == '-' && _strlen(argv[i]) == 2)
            switch (argv[i][1])
            {
                case 'd':
                    fl_d = 1;
                    break;
                case 'h':
                    fl_h = 1;
                    break;
                case '?':
                    fl_q = 1;
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
                    validationError = 1;
                    break;
            }

    if(validationError)
        return 3;

    if(validateFile(argv[1]) || validateFile(argv[2]))
        return 1;

    for(i = 3; i < argc; i++)
        if(argv[i][0] != '-' || _strlen(argv[i]) != 2)
            return 2;

    return 0;
}

int readInputData(char *inputFile, double** A, double** B, double** X, int *n)
{
    int i, checkInput;
    FILE *in = fopen(inputFile, "r");


    checkInput = fscanf(in, "%d", n);
    if (checkInput == EOF)
        return 2;

    if(*n <= 0)
        return 1;

    *A = malloc((*n) * (*n) * sizeof(double));
    *B = malloc((*n) * sizeof(double));
    *X = malloc((*n) * sizeof(double));

    for(i = 0; i < (*n)*(*n); i++) {
        checkInput = fscanf(in, "%lf", (*A + i));
        if (checkInput == EOF)
            return 2;
        if(checkInput == 0)
            return 3;
    }

    for(i = 0; i < (*n); i++){
        checkInput = fscanf(in, "%lf", (*B+i));
        if (checkInput == EOF)
            return 2;
        if(checkInput == 0)
            return 3;
    }

   // if(fscanf(in, "%c", &tmp) != EOF)
   //     return 4;

    return 0;
}

void writeAnswer(char *outputFile, int n, const double* X, int notExist){
    int i;
    FILE *out = fopen(outputFile, "w");
    if(notExist){
        fprintf(out, "%d\n", 0);
    }
    else {
        fprintf(out, "%d\n", n);
        for (i = 0; i < n; i++)
            fprintf(out, "%1.9lf\n", *(X + i));
    }
}

void printHelp(){
    printf("Usage: lss [input_file_name] [output_file_name] [options]\n"
           "Where options include:\n"
           "  -d        print debug messages [default OFF]\n"
           "  -e        print errors [default OFF]\n"
           "  -p        print matrix [default OFF]\n"
           "  -t        print execution time [default OFF]\n"
           "  -h, -?    print this and exit\n"
           "Default input_file_name value is lss_00_00_in.txt, default output_file_name value is lss_00_00_out.txt.");
}

void printMatrix(int n, int m, const double* A){
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            printf("%1.9lf\n ", *AT(A, i, j, n));
        } printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int n;
    double *A, *B, *X, *tmp;
    char* inputFile = "lss_01_03_in.txt";
    char* outputFile = "lss_01_03_out.txt";

    if(argc > 1){
        inputFile = argv[1];
        outputFile = argv[2];
    }

    switch (validateParameters(argc, argv))
    {
        case 1:
            if(fl_e) printf("ValidationError: No such file or directory.\n");
            return 2;
        case 2:
            if(fl_e) printf("ValidationError. Wrong syntax of parameters.\n");
            return 3;
        case 3:
            if(fl_e) printf("ValidationError. Wrong syntax of parameters. There "
                       "is no such parameter.\n");
            return 4;

        default: break;
    }

    if(fl_q || fl_h){
        printHelp();
        return 0;
    }

    switch (readInputData(inputFile, &A, &B, &X, &n))
    {
        case 1:
            if(fl_e) printf("ValidationError. Incorrect input.\n");
            return 5;
        case 2:
            if(fl_e) printf("ValidationError. Incorrect number of coefficients. (Not enough) \n");
            return 6;
        case 3:
            if(fl_e) printf("ValidationError. One of the coefficients not a number \n");
            return 7;
        default: break;
    }

    if(fl_p){
        printMatrix(n, n, A);
    }

    tmp = malloc(lss_memsize_SS_NN(n));

    clock_t begin = clock();

    int notExist;
    notExist = lss_01_03(n, A, B, X, tmp);

    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    if(fl_t) printf("Execution time: %1.9lf\n", timeSpent);

    writeAnswer(outputFile, n, X, notExist);

    return 0;
}