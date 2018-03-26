#pragma once
#include "Car.h"
class Opponent :
	public Car
{
public:
	Opponent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h, glm::vec3 firstFlag, float setMAX_SPEED);
	~Opponent();
	void update(double deltaTime);
	int controller(float deltaTime, float skillMod);
	void SetPosition(glm::vec3 position);
	void setNextFlag(glm::vec3 flag);
	void setFlagIndex(int fIndex);
	glm::vec3 getNextFlag();
	int getFlagIndex();
	void render(Shader & shader);
private:
	glm::vec3 cameraPos;
	glm::vec3 nextFlag;
	int flagIndex;
};

