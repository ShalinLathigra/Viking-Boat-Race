#include "GameEntity.h"

GameEntity::GameEntity(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	: position(entityPos), scale(entityScale), rotationAmount(entityRotationAmount), texture(entityTexture), numElements(entityNumElements)
{
}

GameEntity::GameEntity(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLint entityNumElements)
	: position(entityPos), scale(entityScale), rotationAmount(entityRotationAmount), numElements(entityNumElements) {}

GameEntity::GameEntity(GLint entityTexture, GLint entityNumElements):texture(entityTexture),numElements(entityNumElements) {};

void GameEntity::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader 
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAmount, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	shader.setUniformMat4("x", transformationMatrix);
	transform = transformationMatrix;
	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

//Exclusively used to draw the turrets on the shop
void GameEntity::render(Shader &shader, glm::mat4 transformationMatrix) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -10));
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAmount, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f,0.6,0.6));
	transformationMatrix = transformationMatrix* translate *rotationMatrix *scaleMatrix;
	shader.setUniformMat4("x", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}