#include "pch.h"
#include "matrix.h"


void matrixMultiplication(float* matrix1, float* matrix2, int m1, int n1, int m2, int n2, float* result) {
    if (n1 != m2) return;
    for (int col = 0; col < n2; col++) {
        for (int row = 0; row < m1; row++) {
            result[twoD(n2, col, row)] = 0;
            for (int i = 0; i < n1; i++) {
                result[twoD(n2, col, row)] += (matrix1[twoD(n1, i, row)] * matrix2[twoD(n2, col, i)]);
            }
        }
    }
}

void printMatrix(float* matrix, int m, int n) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < m; col++) {
            printf("%f\t", matrix[twoD(m, col, row)]);
        }
        printf("\n");
    }
}

void BiasJ(float t, int n, float C1, float C2, float T, float* J, float t0) {
    for (int i = 0; i < n; i++) {
        float ti = t0 + t * i;
        J[twoD(3, 0, i)] = 1 - exp(-ti / T);
        J[twoD(3, 1, i)] = 1;
        J[twoD(3, 2, i)] = -C1 * ti / pow(T, 2) * exp(-ti / T);
    }
}

void transpose(float* m, float* t, int cols, int rows) {
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            t[twoD(rows, row, col)] = m[twoD(cols, col, row)];
        }
    }
}



void inverse3(float matrix[3][3]) {
    float det = (matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]));
    det += (matrix[0][1] * (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]));
    det += (matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]));
    printf("\nInverse of matrix is: \n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            printf("%f\t", ((matrix[(j + 1) % 3][(i + 1) % 3] * matrix[(j + 2) % 3][(i + 2) % 3]) - (matrix[(j + 1) % 3][(i + 2) % 3] * matrix[(j + 2) % 3][(i + 1) % 3])) / det);
        printf("\n");
    }
}

int twoD(int collumns, int x, int y) {
    return collumns * (y)+x;
}

float determinant(float* M, int rows, int cols) {
    float D = 0;
    if (rows == 2 && cols == 2) {
        D = M[twoD(cols, 0, 0)] * M[twoD(cols, 1, 1)] - M[twoD(cols, 1, 0)] * M[twoD(cols, 0, 1)];
    }
    else {
        float* M1 = (float*)malloc((rows - 1) * (cols - 1) * sizeof(float));
        for (int col = 0; col < cols; col++) {
            deleteCross(M, rows, cols, 0, col, M1);
            D += (pow(-1, col) * M[twoD(cols, col, 0)] * determinant(M1, rows - 1, cols - 1));
        }
        free(M1);
    }
    return D;
}

void deleteCross(float* M, int rows, int cols, int row, int col, float* Mn) {
    int k = 0;
    for (int i = 0; k < (cols - 1); i++, k++) {
        int l = 0;
        if (i == col) i++;
        for (int j = 0; l < (rows - 1); j++, l++) {
            if (j == row) j++;
            Mn[twoD(cols - 1, k, l)] = M[twoD(cols, i, j)];
        }
    }
}

void cofactors(float* M, int rows, int cols, float* CF) {
    float* cross = (float*)malloc((rows - 1) * (cols - 1) * sizeof(float));
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            deleteCross(M, rows, cols, row, col, cross);
            CF[twoD(cols, col, row)] = pow(-1, row + col) * determinant(cross, rows - 1, cols - 1);
        }
    }
    free(cross);
}

void matrixDivision(float* x, int rows, int cols, float y, float* r) {
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            r[twoD(cols, col, row)] = x[twoD(cols, col, row)] / y;
        }
    }
}

void inverse(float* M, int rows, int cols, float* I) {
    float* temp = (float*)malloc(rows * cols * sizeof(float));
    transpose(M, temp, cols, rows);
    cofactors(temp, rows, cols, I);
    free(temp);
    matrixDivision(I, rows, cols, determinant(M, rows, cols), I);
}