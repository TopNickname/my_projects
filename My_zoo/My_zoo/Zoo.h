// Zoo.h
#pragma once
#include "VectorN.h"
#include "Grid.h"
#include "Animals.h"
#include "Hash.h"   
#include <memory>

class Zoo {
    Grid grid;
    std_plus::HashTable<int, std::shared_ptr<Animal>> animals;
    int nextId = 1;

public:
    Zoo(int gridWidth, int gridHeight);

    // Добавление животного: ему присваивается уникальный id, который становится ключом в таблице.
    void addAnimal(std::unique_ptr<Animal> animal);

    // Перемещение по id
    bool moveAnimal(int animalId, Vectori2 newPos);
    bool moveAnimalRelative(int animalId, int dx, int dy);

    // Перемещение по ссылке (использует поле id объекта)
    bool moveAnimal(Animal& animal, Vectori2 newPos);
    bool moveAnimalRelative(Animal& animal, int dx, int dy);

    Grid& getGrid();
    const Grid& getGrid() const;

    // Доступ ко всей таблице животных
    const std_plus::HashTable<int, std::shared_ptr<Animal>>& getAnimals() const;
};