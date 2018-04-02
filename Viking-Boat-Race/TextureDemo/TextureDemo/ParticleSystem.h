#pragma once
#include "Car.h"
class ParticleSystem
{
public:
	ParticleSystem(float range, GLuint tex);
	ParticleSystem(float range, GLuint tex, glm::vec3 pos);
	static int CreateParticleArray(float range, GLuint & vbo, GLuint & ebo);

	void bindBuffers();

	void renderWind(Shader & particleprogram, Car * A, glm::vec3 playerOffset);

	void renderTrail(Shader & particleprogram, Car * A, glm::vec3 playerOffset);

	void renderBurst(Shader & shader, Car * A, glm::vec3 position, float startTime);
private:
	const float maxDuration = 3.0f;
	int system;
	GLuint texture;
	GLuint partVBO, partEBO;
};

