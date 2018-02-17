#pragma once

class Tile{

public:
	enum class TileProperty {
		RAMP, ROAD, ROUGH, HOLE, WALL
	};


private:
	float friction;
	TileProperty prop;
	int itemSpawnID;
	int item;
	float itemMaxCD = 2.5f;
	float itemCurrentCD;
public:
	Tile();
	Tile(TileProperty tileProp, int id);
	Tile &operator=(Tile &other);
	~Tile();
	inline float getFriction() { return friction; }
	inline TileProperty getProperty() { return prop; }
	inline int getID() { return itemSpawnID; }
	inline int getItem() { return item; }
	inline float getCD() { return itemCurrentCD; }
};
