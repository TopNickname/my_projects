#include "Animals.h"
#include <iostream>

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

bool Mammal::can_traverse(Terrain terrain) const {
    return terrain == Terrain::Land;
}
/*
Minecraft::Minecraft() {
    shape = Shape::cube;
    color = Color::green;
}
*/

bool Minecraft::can_traverse(Terrain terrain) const {
    return true;
}

bool Bird::can_traverse(Terrain terrain) const {
    return true;
}

void Bird::fly() {
    std::cout << "Bird flies\n";
}

bool Fish::can_traverse(Terrain terrain) const {
    return terrain == Terrain::Water;
}

void Fish::swim() {
    std::cout << "Fish swims\n";
}

Cat::Cat() {
    shape = Shape::circle;
    color = Color::orange;
    size = 1.2;
    fluffiness = 0.9;
}

void Cat::switch_killer_mode() {
    is_killer_mode_on = !is_killer_mode_on;
}

void Cat::switch_killer_mode(bool flag) {
    is_killer_mode_on = flag;
}

void Hamster::suddenly_die() {
    HP = 0.0;
    std::cout << "Hamster suddenly died\n";
}

void Shark::do_some_buisness() {
    std::cout << "Shark is doing business\n";
}

void Creeper::explode() {
    std::cout << "Creeper explodes!\n";
}