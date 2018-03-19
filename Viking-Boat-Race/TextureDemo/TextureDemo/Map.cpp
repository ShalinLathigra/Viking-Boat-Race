#include "Map.h"

Map::Map(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements) 
{
}

void Map::SetPosition(glm::vec3 pos) { position = -1.0f * pos; }
void Map::update(double deltaTime)
{
}

void Map::populateData(char * fileName) {
	std::string mapString = ResourceManager::LoadTextFile(fileName);
	glm::vec2 uvOffset;
	addRow();
	int index = 0;

	for (std::string::iterator iter = mapString.begin(); iter != mapString.end(); ++iter) {
		if (*iter == '\n') { addRow(); }
		else if (*iter == ' ') { addTile(Tile::Tile()); }
		else if (*iter - '0' >= 1 && *iter - '0' <= 3 || isalpha(*iter)) {
			addTile(Tile::Tile(Tile::TileProp::ROAD, .1f, 0));

			if (isalpha(*iter)) {
				if (*iter >= 97) {
					aiFlags.push_back(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f));
				}
				else {
					startPositions.push_back(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f));
				}
			}
		}
		else if (*iter == '#') {
			addTile(Tile::Tile(Tile::TileProp::WALL, 1.0f, -2));
		}
		else if (*iter == '|' || *iter == '-') {
			addTile(Tile::Tile(Tile::TileProp::RAMP, -.5f, 2));
		}
	}
}
void Map::CalculateCarCollisions(Car * A)
{
}
Map::~Map()
{
}

Wall::Wall(glm::vec3 pos, glm::vec3 dim)
	: topLeftPosition(pos), dimensions(dim)
{
}
