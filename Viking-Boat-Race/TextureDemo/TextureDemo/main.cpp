#include <iostream>
#include <stdexcept>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>
#include <math.h>

#include "Shader.h"
#include "Window.h"
#include "Map.h"
#include "Car.h"
#include "Opponent.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Agent Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 800;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.2);

// Global texture info
GLuint tex[5];

// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Direction		Time	        uvCoords
		-0.5f, 0.5f,		0.0f, 0.0f,		0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,			0.0f, 0.0f,		0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,		0.0f, 0.0f,		0.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,		0.0f, 0.0f,		0.0f,		0.0f, 1.0f  // Bottom-left
	};

	//in vec2 vertex;
	//in vec2 dir;
	//in float t;
	//in vec2 uv;

	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}


void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(void)
{
<<<<<<< HEAD
	//	tex = new GLuint[3];
	glGenTextures(5, tex);
=======
//	tex = new GLuint[3];
	glGenTextures(4, tex);
>>>>>>> 45a943505b80a5a1da166f513bce83b28827a2fe
	setthisTexture(tex[0], "mapImage.png");
	setthisTexture(tex[1], "car.png");
	setthisTexture(tex[2], "other.png");
	setthisTexture(tex[3], "orb.png");
	setthisTexture(tex[4], "smoke.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
}

<<<<<<< HEAD
=======
GLuint SetupParticleShaders() // returns ID of newly created program
{

	// Set up shaders

	// Create a shader from vertex program source code
	std::string vp = ResourceManager::LoadTextFile("shaderPart.vert");
	const char *source_vpart = vp.c_str();
	std::string fp = ResourceManager::LoadTextFile("shader.frag");
	const char *source_fp = fp.c_str();


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &source_vpart, NULL);
	glCompileShader(vs);

	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling vertex shader:") + std::string(buffer)));
	}

	// Create a shader from the fragment program source code
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &source_fp, NULL);
	glCompileShader(fs);

	// Check if shader compiled successfully
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling fragmentshader: ") + std::string(buffer)));
	}

	// Create a shader program linking both vertex and fragment shaders
	// together
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// Check if shaders were linked successfully
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(program, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error linking shaders: ") +
			std::string(buffer)));
	}

	// Delete memory used by shaders, since they were already compiled
	// and linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

