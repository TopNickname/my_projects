#include "Grid.h"
#include <stdexcept>

Grid::Grid(int w, int h)
    : width(w), height(h)
{
    if (w <= 0 || h <= 0)
        throw std::invalid_argument("Grid dimensions must be positive");
    cells = std_plus::VectorN<std_plus::VectorN<Cell>>(h,
        std_plus::VectorN<Cell>(w));
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }

void Grid::setTerrain(Vectori2 pos, Terrain t) {
    int x = pos.x();
    int y = pos.y();
    if (x >= 0 && x < width && y >= 0 && y < height)
        cells[y][x].terrain = t;
}

Terrain Grid::getTerrain(Vectori2 pos) const {
    int x = pos.x();
    int y = pos.y();
    if (x >= 0 && x < width && y >= 0 && y < height)
        return cells[y][x].terrain;
    return Terrain::Land;
}

bool Grid::canAnimalMoveTo(const Animal& animal, Vectori2 pos) const {
    int x = pos.x();
    int y = pos.y();
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    return animal.can_traverse(cells[y][x].terrain);
}