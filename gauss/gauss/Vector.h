#pragma once

#include "VectorN.h"   // мой вектор

//вектор по тз

class Vector { 
    std_plus::VectorN<double> impl;
public:
    Vector() : impl() {}

    explicit Vector(int n)
        : impl(static_cast<std::size_t>(n), 0.0) {
    }

    Vector(const Vector& other) = default;
    Vector(Vector&& other) noexcept = default;
    Vector& operator=(const Vector& other) = default;
    Vector& operator=(Vector&& other) noexcept = default;

    double& operator[](int i) {
        return impl[static_cast<std::size_t>(i)];
    }

    const double& operator[](int i) const {
        return impl[static_cast<std::size_t>(i)];
    }

    int size() const {
        return static_cast<int>(impl.size());
    }

    const std_plus::VectorN<double>& to_vectorN() const { return impl; }
    std_plus::VectorN<double>& to_vectorN() { return impl; }
};
