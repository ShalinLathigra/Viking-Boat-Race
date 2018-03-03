#include "Car.h"

#define MAX_SPEED 300
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
	std::cout << "(" << position.x << ", " << position.y << ")" << std::endl;
	
	glm::vec3 momentum = 1.2f*velocity * mass;
	forces += momentum;
	float forceX = cos(rotationAmount *(PI / 180.0f))*deltaTime;
	float forceY = sin(rotationAmount *(PI / 180.0f))*deltaTime;

	glm::vec3 f = glm::vec3(forceX, forceY, 0);
	velocity = (float)speed *20.0f*(f/mass)*(float)deltaTime;

	velocity += (forces / mass)*(float) deltaTime;
	position += velocity;
	forces = glm::vec3(0,0,0);
}
void Car::ReflectY()
{
	rotationAmount *= -1.0f;
	speed *= .25f;
}
void Car::ReflectX()
{
	rotationAmount = 180 - rotationAmount;
	speed *= .25f;
}
void Car::drive(double deltaTime,int dir) {
	
	if(dir==1&&speed<MAX_SPEED){//velocity.x < MAX_VELOCITY && dir == 1) {
		//velocity += glm::vec3(0.1,0.1,0.0);
		speed += 1;
		//forces += 20.0f*f;
	}
	else if (dir == 2&&speed>0){//velocity.x > 0-MAX_VELOCITY&&dir == 2) {
		//forces -= 20.0f*f;
		speed -= 2;
		speed = (speed < 0) ? 0 : speed;
	}
	//std::cout << speed << std::endl;
}
//void Car::render(Shader &s) :GameEntity(s) {}

void Car::turn(int d,float deltaTime) {
	//at MAX speed, rotation speed = 1 / 12s = 30 deg / second
	//at MIN speed, rotation speed = 1 / s = 360 deg / second
	rotationSpeed = (speed > 0) ? MIN_ROT_SPEED + (MAX_ROT_SPEED - MIN_ROT_SPEED) * ((float)(MAX_SPEED - speed) / (float)MAX_SPEED) : 0;
	//std::cout << rotationSpeed << std::endl;

	if (d == 1) {//turn left
		rotationAmount += rotationSpeed * deltaTime;		
	}
	else if (d == 2) {//turn Right
		rotationAmount -= rotationSpeed * deltaTime;
	}
	//forceX = cos(rotationAmount *(PI / 180.0f))*deltaTime;
	//forceY = sin(rotationAmount *(PI / 180.0f))*deltaTime;

	//f = glm::vec3(forceX, forceY, 0);
	//velocity *= 44.0f*f*mass;
	//std::cout << "Force "<<forces.x <<" and "<<forces.y << std::endl;
	//std::cout << "Velocity "<<(velocity*mass).x << " and " << (velocity*mass).y << std::endl;
}

void Car::draw(Shader &shader) 
{
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader 
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAmount, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 transformationMatrix = rotationMatrix * scaleMatrix;
	shader.setUniformMat4("x", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}