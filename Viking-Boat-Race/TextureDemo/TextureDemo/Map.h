#pragma once
#include <vector>
#include <algorithm>
#include <map>
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
	enum TileProp {
		ROAD, ROUGH, RAMP, WALL, HOLE, SLICK
	};
	TileProp prop;
	float friction;
	int priority;
	inline Tile(TileProp type, float f, int p) :prop(type), friction(f), priority(p) {};
	inline Tile() :prop(TileProp::ROUGH), friction(.5f), priority(-1) {};
};

#define COLLISION_DISTANCE .1f
#define RESTITUTION .95f
#define TILEWIDTH 18.0f / 64.0f
#define TILEHEIGHT 9.0f / 32.0f

class Map : public GameEntity
{
private:
	GLuint texture;
	GLint numElements;
	const glm::vec3 scale = glm::vec3(4.0f, 4.0f, 4.0f);
	std::vector<std::vector<Tile>> data;
	glm::vec3 topLeft;
	std::vector<Wall> walls;
	std::map<int, glm::vec3> startPositions;
	std::map<int, glm::vec3> aiFlags;

	std::vector<glm::vec3> lapFlags;
	glm::vec3 finalMarker;
	//const int numLaps = 3;
	const int numLaps = 1;
public:
	//Map &Map::operator=(const Map & aMap);
	Map(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, char * fileName, int idin);
	~Map();

	int id;

	void update(double deltaTime) override;
	void setPosition(glm::vec3 pos);

	inline void addRow() { data.push_back(std::vector<Tile>()); }
	inline void addTile(Tile t) { data[data.size() - 1].push_back(t); }
	void populateData(char * fileName);

	glm::vec3 getFlag(int i);
	inline glm::vec3 getMaxFlag() { return aiFlags.at(aiFlags.size() - 1); }
	int getMaxFlags();

	glm::vec3 getStartPosition(int i);
	glm::vec3 nearestFlag(glm::vec3 pos);

	//passing halfway, make the car's boolean true
	//passing finish, increment car's numLaps, boolean = false
	void checkProgress(Car * A);
	int checkFinish(Car * A);


	void calculateCarCollisions(Car * A);
	int getNumLaps();
	Tile::TileProp getPropertyUnder(Car * A);
};
