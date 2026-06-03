// main.cpp (пример с геттерами)
#include <iostream>
#include "Zoo.h"
#include "Animals.h"

int main() {
    Zoo zoo(10, 10);
    zoo.addAnimal(std::make_unique<Cat>());   // ID = 1
    zoo.addAnimal(std::make_unique<Duck>());  // ID = 2

    const auto& animals = zoo.getAnimals();
    std::cout << "Animal 1 pos: " << animals.at(1)->getPos().x()
        << ", " << animals.at(1)->getPos().y() << "\n";

    zoo.moveAnimal(1, Vectori2{ 9, 9 });
    std::cout << "After move: " << animals.at(1)->getPos().x()
        << ", " << animals.at(1)->getPos().y() << "\n";

    // Использование геттера ID
    std::cout << "Animal 2 ID: " << animals.at(2)->getID() << "\n";
    return 0;
}