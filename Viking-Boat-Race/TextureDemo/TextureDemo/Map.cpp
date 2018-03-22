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
		//std::cout << *iter;
	}
	//for (int i = 0; i < aiFlags.size(); i++) {
	//	std::cout << "FLAG: " << i << ": (" << aiFlags.at(i).x << ", " << aiFlags.at(i).y << ")" << std::endl;
	//}
	//for (int i = 0; i < startPositions.size(); i++) {
	//	std::cout << "START: " << i << ": (" << startPositions.at(i).x << ", " << startPositions.at(i).y << ")" << std::endl;
	//}
}
glm::vec3 Map::getFlag(int i) {
	if (i >= 0 && i < aiFlags.size()) {
		return aiFlags.at(i);
	}
}
glm::vec3 Map::getStartPosition(int i) {
	if (i >= 0 && i < startPositions.size()) {
		return startPositions.at(i);
	}
}
Tile::TileProp Map::getPropertyUnder(Car * A) 
{
	//64 wide
	//32 tall
	int col = (int)(64.0f * (A->getPosition().x + 9.0f) / 18.0f);		//x
	int row = (int)(-32.0f * (A->getPosition().y - 4.5f) / 9.0f);		//y
	//if (data[row][col].prop == Tile::TileProp::ROUGH) {
	//	std::cout << "Rough" << std::endl;
	//}
	//else if (data[row][col].prop == Tile::TileProp::ROAD) {
	//	std::cout << "Road" << std::endl;
	//}
	//else if (data[row][col].prop == Tile::TileProp::RAMP) {
	//	std::cout << "Ramp" << std::endl;
	//}
	//else if (data[row][col].prop == Tile::TileProp::WALL) {
	//	std::cout << "Wall" << std::endl;
	//}
	//else if (data[row][col].prop == Tile::TileProp::HOLE) {
	//	std::cout << "Hole" << std::endl;
	//}
	//else if (data[row][col].prop == Tile::TileProp::SLICK) {
	//	std::cout << "Slick" << std::endl;
	//}
	//std::cout << A->id << " ";
	//std::cout << data[0].size() << " " << col << " ";
	//std::cout << data.size() << " " << row << std::endl;
	if (row >= 0 && row < data.size() && col >= 0 && col < data[0].size()) {
		return data[row][col].prop;
	}
	return Tile::TileProp::WALL;
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
