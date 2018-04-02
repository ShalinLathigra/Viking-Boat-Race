#include "Arrow.h"

Arrow::Arrow(glm::vec3 pos, glm::vec3 scale, float rot, GLint entityNumElements, float spd, int dmg, float x, float y, int idNum)
	:GameEntity(pos, scale, rot, entityNumElements)
{
	speed = spd;
	glGenTextures(1, tex);
	setthisTexture(tex[0], "arrow.png");
	id = idNum;
	std::cout << id << std::endl;
}


Arrow::~Arrow()
{
}
float Arrow::rotation() { return rotationAmount; }

void Arrow::setthisTexture(GLuint w, char * fname)
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

void Arrow::update(double deltaTime)
{
	position.x += speed*cos(rotationAmount*PI / 180.0f)*deltaTime;
	position.y += speed*sin(rotationAmount*PI / 180.0f)*deltaTime;

}

/*
std::vector<Car*> Arrow::checkCollisions(std::vector<Car*> cars) {
for (int i = 0; i < cars.size(); i++) {
glm::vec3 L1, L2, L3, L4;
float forceX = cos(rotationAmount *(PI / 180.0f));
float forceY = sin(rotationAmount *(PI / 180.0f));
L1 = glm::vec3(cos((rotationAmount + 90)*(PI / 180.0f)), sin((rotationAmount + 90)*(PI / 180.0f)), 0);
L2 = glm::vec3(forceX, forceY, 0);
glm::vec3 Ax, Ay, Bx, By;
L3 = glm::vec3(cos((cars[i]->rotationAmount + 90)*(PI / 180.0f)), sin((cars[i]->rotationAmount + 90)*(PI / 180.0f)), 0);
L4 = glm::vec3(cos((cars[i]->rotationAmount + 90)*(PI / 180.0f)), sin((cars[i]->rotationAmount)*(PI / 180.0f)), 0);
Ax = L1;
Ay = L2;
Bx = L3;
By = L4;
bool comp1 = abs(glm::dot((cars[i]->position - position), L1)) > (abs(glm::dot(xSize*Ax, Ax)) + abs(glm::dot(ySize*Ay, Ax)) + abs(glm::dot(0.05f*Bx, Ax)) + abs(glm::dot(0.05f*By, Ay)));
bool comp2 = abs(glm::dot((cars[i]->position - position), L2)) > (abs(glm::dot(xSize*Ax, Ay)) + abs(glm::dot(ySize*Ay, Ay)) + abs(glm::dot(0.05f*Bx, Ay)) + abs(glm::dot(0.05f*By, Ay)));
bool comp3 = abs(glm::dot((cars[i]->position - position), L3)) > (abs(glm::dot(xSize*Ax, Bx)) + abs(glm::dot(ySize*Ay, Bx)) + abs(glm::dot(0.05f*Bx, Bx)) + abs(glm::dot(0.05f*By, Bx)));
bool comp4 = abs(glm::dot((cars[i]->position - position), L4)) > (abs(glm::dot(xSize*Ax, By)) + abs(glm::dot(ySize*Ay, By)) + abs(glm::dot(0.05f*Bx, By)) + abs(glm::dot(0.05f*By, By)));

if ((!comp1 && !comp2 && !comp3 && !comp4)) {
//there was a collision that was detected properly
cars.erase(cars.begin()+i);
std::cout << "Collision detected properly" << std::endl << "comp1: " << comp1 << " comp2: " << comp2 << " comp3: " << comp3 << " comp4: " << comp4 << std::endl;
}

}
return cars;
}
*/