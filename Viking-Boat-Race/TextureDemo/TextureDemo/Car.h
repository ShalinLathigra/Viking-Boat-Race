#pragma once
#ifndef CAR_H
#define CAR_H


#include "GameEntity.h"
#include "Arrow.h"
#include <vector>
#include <algorithm>
class Car :
	public GameEntity
{
	friend class Arrow;
public:
	int place;
	int money;
	static int carNum;
	glm::vec3 momentum;
	int id;
	int shots;
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h, GLuint arrow);
	void drive(double deltaTime, int dir);
	void turn(int d, float deltaTime);
	void attack(float dir, std::vector<Arrow>& arrows);
	bool equals(Car*);
	
	void update(double deltaTime) override;
	float speed;
	void checkCollisions(std::vector<Car*>, float);
	void render(Shader & shader, glm::vec3 offset);
	GLuint arrowTex;
	void boxCollisions(std::vector<Car*>, float);
	inline glm::vec3 getVel() { return velocity; }
	void applyImpulse(glm::vec3 impulse);

	int checkArrows(std::vector<Arrow>& arrows);

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
	inline int isSpeeding() { return (speedPercentage() > .9f); }

	inline void nextLap() { currentLap++; }
	inline int getCurrentLap() { return currentLap; }
	inline void setCurrentLap(int newLap) { currentLap = newLap; }
	float mass;
	float MAX_SPEED = 25.0f;
protected:
	const float maxJumpTimer = .5f;
	float jumpTimer;
	const float jumpScaleMod = 1.1f;
	int currentLap;
	

	const float holeScaleShrinkRate = .5f;
	int falling;

	glm::vec3 velocity;
	glm::vec3 forces;
	int health;
	
	float scaleMod;
};

#endif