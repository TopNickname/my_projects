#pragma once
#include "Vector2.h"
#include "VectorN.h"
#include "Animals.h"

struct Cell {
    Terrain terrain = Terrain::Land;
};

class Grid {
    int width, height;
    std_plus::VectorN<std_plus::VectorN<Cell>> cells;

public:
    Grid(int w, int h);

    int getWidth() const;
    int getHeight() const;

    void setTerrain(Vectori2 pos, Terrain t);
    Terrain getTerrain(Vectori2 pos) const;

    bool canAnimalMoveTo(const Animal& animal, Vectori2 pos) const;
};