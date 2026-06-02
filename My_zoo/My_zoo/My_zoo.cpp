#include <iostream>
#include "Vector2.h"
#include "VectorN.h"
#include "Hash.h"

int main() {
        std::string str1 = "Hello, world!";
        std::string str2 = "Hello, world!";
        //uint32_t seed = 42;  // любое 32-битное число
        int tmp = 0;
        std::string tmp1 = "123d";
        std_plus::VectorN<int> v{ 1, 2, 3 };
        std::cout << std_plus::HashFunction<std::string>(str1) << std::endl;
        std::cout << std_plus::HashFunction<std::string>(str2) << std::endl;
        std::cout << std_plus::HashFunction<int>(tmp) << std::endl;
        return 0;
}

