#include "Car.h"

#define MAX_SPEED 25.0f
#define MIN_ROT_SPEED 100
#define MAX_ROT_SPEED 180

Car::Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements)
{
	speed = 0;
	health = h;
	mass = m;

	rotationSpeed = 0.0f;
}
void Car::update(double deltaTime){
	glm::vec3 momentum = velocity * mass;
	forces += momentum;
	float forceX = cos(rotationAmount *(PI / 180.0f));
	float forceY = sin(rotationAmount *(PI / 180.0f));

	glm::vec3 f = glm::vec3(forceX, forceY, 0);
	velocity = (float)speed * (f/mass)*(float)deltaTime;

	velocity += (forces / mass)*(float) deltaTime;
	position += velocity;
	forces = glm::vec3(0,0,0);
}
void Car::render(Shader & shader, glm::vec3 offset)
{
	position -= offset;
	GameEntity::render(shader);
	position += offset;
}

void Car::drive(double deltaTime,int dir) {
	
	if(dir==1&&speed<MAX_SPEED){//velocity.x < MAX_VELOCITY && dir == 1) {
		//velocity += glm::vec3(0.1,0.1,0.0);
		speed += .5;
		//forces += 20.0f*f;
	}
	else if (dir == 2&&speed>0){//velocity.x > 0-MAX_VELOCITY&&dir == 2) {
		//forces -= 20.0f*f;
		speed -= .5;
	}
	else if (speed < 0) {
		speed = 0;
	}
	//std::cout << speed << std::endl;
}
//void Car::render(Shader &s) :GameEntity(s) {}

void Car::turn(int d,float deltaTime) {
	//at MAX speed, rotation speed = 1 / 12s = 30 deg / second
	//at MIN speed, rotation speed = 1 / s = 360 deg / second
	rotationSpeed = (speed > 0) ? MIN_ROT_SPEED + (MAX_ROT_SPEED - MIN_ROT_SPEED) * ((float)(MAX_SPEED - speed) / (float)MAX_SPEED) : 0;

	if (d == 1) {//turn left
		rotationAmount += rotationSpeed * deltaTime;		
	}
	else if (d == 2) {//turn Right
		rotationAmount -= rotationSpeed * deltaTime;
	}
}