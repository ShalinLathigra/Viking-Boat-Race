#include "Car.h"



Car::Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements)
{
	health = h;
	mass = m;

}
void Car::update(double deltaTime){
	position += velocity * (float)deltaTime;
}
void Car::drive(double deltaTime) {
	float forceX = (2.0f * cos(rotationAmount *(PI / 180.0f)))*deltaTime;
	float forceY = (2.0f * sin(rotationAmount *(PI / 180.0f)))*deltaTime;
	glm::vec3 forces = glm::vec3(forceX, forceY, 0);
	velocity += forces;
}

void Car::turn(int d) {
	if (d == 1) {//turn left
		rotationAmount += 1;
	}
	else if (d == 2) {//turn Right
		rotationAmount -= 1;
	}
}