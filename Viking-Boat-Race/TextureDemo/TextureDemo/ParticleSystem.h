#pragma once
#include "GameEntity.h"
#include "Car.h"
class ParticleSystem
{
public:
	ParticleSystem(float range);

	int CreateParticleArray(float range);
	void renderSystem(GLuint particleprogram, Car * A);

	~ParticleSystem();

private:
	int particleSystem;
	int matrixLoc, timeLoc, intLoc;
	float rotationAmount;
	glm::vec3 scale, position;
	GLuint texture;
};