>>>>>>> 45a943505b80a5a1da166f513bce83b28827a2fe
void AttributeBinding(GLuint program)
{

	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the particle elements
<<<<<<< HEAD
	GLint vertex_att = glGetAttribLocation(program, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);

	GLint dir_att = glGetAttribLocation(program, "dir");
	glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(dir_att);

	GLint time_att = glGetAttribLocation(program, "t");
	glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(time_att);

	GLint tex_att = glGetAttribLocation(program, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);

}
=======

		GLint vertex_att = glGetAttribLocation(program, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 *
		sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);

	GLint dir_att = glGetAttribLocation(program, "dir");
	glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
		(void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(dir_att);

	GLint time_att = glGetAttribLocation(program, "t");
	glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 *
		sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(time_att);

	GLint tex_att = glGetAttribLocation(program, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
		(void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);

}
int CreateParticleArray(void) {

	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D): position(2), direction(2), colour(3), time(1), 2D texture coordinates(2)
		//   const int face_att = 3; // Vertex indices (3)

		GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
	   -0.5f, 0.5f,    1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,    0.0f, 1.0f, 0.0f,	    1.0f, 0.0f, // Top-right
		0.5f, -0.5f,   0.0f, 0.0f, 1.0f,	    1.0f, 1.0f, // Bottom-right
	   -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,	    0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[1000 * vertex_attr];
	float theta, r, tmod;

	for (int i = 0; i < 1000; i++)
	{
		if (i % 4 == 0)
		{
			theta = (2.0f * (rand() % 10000) / 10000.0f - 1.0f)*0.13f;
			r = 0.7f + 0.3*(rand() % 10000) / 10000.0f;
			tmod = (rand() % 10000) / 10000.0f;
		}

		//position
		particleatt[i*vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i*vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

		//direction
		particleatt[i*vertex_attr + 2] = sin(theta)*r;
		particleatt[i*vertex_attr + 3] = cos(theta)*r;

		//time
		particleatt[i*vertex_attr + 4] = tmod;

		//texture Coords
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
			manyface[i * 6 + j] = face[j] + i * 4;
	}

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt,
		GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface,
		GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(manyface);

}

void drawParticles(Shader particleprogram, int particlesize)
{

	// Select proper shader program to use
	particleprogram.enable();

	glDepthMask(GL_FALSE);
	//set displacement

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();
	rot = glm::rotate(rot, -k * 360 / 6.283f, glm::vec3(0, 0, 1));
	rot = glm::translate(rot, glm::vec3());
	rot = glm::scale(rot, glm::vec3(0.1, 0.1, 0.1));
	// get ready to draw, load matrix
	particleprogram.setUniformMat4("x", rot);
	particleprogram.setUniform1f("time", k);

	glBindTexture(GL_TEXTURE_2D, tex[3]);

	// Draw 
	glDrawElements(GL_TRIANGLES, 6 * particlesize, GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
}

>>>>>>> 45a943505b80a5a1da166f513bce83b28827a2fe
// Main function that builds and runs the game
int main(void){
    try {
		std::cout << "TESTVAL: " << atan2(1, 1) * (180 / 3.141592653589793238463) << ", ";
		std::cout << "TESTVAL: " << atan2(1, -1)  * (180 / 3.141592653589793238463) << ", ";
		std::cout << "TESTVAL: " << atan2(-1, -1) * (180 / 3.141592653589793238463) << ", ";
		std::cout << "TESTVAL: " << atan2(-1, 1) * (180 / 3.141592653589793238463) << ", ";

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

        // Set up z-buffer for rendering
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_ONE, GL_ONE);

		// Create geometry of the square
		int size = CreateSquare();

        // Set up shaders
		Shader shader("shader.vert", "shader.frag");

		//Particles
		Shader partShader("shaderPart.vert", "shader.frag");
		AttributeBinding(partShader.getShaderID());
		//int carParticles = CreateParticleArray(.5f);

		setallTexture();

		// Setup game objects
		Map map = Map::Map(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(18.0f, 9.0f, 1.0f), 0.0f, tex[0], size);
		Car* player = new Car(map.getStartPosition(4), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[1], size, 12, 10);
		Opponent* enemy0 = new Opponent(map.getStartPosition(0), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);
		Opponent* enemy1 = new Opponent(map.getStartPosition(1), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);
		Opponent* enemy2 = new Opponent(map.getStartPosition(2), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);
		Opponent* enemy3 = new Opponent(map.getStartPosition(3), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);

		//PARTICLE SYSTEM TEST
		int system = CreateParticleArray();
		Shader particleShader("shaderPart.vert", "shader.frag");

        // Run the main loop
		glm::vec3 position = glm::vec3();
		double lastTime = glfwGetTime();
		std::vector<Opponent*> enemies;
		std::vector<Car*> allCars;//this is used for checking collisions
		enemies.push_back(enemy0);
		enemies.push_back(enemy1);
		enemies.push_back(enemy2);
		enemies.push_back(enemy3);
		allCars.push_back(enemy0);
		allCars.push_back(enemy1);
		allCars.push_back(enemy2);
		allCars.push_back(enemy3);
		allCars.push_back(player);

        while (!glfwWindowShouldClose(window.getWindow())){
            // Clear background
			window.clear(glm::vec3(0.0f, .1f, 0.0f));

            // Select proper shader program to use
			shader.enable();
			// Get mouse position relative to screenspace [-1, 1]
			//if (agent.getState() != Agent::State::WANDER) {
			//	double mouseX, mouseY;
			//	glfwGetCursorPos(window.getWindow(), &mouseX, &mouseY);
			//	float screenSpaceMouseX = (mouseX / window_width_g) * 2 - 1;
			//	float screenSpaceMouseY = -((mouseY / window_height_g) * 2 - 1);
			//	agent.setTargetPos(glm::vec3(screenSpaceMouseX, screenSpaceMouseY, 0.0f));
			//}
			// Calculate delta time


			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			//Input
			if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {//these ifs are used to get keyboard input;
				player->drive(deltaTime, 1);
			}
			if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
				player->turn(1, deltaTime);
			}
			if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
				player->drive(deltaTime, 2);
			}
			if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
				player->turn(2, deltaTime);
			}

			if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {

			}
			

<<<<<<< HEAD

=======
			//allCars[0]->setPosition(map.getFlag(0));
>>>>>>> 45a943505b80a5a1da166f513bce83b28827a2fe
			for (int i = 0; i < allCars.size(); i++) {
				allCars[i]->checkCollisions(allCars, deltaTime);
				//MORE PARTICLE TESTING
				//if (i == allCars.size() - 1)
					//drawParticles(particleShader, system);

				//get property + setValues
				if (map.getPropertyUnder(allCars[i]) == Tile::TileProp::WALL) {
					map.calculateCarCollisions(allCars[i]);
				}

				allCars[i]->update(deltaTime);
				allCars[i]->render(shader, player->getPosition());

			}

			//Draw Particles after drawing the Cars, makes it look a bit better
			partShader.enable();
			AttributeBinding(partShader.getShaderID());
			drawParticles(partShader.getShaderID(), carParticles, player);


			//Draw/Update Everything Else
			shader.enable();
			AttributeBinding(shader.getShaderID());


			//map.getPropertyUnder(player);
			//if (map.getPropertyUnder(player) == Tile::TileProp::WALL) {
			//	map.calculateCarCollisions(player);
			//}
			//player->update(deltaTime);
			//player->render(shader, player->getPosition());

			map.setPosition(player->getPosition());
			std::cout << "FLAG NUMBER: " << enemies[0]->getFlagIndex() << std::endl;
			std::cout << "FLAG COORDS: " << enemies[0]->getNextFlag().x << "," << enemies[0]->getNextFlag().y << std::endl;
			std::cout << "AI COORDS:   " << enemies[0]->getPosition().x << "," << enemies[0]->getPosition().y << std::endl;

			for (int i = 0; i < enemies.size(); i++) {
				//enemies[i]->SetPosition(player->getPosition());//Is this still needed?
				//std::cout << "CURRENT ITERATION: " << i;
				int result = enemies[i]->controller(deltaTime, 0);//Checks turning status for all vehicles
				std::cout << "Enemy Position: " << enemies[0]->getPosition().x << "," << enemies[0]->getPosition().y << std::endl;
				if (result == 1)//If we need to set a new flag
				{
					std::cout << "RESET " << i;
					enemies[i]->setFlagIndex(enemies[i]->getFlagIndex() + 1);
					if (enemies[i]->getFlagIndex() >= map.getMaxFlags())
						enemies[i]->setFlagIndex(0);
						
					enemies[i]->setNextFlag(map.getFlag(enemies[i]->getFlagIndex()));
				}
			}

			map.render(shader);

		//	glDrawArrays(GL_TRIANGLES, 0, 6); // if glDrawArrays be used, glDrawElements will be ignored 

            // Update other events like input handling
            glfwPollEvents();
            // Push buffer drawn in the background onto the display
            glfwSwapBuffers(window.getWindow());
        }
    }
    catch (std::exception &e){
		// print exception and sleep so error can be read
        PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
    }

    return 0;
}

