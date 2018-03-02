#include "Car.h"



Car::Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements)
{
	speed = 0;
	health = h;
	mass = m;
}
void Car::update(double deltaTime){
	glm::vec3 momentum = 1.2f*velocity * mass;
	forces += momentum;
	float forceX = cos(rotationAmount *(PI / 180.0f))*deltaTime;
	float forceY = sin(rotationAmount *(PI / 180.0f))*deltaTime;

	glm::vec3 f = glm::vec3(forceX, forceY, 0);
	velocity = (float)speed *20.0f*(f/mass)*(float)deltaTime;

	velocity += (forces / mass)*(float) deltaTime;
	//velocity *= 0.995f;
	position += velocity;
	//std::cout << velocity.x << " and " << velocity.y << std::endl;
	forces = glm::vec3(0,0,0);
}
void Car::drive(double deltaTime,int dir) {
	
	if(dir==1&&speed<500){//velocity.x < MAX_VELOCITY && dir == 1) {
		//velocity += glm::vec3(0.1,0.1,0.0);
		speed += 1;
		//forces += 20.0f*f;
	}
	else if (dir == 2&&speed>-500){//velocity.x > 0-MAX_VELOCITY&&dir == 2) {
		//forces -= 20.0f*f;
		speed -= 1;
	}
	//std::cout << speed << std::endl;
}
//void Car::render(Shader &s) :GameEntity(s) {}

void Car::turn(int d,float deltaTime) {
	float forceX = cos(rotationAmount *(PI / 180.0f))*deltaTime;
	float forceY = sin(rotationAmount *(PI / 180.0f))*deltaTime;
	glm::vec3 f = glm::vec3(forceX, forceY, 0);
	
	if (d == 1) {//turn left
		rotationAmount += 1;
	}
	else if (d == 2) {//turn Right
		rotationAmount -= 1;
	}
	//forceX = cos(rotationAmount *(PI / 180.0f))*deltaTime;
	//forceY = sin(rotationAmount *(PI / 180.0f))*deltaTime;

	//f = glm::vec3(forceX, forceY, 0);
	//velocity *= 44.0f*f*mass;
	//std::cout << "Force "<<forces.x <<" and "<<forces.y << std::endl;
	//std::cout << "Velocity "<<(velocity*mass).x << " and " << (velocity*mass).y << std::endl;
}