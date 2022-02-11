#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

#include "matrix.h"
#include "exceptions.h"

#define EPS 1e-07

namespace prep {

    Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        this->elem.resize(this->rows);

        for (size_t i = 0; i < rows; ++i) {
            this->elem[i].resize(this->cols);
        }
    }

    Matrix::Matrix(std::istream& is) : rows(0), cols(0) {
        if (!is) {
            throw InvalidMatrixStream();
        }

        if (!(is >> rows) || !(is >> cols)) {
            throw InvalidMatrixStream();
        }

        if ((rows == 0) || (cols == 0)) {
            throw InvalidMatrixStream();
        }

        elem.resize(rows);

        for (size_t i = 0; i < rows; ++i) {
            elem[i].resize(cols);
            for (size_t j = 0; j < cols; ++j) {
                if (!(is >> elem[i][j])) {
                    throw InvalidMatrixStream();
                }
            }
        }
    }

    std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << matrix.getRows() << " " << matrix.getCols() << std::endl;

        for (size_t i = 0; i < matrix.rows; ++i) {
            for (size_t j = 0; j < matrix.cols; ++j) {
                if (matrix.elem[i][j] >= 0) {
                    os << " ";
                }

                os << std::setprecision(std::numeric_limits<double>::max_digits10) << matrix.elem[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

    size_t Matrix::getRows() const {
        return this->rows;
    }

    size_t Matrix::getCols() const {
        return this->cols;
    }

    double Matrix::operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols) {
            throw OutOfRange(i, j, *this);
        }
        return elem[i][j];
    }

    double& Matrix::operator()(size_t i, size_t j) {
        if (i >= rows || j >= cols) {
            throw OutOfRange(i, j, *this);
        }
        return elem[i][j];
    }

    bool Matrix::operator==(const Matrix& rhs) const {
        if (this->rows != rhs.rows || this->cols != rhs.cols) {
            return false;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if ((fabs(elem[i][j] - rhs.elem[i][j]) > EPS)) {
                    return false;
                }
            }
        }

        return true;
    }

    bool Matrix::operator!=(const Matrix& rhs) const {
        if (this->operator==(rhs)) {
            return false;
        }

        return true;
    }

    Matrix Matrix::operator+(const Matrix &rhs) const {
        if (this->rows != rhs.getRows() || this->cols != rhs.getCols()) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix Result_matrix(this->rows, this->cols);

        size_t rows = Result_matrix.getRows();
        size_t cols = Result_matrix.getCols();

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                Result_matrix(i, j) = this->elem[i][j] + rhs(i, j);
            }
        }

        return Result_matrix;
    }

    Matrix Matrix::operator-(const Matrix &rhs) const {
        if (this->rows != rhs.getRows() || this->cols != rhs.getCols()) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix Result_matrix(this->rows, this->cols);

        size_t rows = Result_matrix.getRows();
        size_t cols = Result_matrix.getCols();

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                Result_matrix(i, j) = this->elem[i][j] - rhs(i, j);
            }
        }

        return Result_matrix;
    }

    Matrix Matrix::operator*(const Matrix& rhs) const {
        if (this->cols != rhs.getRows()) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix Result_matrix(this->rows, this->cols);

        size_t rows = Result_matrix.getRows();
        size_t cols = Result_matrix.getCols();

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                Result_matrix(i, j) = 0;
                for (size_t t = 0; t < this->cols; ++t) {
                    Result_matrix(i, j) = Result_matrix(i, j) + this->elem[i][t] * rhs(t, j);
                }
            }
        }

        return Result_matrix;
    }

    Matrix Matrix::transp() const {
        Matrix Result_matrix(cols, rows);
        for (size_t i = 0; i < this->cols; ++i) {
            for (size_t j = 0; j < this->rows; ++j) {
                Result_matrix(i, j) = this->elem[j][i];
            }
        }

        return Result_matrix;
    }

    Matrix Matrix::operator*(double val) const {
        Matrix Result_matrix(this->rows, this->cols);
        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                Result_matrix(i, j) = elem[i][j] * val;
            }
        }

        return Result_matrix;
    }

    Matrix operator*(double val, const Matrix& matrix) {
        Matrix Result_matrix(matrix.getRows(), matrix.getCols());
        Result_matrix = matrix.operator*(val);

        return Result_matrix;
    }

    double Matrix::det() const {
        if (this->rows != this->cols) {
            throw DimensionMismatch(*this);
        }

        if (this->rows == 1) {
            return this->elem[0][0];
        }

        if (this->rows == 2) {
            return this->elem[0][0] * this->elem[1][1] - this->elem[0][1] * this->elem[1][0];
        }

        int degree = 1;
        double determinant = 0;

        Matrix Result_matrix(this->rows - 1, this->cols - 1);
        for (size_t j = 0; j < this->rows; ++j) {
            Result_matrix.mini_matrix(0, j, *this);

            determinant += (degree * this->elem[0][j] * Result_matrix.det());
            degree = -degree;
        }

        return determinant;
    }

    Matrix Matrix::adj() const {
        if (this->rows != this->cols) {
            throw DimensionMismatch(*this);
        }

        Matrix Result_matrix(this->rows, this->cols);

        if (Result_matrix.rows == 1) {
            Result_matrix.elem[0][0] = this->elem[0][0];
            return Result_matrix;
        }

        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->cols; ++j) {
                Matrix New_matrix(this->rows - 1, this->cols - 1);
                New_matrix.mini_matrix(i, j, *this);

                Result_matrix.elem[j][i] = pow(-1, i + j) * New_matrix.det();
            }
        }

        return Result_matrix;
    }

    Matrix Matrix::inv() const {
        if (this->rows != this->cols) {
            throw DimensionMismatch(*this);
        }

        Matrix matrixAdj(rows, cols);
        matrixAdj = this->adj();

        double val = this->det();
        if (val == 0) {
            throw SingularMatrix();
            exit(1);
        }
        if (this->rows == 1) {
            val *= val;
        }

        Matrix Result_matrix(rows, cols);
        Result_matrix = matrixAdj * (1 / val);

        return Result_matrix;
    }

    void Matrix::mini_matrix(size_t row, size_t col, const Matrix &matrix) {
        size_t shiftRow = 0;
        for (size_t i = 0; i < this->rows; ++i) {
            if (i == row) {
                shiftRow = 1;
            }

            size_t shiftCol = 0;
            for (size_t j = 0; j < this->cols; ++j) {
                if (j == col) {
                    shiftCol = 1;
                }
                this->elem[i][j] = matrix.elem[i + shiftRow][j + shiftCol];
            }
        }
    }
}  // namespace prep
