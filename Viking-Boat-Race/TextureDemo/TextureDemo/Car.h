#pragma once
#include "GameEntity.h"
class Car :
	public GameEntity
{
public:
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements,float m, int h);
	void drive(double deltaTime);
	void turn(int d);
	void attack(int weaponType);
	void update(double deltaTime) override;
private:
	glm::vec3 velocity;
	glm::vec3 force;
	int health;
	float mass;
	
};

