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

	// Ищет случайную свободную клетку (пытается до 100 раз)
	Vectori2 findFreeRandomPosition() const;

public:
	Zoo(int gridWidth, int gridHeight);

	// Теперь возвращает сырой указатель на созданное животное
	Animal* addAnimal(std::unique_ptr<Animal> animal);

	bool moveAnimal(int animalId, Vectori2 newPos);
	bool moveAnimalRelative(int animalId, int dx, int dy);

	bool moveAnimal(Animal& animal, Vectori2 newPos);
	bool moveAnimalRelative(Animal& animal, int dx, int dy);

	Grid& getGrid();
	const Grid& getGrid() const;

	const std_plus::HashTable<int, std::shared_ptr<Animal>>& getAnimals() const;
};