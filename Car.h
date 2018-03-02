#pragma once
#include "GameEntity.h"
class Car :
	public GameEntity
{
public:
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements,float m, int h);
	void drive(double deltaTime,int dir);
	void turn(int d,float deltaTime);
	void attack(int weaponType);
	const float MAX_VELOCITY = 175;
	void update(double deltaTime) override;
	int speed;
private:
	glm::vec3 velocity;
	glm::vec3 forces;
	int health;
	float mass;
	
};

