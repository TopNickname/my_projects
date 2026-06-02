#pragma once
#include "VectorN.h"
#include <algorithm> 
#include <cmath>     
#include <vector>    

namespace std_plus {

    template<typename T>
    class Matrix {
        size_t n = 0;
        size_t m = 0;
        VectorN<VectorN<T>> data;
        void _swap_with_max_el_in_row(Matrix& ma, size_t i0, size_t j0);

    public:
        Matrix(size_t N = 0, size_t M = 0, T el = T());
        Matrix(const Matrix& x);
        size_t size_n() const { return n; }
        size_t size_m() const { return m; }

        const VectorN<T>& operator[](size_t i) const { return data[i]; }
        VectorN<T>& operator[](size_t i) { return data[i]; }

        Matrix operator+(Matrix ma) const;
        Matrix& operator+=(Matrix ma);
        Matrix operator-(Matrix ma) const;
        Matrix& operator-=(Matrix ma);
        Matrix operator*(Matrix ma) const;
        Matrix& operator*=(Matrix ma);

        Matrix& gaus();

        // Решение СЛАУ Ax = b. Возвращает линейное многообразие решений:
        //   - пустой вектор, если решений нет;
        //   - один вектор, если решение единственно;
        //   - первый вектор --- частное решение (вектор смещения),
        //     остальные --- базис ядра матрицы A.
        std::vector<VectorN<T>> gaus(VectorN<T> v);

        std::vector<VectorN<T>> gaus_c(VectorN<T> v) const;

    };

#pragma region constructors

    template<typename T>
    inline Matrix<T>::Matrix(size_t N, size_t M, T el) {
        n = N;
        m = M;
        data.resize(n);
        for (size_t i = 0; i < n; i++) {
            data[i] = VectorN<T>(m, el);
        }
    }

    template<typename T>
    inline Matrix<T>::Matrix(const Matrix<T>& x) {
        n = x.n;
        m = x.m;
        data.resize(n);
        for (size_t i = 0; i < n; i++) {
            data[i] = x.data[i];
        }
    }

#pragma endregion

#pragma region cout

    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Matrix<T>& ma) {
        for (size_t i = 0; i < ma.size_n(); i++) {
            os << ma[i] << '\n';
        }
        return os;
    }

#pragma endregion

#pragma region math

    template<typename T>
    inline Matrix<T> Matrix<T>::operator+(Matrix ma) const {
        if (ma.size_m() != (*this).size_m() || ma.size_n() != (*this).size_n()) {
            throw "error";
        }
        Matrix<T> tmp(size_n(), size_m(), 0);
        for (size_t i = 0; i < size_n(); i++) {
            for (size_t j = 0; j < size_m(); j++) {
                tmp[i][j] = (*this)[i][j] + ma[i][j];
            }
        }
        return tmp;
    }

    template<typename T>
    inline Matrix<T> Matrix<T>::operator-(Matrix ma) const {
        if (ma.size_m() != (*this).size_m() || ma.size_n() != (*this).size_n()) {
            throw "error";
        }
        Matrix<T> tmp(size_n(), size_m(), 0);
        for (size_t i = 0; i < size_n(); i++) {
            for (size_t j = 0; j < size_m(); j++) {
                tmp[i][j] = (*this)[i][j] - ma[i][j];
            }
        }
        return tmp;
    }

    template<typename T>
    inline Matrix<T>& Matrix<T>::operator+=(Matrix ma) {
        if (ma.size_m() != (*this).size_m() || ma.size_n() != (*this).size_n()) {
            throw "error";
        }
        for (size_t i = 0; i < size_n(); i++) {
            for (size_t j = 0; j < size_m(); j++) {
                (*this)[i][j] += ma[i][j];
            }
        }
        return *this;
    }

    template<typename T>
    inline Matrix<T>& Matrix<T>::operator-=(Matrix ma) {
        if (ma.size_m() != (*this).size_m() || ma.size_n() != (*this).size_n()) {
            throw "error";
        }
        for (size_t i = 0; i < size_n(); i++) {
            for (size_t j = 0; j < size_m(); j++) {
                (*this)[i][j] -= ma[i][j];
            }
        }
        return *this;
    }

    template<typename T>
    inline Matrix<T> Matrix<T>::operator*(Matrix ma) const {
        if (ma.size_n() != (*this).size_m()) {
            throw "error";
        }
        Matrix<T> tmp((*this).size_n(), ma.size_m(), 0);
        for (size_t i = 0; i < (*this).size_n(); i++) {
            for (size_t j = 0; j < ma.size_m(); j++) {
                T sum = T(0);
                for (size_t k = 0; k < (*this).size_m(); k++) {
                    sum += (*this)[i][k] * ma[k][j];
                }
                tmp[i][j] = sum;
            }
        }
        return tmp;
    }

    template<typename T>
    inline Matrix<T>& Matrix<T>::operator*=(Matrix ma) {
        (*this) = (*this) * ma;
        return (*this);
    }

