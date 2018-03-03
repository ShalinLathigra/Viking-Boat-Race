#include "Map.h"

Map::Map(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements) 
{
}

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
					std::cout << *iter << ": (" << startPositions[startPositions.size() - 1].x << ", " << startPositions[startPositions.size() - 1].y << ", " << startPositions[startPositions.size() - 1].z << ")" << std::endl;
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
Map::~Map()
{
}

int Map::CheckBounds(Car * c)
{
	if (c->getPosition().y >= MAP_HEIGHT + .1f || c->getPosition().y <= -MAP_HEIGHT - .1f) {
		c->ReflectY();
		c->SetPosition(glm::vec3(c->getPosition().x, (c->getPosition().y >= MAP_HEIGHT) ? MAP_HEIGHT + .05f : -MAP_HEIGHT - .05f, 0.0f));
		return 1;
	}
	if (c->getPosition().x >= MAP_WIDTH + .25f || c->getPosition().x <= -MAP_WIDTH - .3f) {
		c->ReflectX();
		c->SetPosition(glm::vec3((c->getPosition().x >= MAP_WIDTH) ? MAP_WIDTH + .245f : -MAP_WIDTH - .295f, c->getPosition().y, 0.0f));
		return 1;
	}
	return 0;
}

int Map::CheckWallCollisions(Car * c) {
	glm::vec3 carPos = c->getPosition();

	return 0;
}
