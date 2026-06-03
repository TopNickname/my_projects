#pragma once
#include "Vector2.h"
#include <string>
#include <random>

enum class Terrain {
	Land,
	Water
};

class Animal {
public:
	enum class Shape {
		circle, cube, triangle
	};

	enum class Color {
		red, green, blue, yellow, brown, purple, orange, cyan, black, white,
		pink, lime, teal, maroon, navy, olive, silver, gold
	};

	virtual ~Animal() = default;

	virtual std::string getSound() const = 0;
	virtual std::string species() const = 0;

	static char shapeToChar(Shape s);

	// --- Геттеры ---
	Vectori2 getPos()       const { return pos; }
	double   getHP()        const { return HP; }
	double   getSize()      const { return size; }
	double   getDamage()    const { return damage; }
	Shape    getShape()     const { return shape; }
	Color    getColor()     const { return color; }
	int      getID()        const { return id; }
	std::string getName()   const { return name; }

	// --- Сеттеры ---
	void setPos(Vectori2 p) { pos = p; }
	void setHP(double hp) { HP = hp; }
	void setSize(double s) { size = s; }
	void setDamage(double d) { damage = d; }
	void setShape(Shape s) { shape = s; }
	void setColor(Color c) { color = c; }
	void setName(const std::string& n) { name = n; }

	virtual bool can_traverse(Terrain terrain) const = 0;
	virtual void move(Vectori2 p);
	void tp(Vectori2 p);

	friend class Zoo;

protected:
	Animal() = default;

	double   size = 1.0;
	double   HP = 10.0;
	Vectori2 pos{ 0, 0 };
	Shape    shape = Shape::circle;
	Color    color = Color::black;
	double   damage = 0.0;
	int      id = 0;
	std::string name = "Unnamed";

	virtual void do_some_noise();
	virtual void attack();
};

// --- Абстрактные промежуточные классы ---
class Mammal : public Animal {
protected:
	Mammal() = default;
	double fluffiness = 0.5;
public:
	bool can_traverse(Terrain terrain) const override;
	virtual std::string furDescription() const = 0;
};

class Minecraft : public Animal {
protected:
	Minecraft() = default;
public:
	bool can_traverse(Terrain terrain) const override;
	virtual bool isHostile() const = 0;
};

class Bird : public Animal {
protected:
	Bird() = default;
public:
	bool can_traverse(Terrain terrain) const override;
	void fly();
	virtual int wingSpan() const = 0;
};

class Fish : public Animal {
protected:
	Fish() = default;
public:
	bool can_traverse(Terrain terrain) const override;
	void swim();
	virtual bool isFreshwater() const = 0;
};

// --- Конкретные виды ---
class Cat : public Mammal {
	bool is_killer_mode_on = false;
public:
	Cat();
	void switch_killer_mode();
	void switch_killer_mode(bool flag);

	std::string getSound() const override { return "Meow!"; }
	std::string species()   const override { return "Cat"; }
	std::string furDescription() const override { return "Fluffy orange fur"; }
};

class Bear : public Mammal {
public:
	Bear();
	std::string getSound() const override { return "Growl!"; }
	std::string species()   const override { return "Bear"; }
	std::string furDescription() const override { return "Thick brown fur"; }
};

class Hamster : public Mammal {
public:
	Hamster();
	void suddenly_die();
	std::string getSound() const override { return "Squeak!"; }
	std::string species()   const override { return "Hamster"; }
	std::string furDescription() const override { return "Soft golden fur"; }
};

class Shark : public Fish {
public:
	Shark();
	void do_some_buisness();
	std::string getSound() const override { return "Doing business..."; }
	std::string species()   const override { return "Shark"; }
	bool isFreshwater()     const override { return false; }
};

class Just_a_fish : public Fish {
public:
	Just_a_fish();
	std::string getSound() const override { return "Blub!"; }
	std::string species()   const override { return "Just a fish"; }
	bool isFreshwater()     const override { return true; }
};

class Duck : public Bird {
public:
	Duck();
	std::string getSound() const override { return "Quack!"; }
	std::string species()   const override { return "Duck"; }
	int wingSpan()          const override { return 60; }
};

class Creeper : public Minecraft {
public:
	Creeper();
	void explode();
	std::string getSound() const override { return "Sssss..."; }
	std::string species()   const override { return "Creeper"; }
	bool isHostile()        const override { return true; }
};