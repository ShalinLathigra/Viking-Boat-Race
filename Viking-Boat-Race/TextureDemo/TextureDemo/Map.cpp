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
					aiFlags.push_back(glm::vec3((float)(-9.0f + data[data.size() - 1].size() - 1.0f) * TILEWIDTH, (float)(4.5f - data.size() - 1.0f) * TILEHEIGHT, 0.0f));		//flags == lowercase alpha
				}
				else {
					startPositions.push_back(glm::vec3((float)(-9.0f + data[data.size() - 1].size() - 1.0f) * TILEWIDTH, (float)(4.5f - data.size() - 1.0f) * TILEHEIGHT, 0.0f));	//starts == uppercase alpha
				}
			}
		}
		else if (*iter == '4') {
			addTile(Tile::Tile(Tile::TileProp::HOLE, 1.0f, -2));
		}
		else if (*iter == '5') {
			addTile(Tile::Tile(Tile::TileProp::RAMP, 1.0f, -2));
		}
		else if (*iter == '6') {
			addTile(Tile::Tile(Tile::TileProp::SLICK, 1.0f, -2));
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
	if (A->getPosition().x < minX + 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(minX + 4.0f * COLLISION_DISTANCE, A->getPosition().y, 0.0f)); A->applyImpulse(1.95f * glm::vec3(-A->getVel().x, A->getVel().y, 0.0f));}
	if (A->getPosition().x > maxX - 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(maxX - 4.0f * COLLISION_DISTANCE, A->getPosition().y, 0.0f)); A->applyImpulse(1.95f * glm::vec3(-A->getVel().x, A->getVel().y, 0.0f));}
	if (A->getPosition().y < minY + 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(A->getPosition().x, minY + 4.0f * COLLISION_DISTANCE, 0.0f)); A->applyImpulse(1.95f * glm::vec3(A->getVel().x, -A->getVel().y, 0.0f));}
	if (A->getPosition().y > maxY - 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(A->getPosition().x, maxY - 4.0f * COLLISION_DISTANCE, 0.0f)); A->applyImpulse(1.95f * glm::vec3(A->getVel().x, -A->getVel().y, 0.0f));}

	//Colliding with walls
	for (std::vector<Wall>::iterator wall = walls.begin(); wall != walls.end(); ++wall) 
	{
		//Normal is a line directly to the ball, normalized. 
		//need to find out what edge I am closest to, then use that to find the normal of the collision
		
		glm::vec3 normal = glm::vec3();
		float distToLeft = abs(wall->getOrigin().x - A->getPosition().x);
		float distToRight = abs((wall->getOrigin().x + wall->getDimensions().x) - A->getPosition().x);
		float distToTop = abs(wall->getOrigin().y - A->getPosition().y);;
		float distToBot = abs((wall->getOrigin().y - wall->getDimensions().y) - A->getPosition().y);

		float minimumDist = std::min(std::min(distToLeft, distToRight), std::min(distToBot, distToTop));

		int xWithinRange = wall->getOrigin().x < A->getPosition().x && A->getPosition().x < wall->getOrigin().x + wall->getDimensions().x;
		int yWithinRange = wall->getOrigin().y > A->getPosition().y && A->getPosition().y > wall->getOrigin().y - wall->getDimensions().y;

		//std::cout << "TOP: " << distToTop << ", BOT: " << distToBot << std::endl;

		if (minimumDist < COLLISION_DISTANCE) 
		{
			if (yWithinRange) 
			{
				if (minimumDist == distToLeft)
				{
					normal = glm::vec3(-1.0f, 0.0f, 0.0f);
					A->setPosition(glm::vec3(wall->getOrigin().x - COLLISION_DISTANCE, A->getPosition().y, 0.0f));
				}
				else if (minimumDist == distToRight)
				{
					normal = glm::vec3(1.0f, 0.0f, 0.0f);
					A->setPosition(glm::vec3(wall->getOrigin().x + wall->getDimensions().x + COLLISION_DISTANCE, A->getPosition().y, 0.0f));
				}
			}
			if (xWithinRange) 
			{
				if (minimumDist == distToTop)
				{
					normal = glm::vec3(0.0f, 1.0f, 0.0f);
					A->setPosition(glm::vec3(A->getPosition().x, wall->getOrigin().y + COLLISION_DISTANCE, 0.0f));
				}
				else if (minimumDist == distToBot)
				{
					normal = glm::vec3(0.0f, -1.0f, 0.0f);
					A->setPosition(glm::vec3(A->getPosition().x, wall->getOrigin().y - wall->getDimensions().y - COLLISION_DISTANCE, 0.0f));
				}
			}
		}
	}
}
Tile::TileProp Map::getPropertyUnder(Car * A) 
{
	//64 wide
	//32 tall
	float col = (A->getPosition().x + 9.0f) / TILEWIDTH;		//x
	//coord = -9.0f + scale * x
	//x = (coord + 9.0f) / scale
	float row = -(A->getPosition().y - 4.5f) / TILEHEIGHT;		//y
	//coord = 4.5f - scale * x
	//scale = -(coord - 4.5f) / scale
	//std::cout << col << " " << row << std::endl;

	//if (row < data.size() && col< data[0].size()) {
	//	return data[row][col].prop;
	//}
	return Tile::TileProp::HOLE;
}
Map::~Map()
{
}
Map::Map(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements), topLeft(glm::vec3(-9.0f+1.0f/32.0f, 4.5f+1.0f/32.0f, 0.0f))
{
	float yScale = 9.0f / 32.0f;
	float xScale = 18.0f / 64.0f;

	walls.push_back(Wall::Wall(glm::vec3(-9.0f + xScale * 11.0f, 4.5f - yScale * 11.5f, 0.0f), glm::vec3(xScale * 42.0f, yScale * 10.0f, 0.0f)));
}

Wall::Wall(glm::vec3 pos, glm::vec3 dim)
	: originPosition(pos), dimensions(dim)
{
}
