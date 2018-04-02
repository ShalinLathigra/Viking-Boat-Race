#ifndef ARROW_H
#define ARROW_H
#pragma once
#include <vector>
#include "GameEntity.h"
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "Shader.h"
#include <SOIL/SOIL.h>
#include <chrono>
#include <thread>
#include "ResourceManager.h"
#include "Car.h"
class Arrow :public GameEntity
{
public:
	Arrow(glm::vec3 pos, glm::vec3 scale, float rot, GLint entityNumElements, float spd, int dmg, float x, float y, int idNum);
	~Arrow();
	int id;
	void setthisTexture(GLuint w, char *fname);
	float rotation();
	void update(double deltaTime);
	GLuint tex[1];
	float xSize;
	float ySize;
	int damage;
private:
	float speed;
};

#endif
