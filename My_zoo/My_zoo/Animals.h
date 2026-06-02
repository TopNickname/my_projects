#pragma once
#include "Vector2.h"
#include <random>
#pragma region Animal



class Animal {
protected:
	static enum class Shape {
		circle, cube, triangle
	};
	static enum class Color {
		red,green,blue,yellow,brown,purple,orange,cyan,black,white
	};
	double size;
	double HP;
	Vector2 pos;
	Shape shape = Shape::circle;
	double damage;
	virtual void do_some_noise();
	virtual void attack();

public:
	virtual void move(Vector2);
	void tp(Vector2);
};

#pragma endregion

#pragma region classification

class Mammal : public Animal {
	double fluffiness;

public:

};

class Minecraft : public Animal {
	Shape shape = Shape::cube;
};

class  {

public:
	void do_some_usless_things();
};

class Fish {
	
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

class Shark {
	
public:
	void do_some_buisness();
};

class Just_a_fish {

};

#pragma endregion