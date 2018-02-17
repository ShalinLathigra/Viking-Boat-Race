#include "Map.h"
#include "ResourceManager.h"

Map::Map()
{
}

Map::Map(char * filename)
{
	std::string mapString = ResourceManager::LoadTextFile(filename);
	std::vector<std::string> mapVector;
	while (mapString.length() != 0) {
		mapVector.push_back(mapString.substr(0, mapString.find('\n')));
		mapString = mapString.substr(mapString.find('\n') + 1, mapString.length());
	}
	int yCount = 0;
	int xCount = 0;
	for (std::vector<std::string>::iterator iter = mapVector.begin(); iter != mapVector.end() && yCount < MAX_MAP_HEIGHT; ++iter, yCount++) {
		for (int x = 0, xCount = 0; x < iter->length() && xCount < MAX_MAP_WIDTH; x++, xCount++) {
			Tile::TileProperty prop = Tile::TileProperty::WALL;
			int spawnID = -1;

			if ((*iter)[x] == 'U' || (*iter)[x] == 'u') {
				prop = Tile::TileProperty::RAMP;
			} 
			else if ((*iter)[x] == 'R' || (*iter)[x] == 'r') {
				prop = Tile::TileProperty::ROAD;
			} 
			else if ((*iter)[x] == 'V' || (*iter)[x] == 'v') {
				prop = Tile::TileProperty::ROUGH;
			} 
			else if ((*iter)[x] == 'H' || (*iter)[x] == 'h') {
				prop = Tile::TileProperty::HOLE;
			}
			else if ((*iter)[x] >= '1' && (*iter)[x] <= '9') {
				prop = Tile::TileProperty::ROAD;
				spawnID = ((*iter)[x] - '0');
			}
			else if ((*iter)[x] == '0') {
				prop = Tile::TileProperty::ROAD;
				float rot = 0;
				
				//alter rot based on map name, i don't really know yet

				startPoints.push_back(glm::vec3(xCount, yCount, rot));
			}
			tiles[yCount][x] = *new Tile(prop, spawnID);
		}
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			std::cout << tiles[j][i].getID();
		}
		std::cout << std::endl;
	}
}

Map & Map::operator=(Map & other)
{
	for (int i = 0; i < MAX_MAP_WIDTH; i++) {
		for (int j = 0; j < MAX_MAP_HEIGHT; j++) {
			tiles[j][i] = other.getTileAt(i, j);
		}
	}
	return *this;
}


Map::~Map()
{
}
