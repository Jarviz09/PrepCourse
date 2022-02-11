#include <stdio.h>

#include "matrix.h"

int main(void) {
    Matrix *matrix = {0};
    Matrix *matrix_one = {0};
    Matrix *matrix_two = {0};

    if ((matrix_one = create_matrix_from_file(PATH_FILE)) == NULL) {
        return (-1);
    }

    size_t rows = 0;
    size_t cols = 0;

    if ((matrix_two = create_matrix(rows, cols)) == NULL) {
        return (-1);
    }

    size_t row = 0;
    size_t col = 0;

    get_rows(matrix_one, &row);
    get_cols(matrix_one, &col);

    double val = 0;
    double value = 0;

    get_elem(matrix_one, row, col, &val);
    set_elem(matrix_one, row, col, value);

    if ((matrix = mul_scalar(matrix, val)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    if ((matrix = transp(matrix)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    if ((matrix = sum(matrix_one, matrix_two)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    if ((matrix = sub(matrix_one, matrix_two)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    if ((matrix = mul(matrix_one, matrix_two)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    det(matrix, &val);
    printf("%lf", val);
    if ((matrix = adj(matrix)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    if ((matrix = inv(matrix)) == NULL) {
        free_matrix(matrix_two);
        free_matrix(matrix_one);
        return (-1);
    }

    free_matrix(matrix);
    free_matrix(matrix_two);
    free_matrix(matrix_one);

    return 0;
}


