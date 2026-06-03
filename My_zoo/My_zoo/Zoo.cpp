// Zoo.cpp
#include "Zoo.h"

Zoo::Zoo(int gridWidth, int gridHeight)
    : grid(gridWidth, gridHeight)
{
}

void Zoo::addAnimal(std::unique_ptr<Animal> animal) {
    if (!animal) return;   // защита от nullptr

    int id = nextId++;
    animal->id = id;       // требуется поле id в Animal (friend Zoo)

    std::shared_ptr<Animal> shared = std::move(animal);
    animals.add(id, shared);
}

bool Zoo::moveAnimal(int animalId, Vectori2 newPos) {
    if (!animals.contains(animalId))
        return false;

    Animal& animal = *animals.at(animalId); // at возвращает const shared_ptr&, разыменование даёт Animal&
    if (grid.canAnimalMoveTo(animal, newPos)) {
        animal.tp(newPos);
        return true;
    }
    return false;
}

bool Zoo::moveAnimalRelative(int animalId, int dx, int dy) {
    if (!animals.contains(animalId))
        return false;

    Animal& animal = *animals.at(animalId);
    int curX = animal.pos.x();
    int curY = animal.pos.y();
    return moveAnimal(animalId, Vectori2{ curX + dx, curY + dy });
}

bool Zoo::moveAnimal(Animal& animal, Vectori2 newPos) {
    return moveAnimal(animal.id, newPos);
}

bool Zoo::moveAnimalRelative(Animal& animal, int dx, int dy) {
    return moveAnimalRelative(animal.id, dx, dy);
}

Grid& Zoo::getGrid() {
    return grid;
}

const Grid& Zoo::getGrid() const {
    return grid;
}

const std_plus::HashTable<int, std::shared_ptr<Animal>>& Zoo::getAnimals() const {
    return animals;
}