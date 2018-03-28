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
	//	tex = new GLuint[3];
	glGenTextures(5, tex);
	setthisTexture(tex[0], "mapImage.png");
	setthisTexture(tex[1], "car.png");
	setthisTexture(tex[2], "other.png");
	setthisTexture(tex[3], "orb.png");
	setthisTexture(tex[4], "smoke.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
}

void AttributeBinding(GLuint program)
{

	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the particle elements
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
// Main function that builds and runs the game
int main(void){
    try {
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
		Opponent* enemy0 = new Opponent(map.getStartPosition(0), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10);
		Opponent* enemy1 = new Opponent(map.getStartPosition(1), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10);
		Opponent* enemy2 = new Opponent(map.getStartPosition(2), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10);
		Opponent* enemy3 = new Opponent(map.getStartPosition(3), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10);

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
			


			for (int i = 0; i < allCars.size(); i++) {
				allCars[i]->boxCollisions(allCars, deltaTime);

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
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->SetPosition(player->getPosition());
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

