#include "Map.h"

void Tile::SetUniforms(Shader & shader, glm::vec3 offset) 
{
	shader.setUniform2f("uvPosition", uvOffset);
	
	shader.setUniform2f("offset", glm::vec2(offset.x, -offset.y));

	shader.setUniformMat4("x",glm::mat4(1.0f));
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
	int index = 0;

	for (std::string::iterator iter = mapString.begin(); iter != mapString.end(); ++iter) {
		if (*iter == '\n') { addRow(); }
		else if (*iter == ' ') { addTile(Tile::Tile(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f), glm::vec3(TILESCALE, TILESCALE, TILESCALE), tex, size, glm::vec2(0.0f, 0.0f))); }
		else if (*iter - '0' >= 1 && *iter - '0' <= 3 || isalpha(*iter)) {
			addTile(Tile::Tile(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f), glm::vec3(TILESCALE, TILESCALE, TILESCALE), tex, size, glm::vec2(1.0f, 0.0f))); 
			data[data.size() - 1][data[data.size() - 1].size() - 1].flag[0] = -1;
			data[data.size() - 1][data[data.size() - 1].size() - 1].flag[1] = -1;

			if (isalpha(*iter)) {
				data[data.size() - 1][data[data.size() - 1].size() - 1].flag[0] = 1;
				if (*iter >= 97) {
					data[data.size() - 1][data[data.size() - 1].size() - 1].flag[1] = *iter - 'a';
				}
				else {
					data[data.size() - 1][data[data.size() - 1].size() - 1].flag[1] = *iter - 'A' + 26;
				}
			}
		}
		else if (*iter == '#') {
			addTile(Tile::Tile(glm::vec3((float)(data[data.size() - 1].size() - 1.0f) * TILESCALE, -(float)(data.size() - 1.0f) * TILESCALE, 0.0f), glm::vec3(TILESCALE, TILESCALE, TILESCALE), tex, size, glm::vec2(3.0f, 0.0f)));
			data[data.size() - 1][data[data.size() - 1].size() - 1].flag[0] = 1;
			data[data.size() - 1][data[data.size() - 1].size() - 1].flag[1] = -1;
		}
	}
}
void Map::render(Shader & shader, glm::vec3 playerPosition)
{
	for (std::vector<std::vector<Tile>>::iterator iter = data.begin(); iter != data.end(); ++iter) {
		for (std::vector<Tile>::iterator itr = iter->begin(); itr != iter->end(); itr++) {
			(itr)->SetUniforms(shader, playerPosition);
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
