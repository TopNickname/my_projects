#include "Game.h"
#include <memory>

int main() {
	Game game(20, 10);

	// Стартовый зоопарк
	game.addAnimal(std::make_unique<Cat>());
	game.addAnimal(std::make_unique<Bear>());
	game.addAnimal(std::make_unique<Hamster>());
	game.addAnimal(std::make_unique<Duck>());
	game.addAnimal(std::make_unique<Shark>());
	game.addAnimal(std::make_unique<Just_a_fish>());
	game.addAnimal(std::make_unique<Creeper>());

	game.run();
	return 0;
}