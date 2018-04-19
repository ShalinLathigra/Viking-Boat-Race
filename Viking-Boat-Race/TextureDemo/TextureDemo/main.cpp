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
//#include <GL/fgl.h>
//#include <GL/fglut.h>
//#include <GL/tube.h>
//#include <GL/fglu.h>
#include <math.h>
#include "Ballista.h"
#include "Arrow.h"
#include "Shader.h"
#include "Window.h"
#include "Map.h"
#include "Car.h"
#include "Opponent.h"
#include "ParticleSystem.h"


//RACE STATES
#define MENU 0
#define RACING 1
#define SHOP 2


using namespace std;
// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Agent Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 800;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.2);

// Global texture info
GLuint tex[10];

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
	glGenTextures(10, tex);
	setthisTexture(tex[0], "mapImage0.png");
	setthisTexture(tex[1], "Boat.png");
	setthisTexture(tex[2], "otherBoat.png");
	setthisTexture(tex[3], "orb.png");
	setthisTexture(tex[4], "fireEffect.png");
	setthisTexture(tex[5], "smoke.png");
	setthisTexture(tex[6], "MainMenu.png");
	setthisTexture(tex[7], "arrow.png");
	setthisTexture(tex[8], "ballistaA1.png");
	setthisTexture(tex[9], "mapImage1.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
}



