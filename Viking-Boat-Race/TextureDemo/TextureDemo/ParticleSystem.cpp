#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(float range)
{
	particleSystem = CreateParticleArray(range);
}
int ParticleSystem::CreateParticleArray(float range) {//for car exhaust, range = .5
	const int partCount = 200;
	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D)
								//position(2), direction(2), 2D texture coordinates(2), time(1)
								//   const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f,  0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,	  1.0f, 0.0f, // Top-right
		0.5f, -0.5f,   0.0f, 0.0f, 1.0f,	  1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,   1.0f, 1.0f, 1.0f,	  0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[partCount * vertex_attr];
	float theta, r, tmod;

	for (int i = 0; i < partCount; i++)
	{
		if (i % 4 == 0) {
			//theta = (2 * (rand() % 10000) / 10000.0f - 1.0f)*0.13f;
			theta = (2 * (rand() % 10000) / 10000.0f - 1.0f)*range;

			r = 0.7f + 0.3*(rand() % 10000) / 10000.0f;
			tmod = (rand() % 10000) / 10000.0f;
		}
		//in vec2 vertex;
		//in vec2 dir;
		//in float t;
		//in vec2 uv;

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

	GLuint manyface[partCount * 6];

	for (int i = 0; i < partCount; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			manyface[i * 6 + j] = face[j] + i * 4;
		}
	}

	GLuint vbo, ebo;

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

void ParticleSystem::renderSystem(GLuint particleprogram, Car * A)
{
	glDepthMask(GL_FALSE);
	// Select proper shader program to use
	glUseProgram(particleprogram);

	//set displacement
	int matrixLocation = glGetUniformLocation(particleprogram, "x");
	int timeLocation = glGetUniformLocation(particleprogram, "time");
	int intensityLocation = glGetUniformLocation(particleprogram, "intensity");
	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();

	rot = glm::translate(rot, .05f * glm::vec3(cos((A->getRotationAmount() + 180.0f) * 3.14159f / 180.0f), sin((A->getRotationAmount() + 180.0f) * 3.14159f / 180.0f), 0.0f));
	rot = glm::rotate(rot, A->getRotationAmount() + 90.0f, glm::vec3(0, 0, 1));
	rot = glm::scale(rot, glm::vec3(0.05, 0.05, 0.05));
	// get ready to draw, load matrix
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &rot[0][0]);
	glUniform1f(timeLocation, k);
	glUniform1f(intensityLocation, glm::length(A->momentum) * 50.0f + 2.5f);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Draw 
	glDrawElements(GL_TRIANGLES, 6 * particleSystem, GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
}