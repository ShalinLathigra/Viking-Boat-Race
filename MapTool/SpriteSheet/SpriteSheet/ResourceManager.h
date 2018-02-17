#pragma once

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class ResourceManager {
public:
	static std::string LoadTextFile(const char *filename);
};