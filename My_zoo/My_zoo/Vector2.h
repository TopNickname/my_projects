#pragma once
//#include "VectorN.h"
#include "ArrayN.h"
#include <cmath>

class Vector2 : public std_plus::ArrayN<double,2> {
	using Base = std_plus::ArrayN<double, 2>;
public:
	using Base::Base;
	Vector2(const Base& other) : Base(other) {}
	double& x() { return (*this)[0]; }
	double& y() { return (*this)[1]; }
};