#include "Car.h"



Car::Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements)
{
	health = h;
	mass = m;

}
void Car::update(double deltaTime){
	float forceX = (2.0f * cos(rotationAmount *(PI / 180.0f)))*deltaTime;
	float forceY = (2.0f * sin(rotationAmount *(PI / 180.0f)))*deltaTime;
	glm::vec3 forces = glm::vec3(forceX, forceY, 0);

	position += velocity *forces* (float)deltaTime;
	std::cout << velocity.x << " and " << velocity.y << std::endl;
}
void Car::drive(double deltaTime,int dir) {
	//float forceX = (2.0f * cos(rotationAmount *(PI / 180.0f)))*deltaTime;
	//float forceY = (2.0f * sin(rotationAmount *(PI / 180.0f)))*deltaTime;
	//glm::vec3 forces = glm::vec3(forceX, forceY, 0);
	if (velocity.x < MAX_VELOCITY && dir == 1) {
		velocity += glm::vec3(0.1,0.1,0.0);
		
	}
	else if (velocity.x > 0-MAX_VELOCITY&&dir == 2) {
		velocity -= glm::vec3(0.1, 0.1, 0.0);
	}
}
//void Car::render(Shader &s) :GameEntity(s) {}

void Car::turn(int d) {
	if (d == 1) {//turn left
		rotationAmount += 1;
	}
	else if (d == 2) {//turn Right
		rotationAmount -= 1;
	}
}