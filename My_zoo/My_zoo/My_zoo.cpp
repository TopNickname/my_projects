#include <iostream>
#include "Vector2.h"
#include "VectorN.h"
#include "Hash.h"
#include "Animals.h"

int main() {
        std::string str1 = "Hello, world!";
        std_plus::HashSet<std::string> H;
        std_plus::HashTable<int,std::string> H1;
        Cat c;
        for (int i = 0; i < 10; i++) {
            H.add(str1);
            H1.add(i, str1);
            str1 += 'a'+(i)%20;
        }
        for (auto x : H) {
            std::cout << x << std::endl;
        }
        for (auto x : H1) {
            std::cout << x.first<<' '<<x.second << std::endl;
        }
        return 0;
}

