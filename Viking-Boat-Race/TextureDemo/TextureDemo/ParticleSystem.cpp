#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(float range, GLuint tex)
	: texture(tex)
{
	system = ParticleSystem::CreateParticleArray(range, partVBO, partEBO);
}

ParticleSystem::ParticleSystem(float range, GLuint tex, glm::vec3 pos)
	: texture(tex)
{
	system = ParticleSystem::CreateParticleArray(range, partVBO, partEBO);
}
void ParticleSystem::bindBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, partVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partEBO);

}
void ParticleSystem::renderWind(Shader & particleprogram, Car * A, glm::vec3 playerOffset)
{
	glDepthMask(GL_FALSE);

	particleprogram.enable();
	particleprogram.AttributeBinding();

	//set displacement

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();
	rot = glm::translate(rot, A->getPosition() + .1f * glm::vec3(cos(A->getRotationAmount() * 3.14159/180.0f), sin(A->getRotationAmount() * 3.14159 / 180.0f) , 0.0f) - playerOffset);
	rot = glm::rotate(rot, A->getRotationAmount() + 45.0f, glm::vec3(0, 0, 1));
	float scale = 0.025;
	rot = glm::scale(rot, glm::vec3(scale, scale, scale));
	// get ready to draw, load matrix
	particleprogram.setUniformMat4("x", rot);
	particleprogram.setUniform1f("time", k);

	particleprogram.setUniform1i("isTrail", 0);

	float minSpeed = 1.25f;
	float maxSpeed = 2.5f;
	float speed = minSpeed + (maxSpeed - minSpeed) * A->speedPercentage();
	particleprogram.setUniform1f("speed", speed);

	glBindTexture(GL_TEXTURE_2D, texture);

	// Draw First
	glDrawElements(GL_TRIANGLES, 6 * system, GL_UNSIGNED_INT, 0);

	//Draw Second
	rot = glm::rotate(rot, 90.0f, glm::vec3(0, 0, 1));
	particleprogram.setUniformMat4("x", rot);
	glDrawElements(GL_TRIANGLES, 6 * system, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
}
void ParticleSystem::renderTrail(Shader & particleprogram, Car * A, glm::vec3 playerOffset)
{

	// Select proper shader program to use
	particleprogram.enable();
	particleprogram.AttributeBinding();

	//set displacement

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();
	rot = glm::translate(rot, A->getPosition() - playerOffset);
	rot = glm::rotate(rot, A->getRotationAmount() + 90.0f, glm::vec3(0, 0, 1));
	float scale = 0.05;
	rot = glm::scale(rot, glm::vec3(scale, scale, scale));
	// get ready to draw, load matrix
	particleprogram.setUniformMat4("x", rot);
	particleprogram.setUniform1f("time", k);

	particleprogram.setUniform1i("isTrail", 1);

	float minSpeed = 2.0f;
	float maxSpeed = 7.5f;


	float speed = minSpeed + (maxSpeed - minSpeed) * A->speedPercentage();

	particleprogram.setUniform1f("speed", speed);

	glBindTexture(GL_TEXTURE_2D, texture);

	// Draw 
	glDrawElements(GL_TRIANGLES, 6 * system, GL_UNSIGNED_INT, 0);
}
void ParticleSystem::renderBurst(Shader & particleprogram, Car * A, glm::vec3 position, float startTime)
{
	// Select proper shader program to use
	particleprogram.enable();
	//set displacement

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();
	rot = glm::translate(rot, position /*- A->getPosition()*/);	//Normally I would subtract by player->getPosition, however, since we want the effects to always be visible, ranges are -1, 1 on x and -1, 1 on y, so we don't trans;ate
	float scale = 0.025;
	rot = glm::scale(rot, glm::vec3(scale, scale, scale));
	// get ready to draw, load matrix
	particleprogram.setUniformMat4("x", rot);
	particleprogram.setUniform1f("elapsedTime", k - startTime);
	particleprogram.setUniform1f("speed", 15.5f);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Draw 
	glDrawElements(GL_TRIANGLES, 6 * system, GL_UNSIGNED_INT, 0);
}

int ParticleSystem::CreateParticleArray(float range, GLuint & vbo, GLuint & ebo) {

	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D position(2), direction(2), 2D texture coordinates(2), time(1)
								//   const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	  0.0f, 0.0f, // Top-left
		0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,	  1.0f, 0.0f, // Top-right
		0.5f, -0.5f,   0.0f, 0.0f, 1.0f,	  1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,   1.0f, 1.0f, 1.0f,	  0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[1000 * vertex_attr];
	float theta, r, tmod;

	for (int i = 0; i < 1000; i++)
	{
		if (i % 4 == 0) {
			theta = (2 * (rand() % 10000) / 10000.0f - 1.0f)*range;
			r = 0.7f + 0.3*(rand() % 10000) / 10000.0f;
			tmod = (rand() % 10000) / 10000.0f;
		}

		particleatt[i*vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i*vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

		particleatt[i*vertex_attr + 2] = sin(theta)*r;
		particleatt[i*vertex_attr + 3] = cos(theta)*r;


		particleatt[i*vertex_attr + 4] = tmod;

		particleatt[i*vertex_attr + 5] = vertex[(i % 4) * 7 + 5];
		particleatt[i*vertex_attr + 6] = vertex[(i % 4) * 7 + 6];
	}
	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint manyface[1000 * 6];

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			manyface[i * 6 + j] = face[j] + i * 4;
		}
	}

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface, GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(manyface);

}