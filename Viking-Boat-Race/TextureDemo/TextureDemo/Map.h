#pragma once
#include <vector>
#include <map>
#include "GameEntity.h"
#include "Car.h"
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
#define MAP_WIDTH 4.0f
#define MAP_HEIGHT 2.0f
class Map : public GameEntity
{

private:
	GLuint texture;
	GLint numElements;
	const glm::vec3 scale = glm::vec3(4.0f, 4.0f, 4.0f);
	std::vector<std::vector<Tile>> data;
	std::map<int, glm::vec3> startPositions;
	std::map<int, glm::vec3> aiFlags;

#define PIXEL_SIZE .0357142857f

public:
	Map(glm::vec3 & entityPos, glm::vec3 & entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);
	~Map();

	int CheckBounds(Car * c);
	int CheckWallCollisions(Car * c);

	int getNextIndex(int current);
	inline glm::vec3 getFlag(int i){ return (i >= 0 && i < aiFlags.size() - 1) ? aiFlags[i] : (glm::vec3()); }
	inline std::map<int, glm::vec3> getFlags() { return aiFlags; }

	void update(double deltaTime) override;
	inline void SetPosition(glm::vec3 pos) { position = -1.0f * pos; }

	inline void addRow() { data.push_back(std::vector<Tile>()); }
	inline void addTile(Tile t) { data[data.size() - 1].push_back(t); }
	void populateData(char * fileName);
};