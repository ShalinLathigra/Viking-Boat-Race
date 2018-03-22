#include "Car.h"

#define MAX_SPEED 25.0f
#define MIN_ROT_SPEED 100
#define MAX_ROT_SPEED 180
int Car::carNum = 0;
Car::Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h)
	:GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements)
{
	id = carNum;
	speed = 0;
	health = h;
	mass = m;
	carNum++;
	rotationSpeed = 0.0f;
}
void Car::update(double deltaTime){
	momentum = velocity * mass;
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
		speed += .1f;
		//forces += 20.0f*f;
	}
	else if (dir == 2&&speed>0){//velocity.x > 0-MAX_VELOCITY&&dir == 2) {
		//forces -= 20.0f*f;
		speed -= .1f;
	}
	else if (speed < 0) {
		speed = 0;
	}
	//std::cout << speed << std::endl;
}
//void Car::render(Shader &s) :GameEntity(s) {}

bool Car::equals(Car* car) {
	return(carNum == car->carNum);
}

//CHECKING COLLISIONS BETWEEN CARS ONLY
void Car::checkCollisions(std::vector<Car*> ball, float deltaTime) {
	//check for collisions between balls.
	float distance;
	for (int i = 0; i < ball.size(); i++) {
		distance = sqrt(pow(ball[i]->getPosition().x - position.x, 2) + pow(ball[i]->getPosition().y - position.y, 2));
		glm::vec3 collisionNormal = glm::normalize(glm::vec3(position.x - ball[i]->getPosition().x, position.y - ball[i]->getPosition().y, 0.0f));
		if (distance < 0.1f && !equals(ball[i])) {//apply the impulse collision.
			momentum += (-1.0f*(1.95f)*glm::dot(collisionNormal, (velocity - ball[i]->velocity)) / (1.0f / mass + 1.0f / ball[i]->mass))*collisionNormal;
			ball[i]->momentum -= (-1.0f*(1.95f)*glm::dot(collisionNormal, (velocity - ball[i]->velocity)) / (1.0f / mass + 1.0f / ball[i]->mass))*collisionNormal;
			velocity = momentum / mass;
			ball[i]->velocity = ball[i]->momentum / ball[i]->mass;
			glm::vec3 direction = glm::normalize(ball[i]->getPosition() - position);
			position -= glm::vec3(0.003, 0.003, 0)*direction;//Ensure the cars dont end up inside each other
		}
	}
}


void Car::boxCollisions(std::vector<Car*> cars, float)
{
	float forceX = cos(rotationAmount *(PI / 180.0f));
	float forceY = sin(rotationAmount *(PI / 180.0f));
	glm::vec3 L1, L2, L3, L4;
	L1 = glm::vec3(cos((rotationAmount + 90)*(PI / 180.0f)), sin((rotationAmount + 90)*(PI / 180.0f)), 0);
	L2 = glm::vec3(forceX, forceY, 0);
	for (int i = 0; i < cars.size(); i++) {
		if (equals(cars[i])) {
			continue;
		}
		glm::vec3 Ax, Ay, Bx, By;
		L3 = glm::vec3(cos((cars[i]->rotationAmount + 90)*(PI / 180.0f)), sin((cars[i]->rotationAmount + 90)*(PI / 180.0f)), 0);
		L4 = glm::vec3(cos((cars[i]->rotationAmount + 90)*(PI / 180.0f)), sin((cars[i]->rotationAmount)*(PI / 180.0f)), 0);
		Ax = L1;
		Ay = L2;
		Bx = L3;
		By = L4;
		bool comp1 = glm::dot((position - cars[i]->position), L1) < (abs(glm::dot(0.05f*Ax, L1)) + abs(glm::dot(0.05f*Ay, L1)) + abs(glm::dot(0.05f*Bx, L1)) + abs(glm::dot(0.05f*By, L1)));
		bool comp2 = glm::dot((position - cars[i]->position), L2) < (abs(glm::dot(0.05f*Ax, L2)) + abs(glm::dot(0.05f*Ay, L2)) + abs(glm::dot(0.05f*Bx, L2)) + abs(glm::dot(0.05f*By, L2)));
		bool comp3 = glm::dot((position - cars[i]->position), L3) < (abs(glm::dot(0.05f*Ax, L3)) + abs(glm::dot(0.05f*Ay, L3)) + abs(glm::dot(0.05f*Bx, L3)) + abs(glm::dot(0.05f*By, L3)));
		bool comp4 = glm::dot((position - cars[i]->position), L4) < (abs(glm::dot(0.05f*Ax, L4)) + abs(glm::dot(0.05f*Ay, L4)) + abs(glm::dot(0.05f*Bx, L4)) + abs(glm::dot(0.05f*By, L4)));

		if (!(comp1&&comp2&&comp3&&comp4)) {
			//there was a collision that was detected properly
			std::cout << "Collision detected properly" << std::endl << "comp1: " << comp1 << " comp2: " << comp2 << " comp3: " << comp3 << " comp4: " << comp4 << std::endl;
		}

	}
}

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

void Car::applyImpulse(glm::vec3 impulse) 
{
	momentum += impulse;
	velocity = momentum / mass;
	velocity *= 0.95f;
}