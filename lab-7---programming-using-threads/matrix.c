#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MATRIX_SIZE 20

int matA[MATRIX_SIZE][MATRIX_SIZE];
int matB[MATRIX_SIZE][MATRIX_SIZE];


int matSumResult[MATRIX_SIZE][MATRIX_SIZE];
int matDiffResult[MATRIX_SIZE][MATRIX_SIZE];
int matProductResult[MATRIX_SIZE][MATRIX_SIZE];


//int MAX;

typedef struct {
    int row;
    int col;
} ThreadArg;

void fillMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i;
    int j;
    for(i = 0; i<MATRIX_SIZE; i++) {
        for(j = 0; j<MATRIX_SIZE; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}


void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i;
    int j;
    for(i = 0; i<MATRIX_SIZE; i++) {
        for(j = 0; j<MATRIX_SIZE; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    ThreadArg* arg = (ThreadArg*)args;
    int row = arg->row;
    int col = arg->col;

    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(arg);
    return NULL;
}


// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    ThreadArg* arg = (ThreadArg*)args;
    int row = arg->row;
    int col = arg->col;

    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(arg);
    return NULL;
}


// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    ThreadArg* arg = (ThreadArg*)args;
    int row = arg->row;
    int col = arg->col;

    matProductResult[row][col] = 0;
    int k;
    for(k = 0; k < MATRIX_SIZE; k++){
        matProductResult[row][col] += matA[row][k] + matB[k][col];
    }
    free(arg);
    return NULL;
}


// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    pthread_t threads[MATRIX_SIZE * MATRIX_SIZE * 3];
    int threadIndex = 0;

    srand(time(0));  // Do Not Remove. Just ignore and continue below.
   
    // 0. Get the matrix size from the command line and assign it to MAX
   
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
   
    // 3. Create pthread_t objects for our threads.
    int i;
    int j;
    for (i = 0; i < MATRIX_SIZE; i++){
        for (j = 0; j < MATRIX_SIZE; j++){
            ThreadArg* sumArg = (ThreadArg*)malloc(sizeof(ThreadArg));
            ThreadArg* diffArg = (ThreadArg*)malloc(sizeof(ThreadArg));
            ThreadArg* prodArg = (ThreadArg*)malloc(sizeof(ThreadArg));

            sumArg->row = diffArg->row = prodArg->row = i;
            sumArg->col = diffArg->col = prodArg->col = j;

            pthread_create(&threads[threadIndex++], NULL, computeSum, sumArg);
            pthread_create(&threads[threadIndex++], NULL, computeDiff, diffArg);
            pthread_create(&threads[threadIndex++], NULL, computeProduct, prodArg);
        }
    }
    // 4. Create a thread for each cell of each matrix operation.
    //
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    //
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
   
    // 5. Wait for all threads to finish.
    for (i = 0; i < threadIndex; i++){
        pthread_join(threads[i], NULL);
   }
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
 
}
