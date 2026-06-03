#pragma once
#include "Vector2.h"

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
        red, green, blue, yellow, brown, purple, orange, cyan, black, white
    };

    virtual std::string getSound() const { return "..."; }
    static char shapeToChar(Shape s);

    virtual ~Animal() = default;

    // --- Геттеры ---
    Vectori2 getPos()       const { return pos; }
    double   getHP()        const { return HP; }
    double   getSize()      const { return size; }
    double   getDamage()    const { return damage; }
    Shape    getShape()     const { return shape; }
    Color    getColor()     const { return color; }
    int      getID()        const { return id; }

    // --- Сеттеры ---
    void setPos(Vectori2 p) { pos = p; }
    void setHP(double hp) { HP = hp; }
    void setSize(double s) { size = s; }
    void setDamage(double d) { damage = d; }
    void setShape(Shape s) { shape = s; }
    void setColor(Color c) { color = c; }
    //void setID(int newId) { id = newId; }

    virtual bool can_traverse(Terrain terrain) const = 0;
    virtual void move(Vectori2 p);
    void tp(Vectori2 p);

    friend class Zoo;   // Прямой доступ к полям для Zoo

protected:
    double   size = 1.0;
    double   HP = 10.0;
    Vectori2 pos{ 0, 0 };
    Shape    shape = Shape::circle;
    Color    color = Color::black;
    double   damage = 0.0;
    int      id = 0;     // Уникальный идентификатор

    virtual void do_some_noise();
    virtual void attack();
};

class Mammal : public Animal {
protected:
    double fluffiness = 0.5;
public:
    bool can_traverse(Terrain terrain) const override;
};

class Minecraft : public Animal {
public:
    //Minecraft();
    bool can_traverse(Terrain terrain) const override;
};

class Bird : public Animal {
public:
    bool can_traverse(Terrain terrain) const override;
    void fly();
};

class Fish : public Animal {
public:
    bool can_traverse(Terrain terrain) const override;
    void swim();
};

// конкретные виды
class Cat : public Mammal {
    bool is_killer_mode_on = false;
public:
    void switch_killer_mode();
    void switch_killer_mode(bool flag);
    Cat();
    std::string getSound() const override { return "Meow!"; }

};

class Bear : public Mammal {
public:
    Bear() = default;
    std::string getSound() const override { return "Growl!"; }
};

class Hamster : public Mammal {
public:
    void suddenly_die();
    std::string getSound() const override { return "Squeak!"; }
};

class Shark : public Fish {
public:
    void do_some_buisness();
    std::string getSound() const override { return "Doing business..."; }
};

class Just_a_fish : public Fish {
public:
    std::string getSound() const override { return "Blub!"; }
};

class Duck : public Bird {

public:
    std::string getSound() const override { return "Quack!"; }
};

class Creeper : public Minecraft {
public:
    void explode();
    std::string getSound() const override { return "Sssss..."; }
};