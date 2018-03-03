#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

#define G .05f
#define m2 500.0f
#define PI 3.14159265358979

class GameEntity {
public:
	GameEntity(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);

	virtual void update(double deltaTime) = 0;
	void render(Shader &shader);
	inline float getAngle() { return rotationAmount; }
	inline void SetPosition(glm::vec3 p) { position = p; }
	inline const glm::vec3 getPosition() { return position; }
protected:
	glm::vec3 position, scale;
	float rotationAmount;//IN DEGREES

	GLuint texture;
	GLint numElements;
};