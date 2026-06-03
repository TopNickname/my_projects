#include "Game.h"
#include <memory>

int main() {
	Game game(20, 10);

	// Создаём животных с именами
	auto cat = std::make_unique<Cat>();
	cat->setName("Whiskers");
	game.addAnimal(std::move(cat));

	auto bear = std::make_unique<Bear>();
	bear->setName("Baloo");
	game.addAnimal(std::move(bear));

	auto hamster = std::make_unique<Hamster>();
	hamster->setName("Peanut");
	game.addAnimal(std::move(hamster));

	auto duck = std::make_unique<Duck>();
	duck->setName("Daffy");
	game.addAnimal(std::move(duck));

	auto shark = std::make_unique<Shark>();
	shark->setName("Bruce");
	game.addAnimal(std::move(shark));

	auto fish = std::make_unique<Just_a_fish>();
	fish->setName("Nemo");
	game.addAnimal(std::move(fish));

	auto creeper = std::make_unique<Creeper>();
	creeper->setName("Boomer");
	game.addAnimal(std::move(creeper));

	game.run();
	return 0;
}