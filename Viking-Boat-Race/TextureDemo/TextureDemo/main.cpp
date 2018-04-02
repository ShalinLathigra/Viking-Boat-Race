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
#include "ParticleSystem.h"

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
int CreateSquare(GLuint & vbo, GLuint & ebo) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Dir       time      Texcoords
		-0.5f, 0.5f,	 0.0f, 0.0f,	0.0f,	0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 0.0f,	0.0f,	1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f,	0.0f,	1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 0.0f, 0.0f,	0.0f,	0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

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
	setthisTexture(tex[4], "fireEffect.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
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
		GLuint squareVBO, squareEBO;
		int size = CreateSquare(squareVBO, squareEBO);

		setallTexture();


        // Set up shaders
		Shader shader("shader.vert", "shader.frag");
		Shader partShader("shaderPart.vert", "shaderPart.frag");
		Shader boomShader("boomShader.vert", "boomShader.frag");

		//PARTICLE SYSTEMS
		ParticleSystem exhaust( .1, tex[4] );

		//EXPLOSION SYSTEMS
		int numExplosions = 0;
		const int maxExplosions = 7;
		float explosionTimer = 3.0f;
		ParticleSystem explosion(3.0f, tex[3]);
		std::vector<glm::vec3> explosions = std::vector<glm::vec3>();//x, y, time

		const float maxTimer = 3.0f / (float)maxExplosions;
		float timer = maxTimer;

		//SPEED SYSTEMS
		//draw two lines on either side of the player when at max speed
		ParticleSystem windStream(0.0f, tex[3]);

		// Setup game objects
		Map map = Map::Map(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(18.0f, 9.0f, 1.0f), 0.0f, tex[0], size);
		Car* player = new Car(map.getStartPosition(4), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[1], size, 12, 10);
		Opponent* enemy0 = new Opponent(map.getStartPosition(0), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);
		Opponent* enemy1 = new Opponent(map.getStartPosition(1), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);
		Opponent* enemy2 = new Opponent(map.getStartPosition(2), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);
		Opponent* enemy3 = new Opponent(map.getStartPosition(3), glm::vec3(0.1f, 0.1f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 10);


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
			shader.AttributeBinding();
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
				allCars[i]->checkCollisions(allCars, deltaTime);
				//get property + setValues
				if (map.getPropertyUnder(allCars[i]) == Tile::TileProp::WALL) {
					map.calculateCarCollisions(allCars[i]);
				}

				allCars[i]->update(deltaTime);
				allCars[i]->render(shader, player->getPosition());
			}


			//**************************************************************************************
			//************************        							  **************************
			//************************        Render Exhaust Trail        **************************
			//************************        							  **************************
			//**************************************************************************************
			partShader.enable();
			exhaust.bindBuffers();
			partShader.AttributeBinding();
			exhaust.renderTrail(partShader, player);

			if (player->isSpeeding()) 
			{
				windStream.bindBuffers();
				partShader.AttributeBinding();
				windStream.renderWind(partShader, player);
			}
			//**************************************************************************************
			//*****************************   							     ***********************
			//*****************************          Finish Exhaust          ***********************
			//*****************************   Update and Render Explosions   ***********************
			//*****************************   							     ***********************
			//**************************************************************************************
			
			if (numExplosions < maxExplosions ) 
			{
				if (timer <= 0.0f) 
				{
					float x = (float)(rand() % 200) / 100.0f - 1.0f;
					float y = (float)(rand() % 200) / 100.0f - 1.0f; 
					//std::cout << x << ", " << y << std::endl;
					explosions.push_back(glm::vec3(x, y, glfwGetTime()));
					numExplosions++;
					timer = maxTimer;
				}
				else
				{
					timer -= deltaTime;
				}
			}
			//std::cout << numExplosions << " ";
			boomShader.enable();
			for (int i = 0; i < numExplosions; i++)
			{
				float count = 0;
				if (lastTime > explosions[i].z + explosionTimer) 
				{
					numExplosions--;
					explosions.erase(explosions.begin() + i);
					i--;
				}
				else
				{
					explosion.bindBuffers();
					boomShader.AttributeBinding();
					explosion.renderBurst(boomShader, player, glm::vec3(explosions[i].x, explosions[i].y, 0.0f), explosions[i].z);
				}
			}
			
			//**************************************************************************************
			//********************************							  **************************
			//********************************     Finish Explosions      **************************
			//********************************							  **************************
			//**************************************************************************************
			shader.enable();
			shader.AttributeBinding();


			map.setPosition(player->getPosition());

			for (int i = 0; i < enemies.size(); i++) {
				int result = enemies[i]->controller(deltaTime, 0);//Checks turning status for all vehicles
				if (result == 1)//If we need to set a new flag
				{
					enemies[i]->setFlagIndex(enemies[i]->getFlagIndex() + 1);
					if (enemies[i]->getFlagIndex() >= map.getMaxFlags())
						enemies[i]->setFlagIndex(0);
						
					enemies[i]->setNextFlag(map.getFlag(enemies[i]->getFlagIndex()));
				}
			}

			map.render(shader);

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