// Main function that builds and runs the game
int main(void){
	float PI = 3.1415926535;
    try {

		// Set seed for random
		srand(time(NULL));

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
		ParticleSystem exhaust( .3, tex[5] );
		ParticleSystem windStream(.1f, tex[3]);

		// Setup game objects
		Map map0 = Map::Map(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(18.0f, 9.0f, 1.0f), 0.0f, tex[0], size, "map0.txt", 0);
		Map map1 = Map::Map(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(18.0f, 9.0f, 1.0f), 0.0f, tex[9], size, "map1.txt", 1);
		
		Map map = map1;

		Car* player = new Car(map.getStartPosition(4), glm::vec3(0.15f, 0.15f, 0.1f), 90.0f, tex[1], size, 12, 10,tex[7]);
		Opponent* enemy0 = new Opponent(map.getStartPosition(0), glm::vec3(0.15f, 0.15f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 20, tex[7]);
		Opponent* enemy1 = new Opponent(map.getStartPosition(1), glm::vec3(0.15f, 0.15f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 20, tex[7]);
		Opponent* enemy2 = new Opponent(map.getStartPosition(2), glm::vec3(0.15f, 0.15f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 20, tex[7]);
		Opponent* enemy3 = new Opponent(map.getStartPosition(3), glm::vec3(0.15f, 0.15f, 0.1f), 90.0f, tex[2], size, 12, 10, map.getFlag(0), 20, tex[7]);
		Ballista ballista = Ballista(tex[8], size);


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
		std::vector<Arrow> arrows;
		bool space = false;

		int mode = MENU;
		int raceOver = 0;
		int weightUpgrade = 1;
		int speedUpgrade = 1;
		int placeSystem = 1;
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(glm::vec3(0.0f, .1f, 0.0f));

			// Select proper shader program to use
			shader.enable();
			shader.AttributeBinding();
			// Get mouse position relative to screenspace [-1, 1]
			//if (agent.getState() != Agent::State::WANDER) {
			double mouseX, mouseY;
			glfwGetCursorPos(window.getWindow(), &mouseX, &mouseY);
			float screenSpaceMouseX = (mouseX / window_width_g) * 2 - 1;
			float screenSpaceMouseY = -((mouseY / window_height_g) * 2 - 1);
			float mouseAngle = -atan2(screenSpaceMouseX, screenSpaceMouseY)* 180.0f / PI;
			//	agent.setTargetPos(glm::vec3(screenSpaceMouseX, screenSpaceMouseY, 0.0f));
			//}
			// Calculate delta time
			if (mode == MENU)
			{
				player->position = map.getStartPosition(4);
				enemies[0]->position = map.getStartPosition(0);
				enemies[3]->position = map.getStartPosition(3);
				enemies[2]->position = map.getStartPosition(2);
				enemies[1]->position = map.getStartPosition(1);

				if (glfwGetKey(window.getWindow(), GLFW_KEY_P) == GLFW_PRESS)
				{
					mode = RACING;
				}

				// Bind the entities texture
				glBindTexture(GL_TEXTURE_2D, tex[6]);

				// Setup the transformation matrix for the shader 
				glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
				glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 1));
				glm::mat4 transformationMatrix = translationMatrix * scaleMatrix;
				shader.setUniformMat4("x", transformationMatrix);

				// Draw the entity
				glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

				glfwPollEvents();
				// Push buffer drawn in the background onto the display
				glfwSwapBuffers(window.getWindow());
			}


			else if (mode == RACING)
			{
				double currentTime = glfwGetTime();
				double deltaTime = currentTime - lastTime;
				lastTime = currentTime;

				if (!raceOver) //Turn off player movement when race over
				{
					//PLAYER INPUT
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

					if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE&&space) {
						space = false;
						//std::cout << player->id << std::endl;
						player->attack(ballista.rotationAmount, arrows);
					}

					if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
						space = true;
					}

					map.checkProgress(player);
					if (map.checkFinish(player) > 0) 
					{
						if (player->place == 0) {
							player->place = placeSystem;
							placeSystem++;
							std::cout << "Player got " << player->place << " place." << std::endl;
							player->money += (int)(2000.0f*(5.0f/(float)player->place));
							if (player->place == 5) { player->money -= 3000; }
							mode = SHOP; 
						}
					}

					ballista.rotationAmount = mouseAngle-90;
					ballista.render(shader, player->transform);
					//ENEMY UPDATING
					for (int i = 0; i < enemies.size(); i++) {
						bool colliding = enemies[i]->checkArrows(arrows);
						if (colliding)
						{
							enemies[i]->setNatSteerDir(1);
							enemies[i]->setNatSteerTimer(00);
						}

						int result = enemies[i]->controller(deltaTime, 0);//Checks turning status for all vehicles
						if (result == 1)//If we need to set a new flag
						{
							enemies[i]->setFlagIndex(enemies[i]->getFlagIndex() + 1);
							if (enemies[i]->getFlagIndex() >= map.getMaxFlags())
							{
								enemies[i]->setFlagIndex(0);
								enemies[i]->setCurrentLap(enemies[i]->getCurrentLap() + 1);

								std::cout << enemies[i]->getCurrentLap() << ", " << map.getNumLaps() << ": " << (enemies[i]->getCurrentLap() > map.getNumLaps()) << std::endl;
								if (enemies[i]->getCurrentLap() > map.getNumLaps())
								{
									if (enemies[i]->place == 0) {
										enemies[i]->place = placeSystem;
										placeSystem += 1;
										//raceOver = 1;
										std::cout << "ENEMY " << i << " got " << enemies[i]->place << " place." << std::endl;
										if (placeSystem > 5) { mode = SHOP; }
									}

								}
							}
							//This all just adds slight variation to the points the AI are targeting so they don't follow each other//
							double diffx = rand() % 30 - 15;
							double diffy = rand() % 30 - 15;
							glm::vec3 newFlag = map.getFlag(enemies[i]->getFlagIndex());
							newFlag.x += diffx / 100;
							newFlag.y += diffy / 100;
							enemies[i]->setNextFlag(newFlag);
						}
					}
				}
				for (int i = 0; i < allCars.size(); i++) {
					allCars[i]->checkCollisions(allCars, deltaTime);
					allCars[i]->boxCollisions(allCars, deltaTime);
					//get property + setValues
					if (map.getPropertyUnder(allCars[i]) == Tile::TileProp::WALL) {
						map.calculateCarCollisions(allCars[i]);
					}
					allCars[i]->update(deltaTime);
					allCars[i]->render(shader, player->getPosition());
				}
				for (int i = 0; i < arrows.size(); i++) {
					arrows[i].update((double)deltaTime);
					arrows[i].render(shader, player->getPosition());
				}
				partShader.enable();
				exhaust.bindBuffers();
				partShader.AttributeBinding();

				for (std::vector<Car *>::iterator c = allCars.begin(); c != allCars.end(); ++c)
				{
					exhaust.renderTrail(partShader, *c, player->getPosition());
				}

				windStream.bindBuffers();
				partShader.AttributeBinding();
				for (std::vector<Car *>::iterator c = allCars.begin(); c != allCars.end(); ++c)
				{
					if ((*c)->isJumping())
					{
						windStream.renderWind(partShader, *c, player->getPosition());
					}
				}
				shader.enable();
				shader.AttributeBinding();


				map.setPosition(player->getPosition());

				//std::cout << "Player Pos: " << player->getPosition().x << ", " << player->getPosition().y << std::endl;
				

				map.render(shader);

				// Update other events like input handling
				glfwPollEvents();
				// Push buffer drawn in the background onto the display
				glfwSwapBuffers(window.getWindow());
			}else if (mode == SHOP) {
				string response;
				std::cout << "Congratulations on finishing the race!" << std::endl;
				std::cout << "You have: " << player->money << "$" << std::endl;
				cout << "Upgrades (type yes or no to answer):" << endl;
				cout << "Increase weight ($"<<weightUpgrade*5000<<"):" << endl;
				player->position = map.getStartPosition(4);
				enemies[0]->position = map.getStartPosition(0);
				enemies[3]->position = map.getStartPosition(3);
				enemies[2]->position = map.getStartPosition(2);
				enemies[1]->position = map.getStartPosition(1);
				player->shots = 0;
				for (int i = 0; i < enemies.size(); i++) {
					enemies[i]->setCurrentLap(0);
					enemies[i]->setFlagIndex(0);
					enemies[i]->setNextFlag(map.getFlag(0));
				}
				for (int i = 0; i < allCars.size(); i++) {
					allCars[i]->speed = 0;
					allCars[i]->rotationAmount = 90;
					//allCars[i]->velocity = glm::vec3(0, 0, 0);
					allCars[i]->update(0.0f);
					allCars[i]->render(shader, player->getPosition());
				}
				for (int i = 0; i < allCars.size(); i++) {
					allCars[i]->place = 0;
				}



				while (response != "yes"&&response != "no") {
					getline(cin, response);
				}
				if (response == "yes"&&player->money >= weightUpgrade*5000) {
					weightUpgrade += 1.5;
					player->mass *= 1.5;
				}
				response = "";
				cout << "Increase speed ($" << speedUpgrade * 5000 << "):" << endl;
				while (response != "yes"&&response != "no") {
					getline(cin, response);
				}
				if (response == "yes"&&player->money >= speedUpgrade * 5000) {
					speedUpgrade += 1;
					player->MAX_SPEED *= 1.2;
				}
				mode = MENU;
			}
		}
    }
    catch (std::exception &e){
		// print exception and sleep so error can be read
        PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
    }

    return 0;
}

