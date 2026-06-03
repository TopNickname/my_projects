#include "Animals.h"
#include <iostream>

// ------------------ Animal ------------------
void Animal::do_some_noise() {}
void Animal::attack() {}

void Animal::move(Vectori2 p) {
	pos = p;
}

void Animal::tp(Vectori2 p) {
	pos = p;
}

char Animal::shapeToChar(Shape s) {
	switch (s) {
	case Shape::circle:   return 'O';
	case Shape::cube:     return '#';
	case Shape::triangle: return '^';
	default:              return '?';
	}
}

// ------------------ Mammal ------------------
bool Mammal::can_traverse(Terrain terrain) const {
	return terrain == Terrain::Land;
}

// ------------------ Minecraft ------------------
bool Minecraft::can_traverse(Terrain terrain) const {
	return true;
}

// ------------------ Bird ------------------
bool Bird::can_traverse(Terrain terrain) const {
	return true;
}

void Bird::fly() {
	std::cout << "Bird flies\n";
}

// ------------------ Fish ------------------
bool Fish::can_traverse(Terrain terrain) const {
	return terrain == Terrain::Water;
}

void Fish::swim() {
	std::cout << "Fish swims\n";
}

// ============== Вспомогательная функция случайного цвета ==============
namespace {
	template<typename T>
	T randomFrom(const std::vector<T>& options) {
		static std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<size_t> dist(0, options.size() - 1);
		return options[dist(rng)];
	}
}

// ------------------ Cat ------------------
Cat::Cat() {
	shape = Shape::circle;
	color = randomFrom<Color>({
		Color::orange, Color::black, Color::white, Color::silver, Color::brown
		});
	size = 1.2;
	fluffiness = 0.9;
}

void Cat::switch_killer_mode() {
	is_killer_mode_on = !is_killer_mode_on;
}

void Cat::switch_killer_mode(bool flag) {
	is_killer_mode_on = flag;
}

// ------------------ Bear ------------------
Bear::Bear() {
	shape = Shape::circle;
	color = randomFrom<Color>({ Color::white, Color::black, Color::brown });
	size = 3.0;
	fluffiness = 0.7;
}

// ------------------ Hamster ------------------
Hamster::Hamster() {
	shape = Shape::circle;
	color = randomFrom<Color>({ Color::gold, Color::white, Color::brown });
	size = 0.5;
	fluffiness = 0.95;
}

void Hamster::suddenly_die() {
	HP = 0.0;
	std::cout << "Hamster suddenly died\n";
}

// ------------------ Shark ------------------
Shark::Shark() {
	shape = Shape::triangle;
	color = randomFrom<Color>({ Color::silver, Color::teal, Color::blue });
	size = 4.0;
	damage = 5.0;
}

void Shark::do_some_buisness() {
	std::cout << "Shark is doing business\n";
}

// ------------------ Just_a_fish ------------------
Just_a_fish::Just_a_fish() {
	shape = Shape::triangle;
	color = randomFrom<Color>({ Color::teal, Color::silver, Color::blue });
	size = 0.8;
}

// ------------------ Duck ------------------
Duck::Duck() {
	shape = Shape::circle;
	color = randomFrom<Color>({ Color::yellow, Color::white, Color::brown, Color::green });
	size = 1.0;
}

// ------------------ Creeper ------------------
Creeper::Creeper() {
	shape = Shape::cube;
	color = randomFrom<Color>({ Color::lime, Color::green });
	size = 1.8;
	damage = 20.0;
}

void Creeper::explode() {
	std::cout << "Creeper explodes!\n";
}