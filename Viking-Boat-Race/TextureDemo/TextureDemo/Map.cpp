#include "Map.h"


void Map::setPosition(glm::vec3 pos) { position = -1.0f * pos; }
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
void Map::calculateCarCollisions(Car * A)
{
	float minX = topLeft.x;
	float maxX = -minX;
	float minY = -topLeft.y;
	float maxY = -minY;

	//Bounding Position
	if (A->getPosition().x < minX + 4.0f * COLLISION_DISTANCE) { /*std::cout << "A" << std::endl; */A->setPosition(glm::vec3(minX + 4.0f * COLLISION_DISTANCE, A->getPosition().y, 0.0f)); A->applyImpulse(1.95f * glm::vec3(-A->getVel().x, A->getVel().y, 0.0f));}
	if (A->getPosition().x > maxX - 4.0f * COLLISION_DISTANCE) { /*std::cout << "B" << std::endl; */A->setPosition(glm::vec3(maxX - 4.0f * COLLISION_DISTANCE, A->getPosition().y, 0.0f)); A->applyImpulse(1.95f * glm::vec3(-A->getVel().x, A->getVel().y, 0.0f));}
	if (A->getPosition().y < minY + 4.0f * COLLISION_DISTANCE) { /*std::cout << "C" << std::endl; */A->setPosition(glm::vec3(A->getPosition().x, minY + 4.0f * COLLISION_DISTANCE, 0.0f)); A->applyImpulse(1.95f * glm::vec3(A->getVel().x, -A->getVel().y, 0.0f));}
	if (A->getPosition().y > maxY - 4.0f * COLLISION_DISTANCE) { /*std::cout << "D" << std::endl; */A->setPosition(glm::vec3(A->getPosition().x, maxY - 4.0f * COLLISION_DISTANCE, 0.0f)); A->applyImpulse(1.95f * glm::vec3(A->getVel().x, -A->getVel().y, 0.0f));}

	//Colliding with walls
	for (std::vector<Wall>::iterator iter = walls.begin(); iter != walls.end(); iter++) 
	{
		//			_________________________________________________________________
		//			|																|
		//			|																|
		//			|																|
		//			|																|
		//			|																|
		//			|																|
		//			|																|
		//			|																|
		//			|																|
		//			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
	}
}
Map::~Map()
{
}
Map::Map(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements), topLeft(glm::vec3(-9.0f+1.0f/32.0f, 4.5f+1.0f/32.0f, 0.0f))
{
	float dim = 1.0f / 32.0f;

	walls.push_back(Wall::Wall(glm::vec3(dim * 11.0f, dim * 12.0f, 0.0f), glm::vec3(dim * 41.0f, dim * 10.0f, 0.0f)));
}

Wall::Wall(glm::vec3 pos, glm::vec3 dim)
	: originPosition(pos), dimensions(dim)
{
}
