#pragma once
#include "Vector2.h"
#include <random>

#pragma region Animal

class Animal {
protected:
	enum class Shape {
		circle, cube, triangle
	};
	enum class Color {
		red,green,blue,yellow,brown,purple,orange,cyan,black,white
	};
	double size=1.0;
	double HP=10.0;
	Vector2 pos{ 0.0,0.0 };
	Shape shape = Shape::circle;
	Color color = Color::black;
	double damage;
	virtual void do_some_noise();
	virtual void attack();

public:
	virtual void move(Vector2 p);
	void tp(Vector2 p);
	virtual ~Animal() = default;
};

#pragma endregion

#pragma region classification

class Mammal : public Animal {
	double fluffiness;

public:

};

class Minecraft : public Animal {
	
public:
	Minecraft() {
		shape = Shape::cube;
		color = Color::green;
	}
};

class Bird :public Animal {

public:
	void fly();
};

class Fish : public Animal {
	
public:
	void swim();
};


#pragma endregion

#pragma region species


class Cat : public Mammal{
	bool is_killer_mode_on = false;
	
public:
	void switch_killer_mode();
	void switch_killer_mode(bool flag);
	Cat();
};

class Bear : public Mammal {
	

public:
	
};

class Hamster : public Mammal {
	
public:
	void suddenly_die();
};

class Shark : public Fish {
	
public:
	void do_some_buisness();
};

class Just_a_fish : public Fish {

};

class Duck : public Bird {

};

class Creeper : public Minecraft {

public:
	void explode() {

	}
};

#pragma endregion