#pragma once
#include <vector>
#include "GameEntity.h"
#include "ResourceManager.h"

class Tile
{
protected:
public:
	enum class TileProp {
		ROAD, ROUGH, RAMP, WALL, HOLE
	};
	TileProp prop;
	float friction;
	int priority;
	inline Tile(TileProp type, float f, int p) :prop(type), friction(f), priority(p) {};
	inline Tile() :prop(TileProp::ROUGH), friction(.5f), priority(-1) {};
};

#define TILESCALE .0625f
class Map
{
private:
	GLuint texture;
	GLint numElements;
	const glm::vec3 scale = glm::vec3(4.0f, 4.0f, 4.0f);
	std::vector<std::vector<Tile>> data;
	std::vector<glm::vec3> startPositions;
	std::vector<glm::vec3> aiFlags;
public:
	inline void addRow() { data.push_back(std::vector<Tile>()); }
	inline void addTile(Tile t) { data[data.size() - 1].push_back(t); }
	void populateData(char * fileName);
	void render(Shader & shader, const glm::vec3 playerPosition, float scale);
	Map(GLint n, GLuint tex);
	~Map();


};