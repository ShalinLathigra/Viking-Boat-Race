#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

#define G .05f
#define m2 500.0f

class GameEntity {
public:
	GameEntity(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);
	float PI = 3.14159265358979;
	virtual void update(double deltaTime) = 0;
	void render(Shader &shader);
protected:
	glm::vec3 position, scale;
	float rotationAmount;//IN DEGREES

	GLuint texture;
	GLint numElements;
};