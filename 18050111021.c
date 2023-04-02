//
// Created by Asus on 2.04.2023.
//
#define MASTER 0

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **allocate_matrix_2d(int rows, int cols) {
    double **matrix = (double **) malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *) malloc(cols * sizeof(double));
    }
    return matrix;
}

double *allocate_matrix_1d(int rows, int cols) {
    double *matrix = (double *) malloc(rows * cols * sizeof(double));
    return matrix;
}

void initialize_matrix_2d(double **matrix, int rows, int cols) {
    srand(time(NULL));  // Seed the random number generator
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (double) rand() / RAND_MAX;
        }
    }
}

void initialize_matrix_1d(double *matrix, int rows, int cols) {
    srand(time(NULL));  // Seed the random number generator
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = (double) rand() / RAND_MAX;
    }
}

void print_matrix(double *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}
void free_1d_array(double* arr) {
    free(arr);
}

void free_2d_array(double** arr, int rows) {
    for (int i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}
void RowMatrixVectorMultiply(int dim, double *matrix_data, double *vector_data,double *result, char *output_file){
    FILE *fp;
    fp = fopen(output_file, "a+");
    int rank,size;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    double* localresult = allocate_matrix_1d(dim / size,1);
    double matrix[dim][dim];  //local matrix
    double start_time = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(matrix_data, (dim*dim)/size, MPI_DOUBLE, matrix, (dim*dim)/size, MPI_DOUBLE, MASTER, MPI_COMM_WORLD); //Scatter the Matrix
    MPI_Bcast(vector_data, dim, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);// Broadcast the Vector

    //Calculate the results
    for (int i = 0;i<(dim/size);i++)
        for (int j = 0;j<dim;j++)
            localresult[i]+=vector_data[j]*matrix[i][j];

    MPI_Gather(localresult, (dim)/size, MPI_DOUBLE, result, (dim)/size, MPI_DOUBLE, MASTER, MPI_COMM_WORLD); // Gather the results
    double end_time = MPI_Wtime();

    free_1d_array(localresult);

    if (rank == MASTER) {
        //print_matrix(result, dim, 1);
        fprintf(fp, "Elapsed time is %.3f seconds for parallel mxv with %d processes\n", end_time - start_time, size);
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == MASTER) {
            fprintf(stderr, "usage: %s <large matrix size> <small matrix size>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int dim = atoi(argv[1]);
    char *output_file = argv[2];

    double **matrix_data = allocate_matrix_2d(dim, dim);
    double *vector_data = allocate_matrix_1d(dim, 1);
    double *result = allocate_matrix_1d(dim, 1);

    if (rank == MASTER) {
        initialize_matrix_2d(matrix_data, dim, dim);
        initialize_matrix_1d(vector_data, dim, 1);
    }

    RowMatrixVectorMultiply(dim, (double *) matrix_data, vector_data, result, output_file);

    MPI_Finalize();

    free_1d_array(vector_data);
    free_1d_array(result);
    free_2d_array(matrix_data, dim);

    return 0;
}
