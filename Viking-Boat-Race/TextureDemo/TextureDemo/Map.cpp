#include "Map.h"

void Tile::SetUniforms(Shader & shader) 
{
	shader.setUniform2f("uvPosition", uvOffset);
}
Tile::Tile(glm::vec3 &entityPos, glm::vec3 &entityScale, GLuint entityTexture, GLint entityNumElements, glm::vec2 uvOffset)
	: GameEntity(entityPos, entityScale, 0.0f, entityTexture, entityNumElements), uvOffset(uvOffset)
{
}
void Tile::update(double deltaTime)
{
}
Tile::~Tile()
{
}



void Map::populateData(char * fileName, GLuint tex, GLint size) {
	std::string mapString = ResourceManager::LoadTextFile(fileName);
	glm::vec2 uvOffset;
	addRow();

	for (std::string::iterator iter = mapString.begin(); iter != mapString.end(); ++iter) {
		if (*iter == '\n') { addRow(); }
		else if (*iter == ' ') { addTile(Tile::Tile(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f), glm::vec3(TILESCALE, TILESCALE, TILESCALE), tex, size, glm::vec2(0.0f, 0.0f))); }
		else if (*iter == '1') { addTile(Tile::Tile(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f), glm::vec3(TILESCALE, TILESCALE, TILESCALE), tex, size, glm::vec2(1.0f, 0.0f))); }
	}
}
void Map::render(Shader & shader)
{
	for (std::vector<std::vector<Tile>>::iterator iter = data.begin(); iter != data.end(); ++iter) {
		for (std::vector<Tile>::iterator itr = iter->begin(); itr != iter->end(); itr++) {
			(itr)->SetUniforms(shader);
			(itr)->render(shader);
		}
	}
}
Map::Map()
{
}
Map::~Map()
{
}
