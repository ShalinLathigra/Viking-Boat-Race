#pragma once
#include "GameEntity.h"
class Car :
	public GameEntity
{
public:
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements,float m, int h);
	void drive(double deltaTime,int dir);
	void turn(int d,float deltaTime);
	void draw(Shader & shader);
	void attack(int weaponType);
	const float MAX_VELOCITY = 175;
	void update(double deltaTime) override;
	int speed;

	void ReflectY();		//Used for collisions with walls, easy vertical and horizontal collisions
	void ReflectX();

private:
	glm::vec3 velocity;
	glm::vec3 forces;
	float rotationSpeed;
	int health;
	float mass;
	
};

