#pragma once
#include "GameEntity.h"
#include <vector>
#include <algorithm>
class Car :
	public GameEntity
{
public:
	static int carNum;
	glm::vec3 momentum;
	int id;
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements,float m, int h);
	void drive(double deltaTime,int dir);
	void turn(int d,float deltaTime);
	void attack(int weaponType);
	bool equals(Car*);
	const float MAX_VELOCITY = 175;
	void update(double deltaTime) override;
	float speed;
	void checkCollisions(std::vector<Car*>, float);
	void render(Shader & shader, glm::vec3 offset);
	void boxCollisions(std::vector<Car*>, float);
	inline glm::vec3 getVel() { return velocity; }
	void applyImpulse(glm::vec3 impulse);

	inline void setPosition(glm::vec3 pos) { position = pos; }
<<<<<<< HEAD
	inline float getRotationAmount() { return rotationAmount; }
	void startFall();
	void advanceFall(double deltaTime);
	int isFalling();
	int doneFalling();
	void resetFall(glm::vec3 pos);
=======
>>>>>>> 45a943505b80a5a1da166f513bce83b28827a2fe

	void startJump();
	void advanceTimers(double deltaTime);
	inline int isJumping() {/* std::cout<< jumpTimer <<std::endl; */return jumpTimer > 0.0f; }

	float speedMod;	//Controls the different surfaces
	float rotationSpeed;
protected:
	const float maxJumpTimer = .5f;
	float jumpTimer;
	const float jumpScaleMod = 1.1f;

	float MAX_SPEED = 25.0f;

	const float holeScaleShrinkRate = .1f;

	glm::vec3 velocity;
	glm::vec3 forces;
	int health;
	float mass;
	float scaleMod;
};

