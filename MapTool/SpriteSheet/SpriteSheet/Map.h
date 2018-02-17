#pragma once
#include "Tile.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>

#define MAX_MAP_HEIGHT 64
#define MAX_MAP_WIDTH 64

class Map{
private:
	Tile tiles[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	std::vector<glm::vec3> startPoints;
public:
	inline Tile& getTileAt(int x, int y) { return tiles[y][x]; }
	Map();
	Map(char * filename);
	Map &operator=(Map &other);
	~Map();
};