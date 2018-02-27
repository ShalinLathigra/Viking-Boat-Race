#include "Agent.h"
#include <time.h>
Agent::Agent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	: GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements),
	  targetPos(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), acc(0.0f, 0.0f, 0.0f)
{
	currentState = State::WANDER;
	speed = 0.9f;
}

void Agent::update(double deltaTime) {
	if (currentState == State::CHASE || currentState == State::FLEE) {
		glm::vec3 targetVel = glm::normalize(targetPos - position);
		acc = maxAcc * glm::normalize(targetVel - velocity);
		velocity += acc * (float)deltaTime;

		if (glm::length(velocity) > speed) {
			velocity = speed * glm::normalize(velocity);
		}

		if (currentState == State::CHASE) {
			position += velocity * speed * (float)deltaTime;
		} else {
			position -= velocity * speed * (float)deltaTime;
		}
	}
	if (currentState == State::WANDER) {
		if (glm::length(targetPos - position) < .01) {
			float w = (float)((float)rand() / (float)RAND_MAX);
			float r = (float)((float)rand() / (float)RAND_MAX);
			std::cout << w << " " << r << std::endl;
			float theta = (rand() % 3600) / 10.0f;
			targetPos = position + velocity*w + glm::vec3(r * cos(theta * 3.14159 /180), r * sin(theta * 3.14159 / 180), 0);
		}
		velocity = glm::normalize(targetPos - position);
		position += velocity * speed * (float)deltaTime;
	}
}