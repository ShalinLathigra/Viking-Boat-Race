#pragma once

#include "GameEntity.h"

class Agent : public GameEntity {


public:

	enum class State {
		CHASE, FLEE, WANDER
	};

	Agent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);

	virtual void update(double deltaTime) override;

	inline State getState(){return currentState;}
	// Setters
	inline void setTargetPos(glm::vec3 &target) { targetPos = target; }
private:
	glm::vec3 velocity;
	glm::vec3 acc;
	float maxAcc = 2.0f;
	float speed;
	
	State currentState;
	glm::vec3 targetPos;
};