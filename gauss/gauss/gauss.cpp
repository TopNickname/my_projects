
#include "Matrix.h"
#include "GaussSolver.h"
#include "vector"

int main() {
	Matrix a(2, 2);
	Vector c(2);
	c[0] = 1;
	a[0][0] = 1;
	a[0][1] = 2;
	a[1][1] = 4;
	a[1][0] = 3;
	std::vector<Vector> b;
	GaussSolver g;
	b = g.solve(a, c);
	for (auto x : b) {
		for (int i=0;i<x.size();i++)std::cout << x[i]<<' ';
		std::cout << std::endl;
	}
	return 0;
}