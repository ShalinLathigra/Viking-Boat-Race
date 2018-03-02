#include "Map.h"

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
void Map::render(Shader & shader, const glm::vec3 playerPosition, float sin)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader 
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -playerPosition);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(sin, sin, sin));
	glm::mat4 transformationMatrix = translationMatrix * scaleMatrix;
	shader.setUniformMat4("x", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
Map::Map(GLint n, GLuint tex) : numElements(n), texture(tex)
{
	startPositions = std::vector<glm::vec3>();
}
Map::~Map()
{
}
