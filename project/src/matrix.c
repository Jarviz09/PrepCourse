#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

static int det_mini_matrix(const Matrix *matrix, size_t size,
                           size_t row, size_t col, Matrix *new_matrix);

Matrix* create_matrix_from_file(const char* path_file) {
    size_t row;
    size_t col;
    FILE* read_file = fopen(path_file, "r");
    if (read_file == NULL) {
        return NULL;
    }

    if (fscanf(read_file, "%zu %zu\n", &row, &col) != 2) {
        fclose(read_file);
        return NULL;
    }

    Matrix *matrix;
    if ((matrix = create_matrix(row, col)) == NULL) {
        fclose(read_file);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            if (fscanf(read_file, "%lf", &matrix->elem[i][j]) != 1) {
                free_matrix(matrix);
                fclose(read_file);
                return NULL;
            }
        }
    }
    fclose(read_file);
    return matrix;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
        return NULL;
    }

    Matrix* matrix = malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }

    matrix->elem = malloc(sizeof(double*) * rows);
    if (matrix->elem == NULL) {
        free(matrix);
        return NULL;
    }

    for (size_t i = 0; i < rows; ++i) {
        matrix->elem[i] = malloc(sizeof(double) * cols);
        if (matrix->elem[i] == NULL) {
            free_matrix(matrix);
            return NULL;
        }
    }

    matrix->rows = rows;
    matrix->cols = cols;

    return matrix;
}

int free_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        return (-1);
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        free(matrix->elem[i]);
    }

    free(matrix->elem);
    free(matrix);

    return 0;
}

// Basic operations

int get_rows(const Matrix* matrix, size_t* row) {
    if (matrix == NULL || row == NULL) {
        return (-1);
    }

    *row = matrix->rows;

    return 0;
}

int get_cols(const Matrix* matrix, size_t* col) {
    if (matrix == NULL || col == NULL) {
        return (-1);
    }

    *col = matrix->cols;

    return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (matrix == NULL || val == NULL) {
        return (-1);
    }

    if (row >= matrix->rows || col >= matrix->cols) {
        return (-1);
    }

    *val = matrix->elem[row][col];

    return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (matrix == NULL) {
        return (-1);
    }

    if (row >= matrix->rows || col >= matrix->cols) {
        return (-1);
    }

    matrix->elem[row][col] = val;

    return 0;
}

// Math operations

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (matrix == NULL) {
        return NULL;
    }

    Matrix* matrix_result = create_matrix(matrix->rows, matrix->cols);
    if (matrix_result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix_result->rows; ++i) {
        for (size_t j = 0; j < matrix_result->cols; ++j) {
            matrix_result->elem[i][j] = matrix->elem[i][j] * val;
        }
    }

    return matrix_result;
}

Matrix* transp(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    Matrix* matrix_result = create_matrix(matrix->cols, matrix->rows);
    if (matrix_result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix_result->rows; ++i) {
        for (size_t j = 0; j < matrix_result->cols; ++j) {
            matrix_result->elem[i][j] = matrix->elem[j][i];
        }
    }

    return matrix_result;
}

Matrix* sum(const Matrix* matrix_one, const Matrix* matrix_two) {
    if (matrix_one == NULL || matrix_two == NULL) {
        return NULL;
    }

    if (matrix_one->rows != matrix_two->rows || matrix_one->cols != matrix_two->cols) {
        return NULL;
    }

    Matrix* matrix_result = create_matrix(matrix_one->rows, matrix_one->cols);
    if (matrix_result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix_result->rows; ++i) {
        for (size_t j = 0; j < matrix_result->cols; ++j) {
            matrix_result->elem[i][j] = matrix_one->elem[i][j] + matrix_two->elem[i][j];
        }
    }

    return matrix_result;
}

Matrix* sub(const Matrix* matrix_one, const Matrix* matrix_two) {
    if (matrix_one == NULL || matrix_two == NULL) {
        return NULL;
    }

    if (matrix_one->rows != matrix_two->rows || matrix_one->cols != matrix_two->cols) {
        return NULL;
    }

    Matrix* matrix_result = create_matrix(matrix_one->rows, matrix_one->cols);
    if (matrix_result == NULL) {;
        return NULL;
    }

    for (size_t i = 0; i < matrix_result->rows; ++i) {
        for (size_t j = 0; j < matrix_result->cols; ++j) {
            matrix_result->elem[i][j] = matrix_one->elem[i][j] - matrix_two->elem[i][j];
        }
    }

    return matrix_result;
}

