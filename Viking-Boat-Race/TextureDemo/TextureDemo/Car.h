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
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h);
	void drive(double deltaTime, int dir);
	void turn(int d, float deltaTime);
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
	inline float getRotationAmount() { return rotationAmount; }
	void startFall();
	void advanceFall(double deltaTime);
	inline int isFalling() { return falling; }
	inline int doneFalling() { return scaleMod <= 0.0f; }
	inline void resetFall(glm::vec3 pos) { position = pos; scaleMod = 1.0f; falling = 0; velocity = glm::vec3(); }
	void startJump();
	void advanceTimers(double deltaTime);
	inline int isJumping() {/* std::cout<< jumpTimer <<std::endl; */return jumpTimer > 0.0f; }

	float speedMod;	//Controls the different surfaces
	float rotationSpeed;

	inline float speedPercentage() { return speed / MAX_SPEED; }
protected:
	const float maxJumpTimer = .5f;
	float jumpTimer;
	const float jumpScaleMod = 1.1f;
	int currentLap;
	float MAX_SPEED = 25.0f;

	const float holeScaleShrinkRate = .5f;
	int falling;

	glm::vec3 velocity;
	glm::vec3 forces;
	int health;
	float mass;
	float scaleMod;
};

