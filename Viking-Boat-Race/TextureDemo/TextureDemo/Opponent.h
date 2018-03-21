#pragma once
#include "Car.h"
class Opponent :
	public Car
{
public:
	Opponent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h);
	~Opponent();
	void update(double deltaTime);
	void controller(float deltaTime, float skillMod);
	void SetPosition(glm::vec3 position);
	void render(Shader & shader);
private:
	glm::vec3 cameraPos;
};

