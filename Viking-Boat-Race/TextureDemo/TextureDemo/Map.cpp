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

	for (std::string::iterator iter = mapString.begin(); iter != mapString.end(); ++iter) 
	{
		if (*iter == '\n') 
		{
			addRow(); 
		}
		else if (*iter == ' ') 
		{ 
			addTile(Tile::Tile()); 
		}
		else if (*iter - '0' >= 1 && *iter - '0' <= 3 || isalpha(*iter)) 
		{
			addTile(Tile::Tile(Tile::TileProp::ROAD, .1f, 0));

			if (isalpha(*iter)) {
				float x = -9.0f + (data[data.size() - 1.0f].size() - 1.0f) / 64.0f * 18.0f;
				float y = 4.5f - (data.size() - 1.0f) / 32.0f * 9.0f;

				if (*iter >= 97) {
					aiFlags.insert(std::pair<int, glm::vec3>(*iter - 'a', glm::vec3(x, y, 0.0f)));		//flags == lowercase alpha
				}
				else {
					startPositions.insert(std::pair<int, glm::vec3>(*iter - 'A', glm::vec3(x, y, 0.0f)));	//starts == uppercase alpha
				}
			}
		}
		else if (*iter == '4') 
		{
			addTile(Tile::Tile(Tile::TileProp::HOLE, 1.0f, -2));
		}
		else if (*iter == '5') 
		{
			addTile(Tile::Tile(Tile::TileProp::RAMP, 1.0f, -2));
		}
		else if (*iter == '6') 
		{
			addTile(Tile::Tile(Tile::TileProp::SLICK, 1.0f, -2));
		}
		else if (*iter == '|' || *iter == '-') 
		{
			addTile(Tile::Tile(Tile::TileProp::RAMP, -.5f, 2));
		}
		else if (*iter == '#') 
		{
			addTile(Tile::Tile(Tile::TileProp::WALL, -.5f, 2));
		}
	}
}

//return the position, if it exists;
glm::vec3 Map::getFlag(int i) {
	if (i >= 0 && i < aiFlags.size()) {
		return aiFlags.at(i);
	}
}
//return the position, if it exists;
glm::vec3 Map::getStartPosition(int i) {
	if (i >= 0 && i < startPositions.size()) {
		return startPositions.at(i);
	}
}
Tile::TileProp Map::getPropertyUnder(Car * A) 
{
	//64 wide
	//32 tall
	Tile::TileProp ret = Tile::TileProp::WALL;
	int col = (int)(64.0f * (A->getPosition().x + 9.0f) / 18.0f);		//x
	int row = (int)(-32.0f * (A->getPosition().y - 4.5f) / 9.0f);		//y

	if (row >= 0 && row < data.size() && col >= 0 && col < data[0].size()) 
	{
		ret = data[row][col].prop;
	}
	if (!A->isJumping()) 
	{
		//set the car's speed mod
		A->speedMod = (ret == Tile::TileProp::ROAD) ? 1.0f : (ret == Tile::TileProp::RAMP) ? 2.5f : (ret == Tile::TileProp::SLICK) ? 1.5f : (ret == Tile::TileProp::HOLE) ? .1f : .5f;

		//if on ramp, start the jump
		if (ret == Tile::TileProp::RAMP) 
		{
			A->startJump();
		}
		//if in a hole, start fall
		else if (ret == Tile::TileProp::HOLE) 
		{
			A->startFall();
		}
		A->rotationSpeed = (ret == Tile::TileProp::SLICK) ? -1.5f : A->rotationSpeed;

		//reset falling if necessary
		if (A->isFalling() && A->doneFalling())
		{
			A->resetFall(nearestFlag(A->getPosition()));
		}
	}
	return ret;
}
glm::vec3 Map::nearestFlag(glm::vec3 pos) 
{
	float nearestDist = 100000.0f;
	float currentDist;
	glm::vec3 nearestFlag = aiFlags.at(0);
	for (int i = 0; i < aiFlags.size(); i++) 
	{
		currentDist = glm::distance(pos, aiFlags.at(i));
		if ( currentDist < nearestDist) {
			nearestDist = currentDist;
			nearestFlag = aiFlags.at(i);
		}
	}
	return nearestFlag;
}

//Car collisions with walls
void Map::calculateCarCollisions(Car * A)
{
	float minX = topLeft.x;
	float maxX = -minX;
	float minY = -topLeft.y;
	float maxY = -minY;

	//Bounding Position
	if (A->getPosition().x < minX + 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(minX + 4.0f * COLLISION_DISTANCE, A->getPosition().y, 0.0f));}
	if (A->getPosition().x > maxX - 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(maxX - 4.0f * COLLISION_DISTANCE, A->getPosition().y, 0.0f));}
	if (A->getPosition().y < minY + 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(A->getPosition().x, minY + 4.0f * COLLISION_DISTANCE, 0.0f));}
	if (A->getPosition().y > maxY - 4.0f * COLLISION_DISTANCE) { A->setPosition(glm::vec3(A->getPosition().x, maxY - 4.0f * COLLISION_DISTANCE, 0.0f));}

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


Map::~Map()
{
}
Map::Map(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements), topLeft(glm::vec3(-9.0f+1.0f/32.0f, 4.5f+1.0f/32.0f, 0.0f))
{
	float yScale = 9.0f / 32.0f;
	float xScale = 18.0f / 64.0f;

	walls.push_back(Wall::Wall(glm::vec3(-9.0f + xScale * 11.0f, 4.5f - yScale * 11.5f, 0.0f), glm::vec3(xScale * 42.0f, yScale * 10.0f, 0.0f)));
	populateData("map.txt");
}

Wall::Wall(glm::vec3 pos, glm::vec3 dim)
	: originPosition(pos), dimensions(dim)
{
}