Matrix* mul(const Matrix* matrix_one, const Matrix* matrix_two) {
    if (matrix_one == NULL || matrix_two == NULL) {
        return NULL;
    }

    if (matrix_one->cols != matrix_two->rows) {
        return NULL;
    }

    Matrix* matrix_result = create_matrix(matrix_one->rows, matrix_two->cols);
    if (matrix_result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < matrix_result->rows; ++i) {
        for (size_t j = 0; j < matrix_result->cols ; ++j) {
            matrix_result->elem[i][j] = 0;
            for (size_t k = 0; k < matrix_one->cols; ++k) {
                matrix_result->elem[i][j] += matrix_one->elem[i][k] * matrix_two->elem[k][j];
             }
        }
    }

    return matrix_result;
}

// Extra operations

int det(const Matrix* matrix, double* val) {
    double deter = 0;
    double value = 0;
    int degree = 1;

    if (matrix == NULL || val == NULL) {
        return (-1);
    }

    if (matrix->rows != matrix->cols) {
        return (-1);
    }

    if (matrix->rows == 1) {
        *val = matrix->elem[0][0];
        return 0;
    }

    if (matrix->rows == 2) {
        *val = matrix->elem[0][0] * matrix->elem[1][1] -
        matrix->elem[0][1] * matrix->elem[1][0];
        return 0;
    }

    Matrix *new_matrix = create_matrix(matrix->rows - 1, matrix->cols - 1);
    if (new_matrix == NULL) {
        return (-1);
    }

    for (size_t j = 0; j < matrix->rows; ++j) {
        det_mini_matrix(matrix, matrix->rows, 0, j, new_matrix);

        int res = det(new_matrix, &value);

        if (res != 0) {
            free_matrix(new_matrix);
            return (-1);
        }

        deter = deter + (degree * matrix->elem[0][j] * value);
        degree = -degree;
    }

    free_matrix(new_matrix);
    *val = deter;

    return 0;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    if (matrix->rows != matrix->cols) {
        return NULL;
    }

    Matrix *matrix_result = create_matrix(matrix->rows, matrix->cols);
    if (matrix_result == NULL) {
        return NULL;
    }

    if (matrix->rows == 1) {
        matrix_result->elem[0][0] = matrix->elem[0][0];
        return matrix_result;
    }

    Matrix *new_matrix = create_matrix(matrix->rows - 1, matrix->cols - 1);
    if (new_matrix == NULL) {
        free_matrix(matrix_result);
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            det_mini_matrix(matrix, matrix->rows, i, j, new_matrix);

            double val;
            int res = det(new_matrix, &val);
            if (res != 0) {
                free_matrix(matrix_result);
                free_matrix(new_matrix);
                return NULL;
            }

            matrix_result->elem[j][i] = pow(-1, i + j) * val;
        }
    }
    free_matrix(new_matrix);

    return matrix_result;
}

Matrix* inv(const Matrix* matrix) {
    if (matrix == NULL) {
        return NULL;
    }

    if (matrix->rows != matrix->cols) {
        return NULL;
    }

    Matrix *adj_matrix = adj(matrix);
    if (adj_matrix == NULL) {
        return NULL;
    }

    double val;
    int res = det(matrix, &val);
    if (res != 0 || val == 0) {
        free_matrix(adj_matrix);
        return NULL;
    }

    if (matrix->rows == 1) {
        val *= val;
    }

    Matrix *result_matrix = mul_scalar(adj_matrix, 1 / val);
    if (result_matrix == NULL) {
        free_matrix(adj_matrix);
        return NULL;
    }

    free_matrix(adj_matrix);

    return result_matrix;
}

// Utils

static int det_mini_matrix(const Matrix *matrix, size_t size,
                                        size_t row, size_t col, Matrix *new_matrix) {
    int offset_row = 0;

    if (matrix == NULL) {
        return (-1);
    }

    if (new_matrix == NULL) {
        return (-1);
    }

    for (size_t i = 0; i < size - 1; ++i) {
        if (i == row) {
            offset_row = 1;
        }

        int offset_col = 0;
        for (size_t j = 0; j < size - 1; ++j) {
            if (j == col) {
                offset_col = 1;
            }

            new_matrix->elem[i][j] = matrix->elem[i + offset_row][j + offset_col];
        }
    }

    return 0;
}
