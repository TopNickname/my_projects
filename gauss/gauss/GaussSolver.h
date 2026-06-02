#pragma once

#include "Matrix.h"   
#include <vector>
#include <stdexcept>

class GaussSolver {
public:
    GaussSolver() {}

    std::vector<Vector> solve(const Matrix& A, const Vector& b) {
        int m = A.m_rows;
        int n = A.n_cols;

        if (m != b.size()) {
            throw std::runtime_error("Vector size must match number of rows");
        }

        std_plus::Matrix<double> A_plus(static_cast<std::size_t>(m),
            static_cast<std::size_t>(n), 0.0);
        for (int i = 0; i < m; ++i) {
            A_plus[static_cast<std::size_t>(i)] = A.rows[i].to_vectorN();
        }

        std_plus::VectorN<double> b_plus(static_cast<std::size_t>(m), 0.0);
        for (int i = 0; i < m; ++i) {
            b_plus[static_cast<std::size_t>(i)] = b[i];
        }

        auto sol_vn = A_plus.gaus(b_plus);

        std::vector<Vector> result;
        result.reserve(sol_vn.size());
        for (const auto& vn : sol_vn) {
            Vector vec(static_cast<int>(vn.size()));
            for (std::size_t j = 0; j < vn.size(); ++j) {
                vec[static_cast<int>(j)] = vn[j];
            }
            result.push_back(std::move(vec));
        }
        return result;
    }
};