#pragma endregion

#pragma region gaus

    template<typename T>
    inline void Matrix<T>::_swap_with_max_el_in_row(Matrix& ma, size_t i0, size_t j0) {
        size_t max_row = i0;
        T max_val = std::abs(ma[i0][j0]);
        for (size_t i = i0 + 1; i < ma.size_n(); i++) {
            if (std::abs(ma[i][j0]) > max_val) {
                max_val = std::abs(ma[i][j0]);
                max_row = i;
            }
        }
        if (max_row != i0) {
            std::swap(ma.data[i0], ma.data[max_row]);
        }
    }

    template<typename T>
    inline Matrix<T>& Matrix<T>::gaus() {
        size_t diag = std::min(n, m);
        for (size_t i = 0; i < diag; i++) {
            _swap_with_max_el_in_row(*this, i, i);
            if (std::abs((*this)[i][i]) < std::numeric_limits<T>::epsilon()) {
                continue;
            }
            for (size_t r = i + 1; r < n; r++) {
                T factor = (*this)[r][i] / (*this)[i][i];
                (*this)[r][i] = T(0);
                for (size_t c = i + 1; c < m; c++) {
                    (*this)[r][c] -= factor * (*this)[i][c];
                }
            }
        }
        return *this;
    }

    template<typename T>
    inline std::vector<VectorN<T>> Matrix<T>::gaus(VectorN<T> v) {
        if (v.size() != n) {
            throw std::runtime_error("Vector size must match number of rows");
        }

        // Построим расширенную матрицу [A | b]
        Matrix<T> aug(n, m + 1, T(0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                aug[i][j] = (*this)[i][j];
            }
            aug[i][m] = v[i];
        }

        T eps = std::numeric_limits<T>::epsilon();
        std::vector<size_t> pivot_cols;   // ведущие столбцы
        size_t row = 0;                    // текущая строка для ведущего элемента

        // Прямой ход: приведение к ступенчатому виду с выбором ведущего элемента
        for (size_t col = 0; col < m && row < n; ++col) {
            aug._swap_with_max_el_in_row(aug, row, col);

            if (std::abs(aug[row][col]) <= eps) {
                continue;   // в этом столбце нет ненулевого элемента, переходим к следующему
            }

            pivot_cols.push_back(col);

            // Исключение под ведущим элементом
            for (size_t i = row + 1; i < n; ++i) {
                T factor = aug[i][col] / aug[row][col];
                aug[i][col] = T(0);
                for (size_t j = col + 1; j <= m; ++j) {   // включая столбец b
                    aug[i][j] -= factor * aug[row][j];
                }
            }
            ++row;
        }

        size_t rank = row;   // ранг матрицы

        // Проверка совместности: нулевые строки A с ненулевыми b
        for (size_t i = rank; i < n; ++i) {
            if (std::abs(aug[i][m]) > eps) {
                return {};   // решений нет
            }
        }

        // ---- Построение многообразия решений ----
        std::vector<VectorN<T>> result;

        // 1. Частное решение (свободные переменные = 0)
        VectorN<T> x_part(m, T(0));
        for (size_t i = rank; i-- > 0; ) {
            size_t pc = pivot_cols[i];
            T sum = aug[i][m];
            for (size_t j = pc + 1; j < m; ++j) {
                sum -= aug[i][j] * x_part[j];
            }
            x_part[pc] = sum / aug[i][pc];
        }
        result.push_back(x_part);

        // 2. Базис ядра (для каждой свободной переменной)
        // Свободные столбцы — все индексы, не попавшие в pivot_cols
        std::vector<bool> is_pivot(m, false);
        for (size_t pc : pivot_cols) {
            is_pivot[pc] = true;
        }

        for (size_t free_col = 0; free_col < m; ++free_col) {
            if (!is_pivot[free_col]) {
                VectorN<T> basis(m, T(0));
                basis[free_col] = T(1);

                // Решаем A * basis = 0, используя ту же верхнюю ступенчатую форму
                for (size_t i = rank; i-- > 0; ) {
                    size_t pc = pivot_cols[i];
                    T sum = T(0);
                    for (size_t j = pc + 1; j < m; ++j) {
                        sum += aug[i][j] * basis[j];
                    }
                    basis[pc] = -sum / aug[i][pc];
                }
                result.push_back(basis);
            }
        }

        return result;
    }

    template<typename T>
    inline std::vector<VectorN<T>> Matrix<T>::gaus_c(VectorN<T> v) const {
        Matrix<T> copy(*this);
        return copy.gaus(v);
    }

#pragma endregion

}