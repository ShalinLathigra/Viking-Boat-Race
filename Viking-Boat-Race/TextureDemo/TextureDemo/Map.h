#pragma once
#include <vector>
#include "GameEntity.h"
#include "ResourceManager.h"
#include "Car.h"

class Wall {

private:
	glm::vec3 originPosition;
	glm::vec3 dimensions;
public:
	Wall(glm::vec3 pos, glm::vec3 dim);
	inline glm::vec3 getDimensions() { return dimensions; }
	inline glm::vec3 getOrigin() { return originPosition; }
};

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

#define COLLISION_DISTANCE .1f
#define RESTITUTION .95f
#define TILESCALE .0625f
class Map : public GameEntity
{
private:
	GLuint texture;
	GLint numElements;
	const glm::vec3 scale = glm::vec3(4.0f, 4.0f, 4.0f);
	std::vector<std::vector<Tile>> data;
	glm::vec3 topLeft;
	std::vector<Wall> walls;
	std::vector<glm::vec3> startPositions;
	std::vector<glm::vec3> aiFlags;
public:
	Map(glm::vec3 & entityPos, glm::vec3 & entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);
	~Map();

	void update(double deltaTime) override;
	void setPosition(glm::vec3 pos);

	inline void addRow() { data.push_back(std::vector<Tile>()); }
	inline void addTile(Tile t) { data[data.size() - 1].push_back(t); }
	void populateData(char * fileName);

	void calculateCarCollisions(Car * A);
};
