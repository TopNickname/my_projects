#include "Zoo.h"
#include <random>
#include <chrono>

Zoo::Zoo(int gridWidth, int gridHeight)
	: grid(gridWidth, gridHeight)
{
}

Vectori2 Zoo::findFreeRandomPosition() const {
	int w = grid.getWidth();
	int h = grid.getHeight();
	std::mt19937 gen(static_cast<unsigned>(
		std::chrono::steady_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distX(0, w - 1);
	std::uniform_int_distribution<int> distY(0, h - 1);

	for (int attempt = 0; attempt < 100; ++attempt) {
		int x = distX(gen);
		int y = distY(gen);
		Vectori2 pos(x, y);
		bool occupied = false;
		for (const auto& pair : animals) {
			if (pair.second->getPos().x() == x && pair.second->getPos().y() == y) {
				occupied = true;
				break;
			}
		}
		if (!occupied)
			return pos;
	}
	return Vectori2(0, 0);
}

Vectori2 Zoo::getRandomFreePosition() const {
	return findFreeRandomPosition();
}

Animal* Zoo::addAnimal(std::unique_ptr<Animal> animal) {
	if (!animal) return nullptr;

	int id = nextId++;
	animal->id = id;

	// Если позиция не задана (осталась (0,0)), ищем свободную клетку
	if (animal->getPos().x() == 0 && animal->getPos().y() == 0) {
		animal->setPos(findFreeRandomPosition());
	}

	std::shared_ptr<Animal> shared = std::move(animal);
	animals.add(id, shared);
	return shared.get();
}

void Zoo::removeAnimal(int id) {
	animals.erase(id);
}

bool Zoo::moveAnimal(int animalId, Vectori2 newPos) {
	if (!animals.contains(animalId))
		return false;

	Animal& animal = *animals.at(animalId);
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