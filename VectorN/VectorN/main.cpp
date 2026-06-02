#include <iostream>
#include "ArrayN.h"
#include <algorithm>
#include <vector>
#include "VectorN.h"
#include "mystring.h"
//#include <string.h>

int main() {
	
	std_plus::mystring a = "123345";
	a[1] = '6';
	a.push_back('1');
	std::cout << a<<std::endl;
	std_plus::mystring b;
	b.push_back('1');
	b.push_back('0');
	std::cout << b << std::endl;
	char c_[] = "12346";
	std_plus::mystring c = c_;
	std::cout << c << std::endl;
	std::cout << c + a + b<<std::endl;
}