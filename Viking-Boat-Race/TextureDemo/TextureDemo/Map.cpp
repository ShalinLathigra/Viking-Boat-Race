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
					aiFlags[*iter - 97] = glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f);
					std::cout << *iter << ": (" << aiFlags[*iter - 97].x << ", " << aiFlags[*iter - 97].y << ")" << std::endl;
				}
				else {
					startPositions[*iter - 45] = glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f);
					std::cout << *iter << ": (" << startPositions[*iter - 45].x << ", " << startPositions[*iter - 45].y << ")" << std::endl;
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
	glm::vec3 carVel = glm::vec3(c->speed * cos(c->getAngle()), c->speed * sin(c->getAngle()), 0.0f);

	carPos += glm::vec3(4.0f, 2.0f, 0.0f);

	//need to add to carPos, 1/2 of the total size of the map
	//then multiply that by PIXEL_SIZE and cast to int to get the central position of the car

	return 0;
}

int Map::getNextIndex(int index) {
	if (index == aiFlags.size() - 1) {
		return 0;
	} 
	return ++index;
}