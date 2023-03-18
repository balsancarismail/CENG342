#include "hellomake.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <matrix_rows> <matrix_cols> <output_filename>\n", argv[0]);
        return 1;
    }

    int matrix_rows = atoi(argv[1]);
    int matrix_cols = atoi(argv[2]);
    char *output_filename = argv[3];

    // Set the seed for the random number generator
    srand(time(NULL));

    // Allocate memory for the matrix
    double **matrix = (double **)malloc(matrix_rows * sizeof(double *));
    for (int i = 0; i < matrix_rows; i++) {
        matrix[i] = (double *)calloc(matrix_cols, sizeof(double));
        for (int j = 0; j < matrix_cols; j++) {
            // Fill the matrix with random double values between 1.0 and 100.0
            matrix[i][j] = ((double)rand() / RAND_MAX) * 99.0 + 1.0;
        }
    }

    // Allocate memory for the vector
    double *vector = (double *)calloc(matrix_cols, sizeof(double));
    for (int i = 0; i < matrix_cols; i++) {
        // Fill the vector with random double values between 1.0 and 100.0
        vector[i] = ((double)rand() / RAND_MAX) * 99.0 + 1.0;
    }

    // Perform matrix-vector multiplication
    double *result = (double *)calloc(matrix_rows, sizeof(double));
    for (int i = 0; i < matrix_rows; i++) {
        for (int j = 0; j < matrix_cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    // Write the result to a file
    FILE *output_file = fopen(output_filename, "w");
    for (int i = 0; i < matrix_rows; i++) {
        fprintf(output_file, "%.2f\n", result[i]);
    }
    fclose(output_file);

    // Free memory
    for (int i = 0; i < matrix_rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
