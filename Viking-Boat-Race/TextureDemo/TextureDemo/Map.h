#pragma once
#include <vector>
#include "GameEntity.h"
#include "ResourceManager.h"

class Tile : public GameEntity
{
protected:
public:
	int flag[2];
	glm::vec2 uvOffset;
	Tile(glm::vec3 &entityPos, glm::vec3 &entityScale, GLuint entityTexture, GLint entityNumElements, glm::vec2 uvOffset);
	virtual void update(double deltaTime) override;
	void SetUniforms(Shader & shader, glm::vec3 offset);
	~Tile();
};

#define TILESCALE .0625f
#define TILEWIDTH .05f
#define TILEHEIGHT .025f
class Map
{
private:
	std::vector<std::vector<Tile>> data;
public:
	inline void addRow() { data.push_back(std::vector<Tile>()); }
	inline void addTile(Tile t) { data[data.size() - 1].push_back(t); }
	void populateData(char * fileName, GLuint tex, GLint geom);
	void render(Shader & shader, glm::vec3 playerPosition);
	Map();
	~Map();
};

