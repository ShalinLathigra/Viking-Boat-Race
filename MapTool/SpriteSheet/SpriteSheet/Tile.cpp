#include "Tile.h"



Tile::Tile():
	friction(-1.0f), prop(TileProperty::ROAD), itemSpawnID(-1), item(-1)
{
}
Tile::Tile(TileProperty tileProp, int id):
	prop(tileProp), itemSpawnID(id)
{
	switch (tileProp) {
	case(TileProperty::RAMP):friction = -10.0f; break;		//assuming that speed is reduced by deltaTime * friction
	case(TileProperty::ROAD):friction = 1.0f; break;
	case(TileProperty::ROUGH):friction = 50.0f; break;
	case(TileProperty::HOLE):friction = 1000.0f; break;
	}
}
Tile & Tile::operator=(Tile & other)
{
	friction = other.getFriction();
	prop = other.getProperty();
	itemSpawnID = other.getID();
	item = other.getItem();
	itemCurrentCD = other.getCD();

	return *this;
}


Tile::~Tile()
{
}
