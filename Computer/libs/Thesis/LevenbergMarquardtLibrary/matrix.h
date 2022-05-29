#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void matrixMultiplication(float* matrix1, float* matrix2, int m1, int n1, int m2, int n2, float* result);
void printMatrix(float* matrix, int m, int n);
void BiasJ(float t, int n, float C1, float C2, float T, float* J, float t0);
void transpose(float* m, float* t, int cols, int rows);
int twoD(int collumns, int x, int y);
float determinant(float* M, int rows, int cols);
void deleteCross(float* M, int rows, int cols, int row, int col, float* Mn);
void cofactors(float* M, int rows, int cols, float* CF);
void matrixDivision(float* x, int rows, int cols, float y, float* r);
void inverse(float* M, int rows, int cols, float* I);
void inverse3(float matrix[3][3]);