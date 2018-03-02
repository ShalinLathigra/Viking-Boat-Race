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
class Map : public GameEntity
{
private:
	GLuint texture;
	GLint numElements;
	const glm::vec3 scale = glm::vec3(4.0f, 4.0f, 4.0f);
	std::vector<std::vector<Tile>> data;
	std::vector<glm::vec3> startPositions;
	std::vector<glm::vec3> aiFlags;
public:
	Map(glm::vec3 & entityPos, glm::vec3 & entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);
	~Map();

	void update(double deltaTime) override;
	void SetPosition(glm::vec3 pos);


	inline void addRow() { data.push_back(std::vector<Tile>()); }
	inline void addTile(Tile t) { data[data.size() - 1].push_back(t); }
	void populateData(char * fileName);


};