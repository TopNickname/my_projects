#pragma once
#include "VectorN.h"
#include "ArrayN.h"
#include <cmath>

class Vector2 : public std_plus::ArrayN<double, 2> {
	using Base = std_plus::ArrayN<double, 2>;
public:
	using Base::Base;
	Vector2(const Base& other) : Base(other) {}
	double& x() { return (*this)[0]; }
	double& y() { return (*this)[1]; }
	Vector2(double _x, double _y) {
		(*this)[0] = _x;
		(*this)[1] = _y;
	}
};

class Vectori2 : public std_plus::ArrayN<int, 2> {
	using Base = std_plus::ArrayN<int, 2>;
public:
	using Base::Base;
	Vectori2(const Base& other) : Base(other) {}
	int& x() { return (*this)[0]; }
	int& y() { return (*this)[1]; }
	Vectori2(int _x, int _y) {
		(*this)[0] = _x;
		(*this)[1] = _y;
	}
};