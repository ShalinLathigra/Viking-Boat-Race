#include "Opponent.h"



Opponent::Opponent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h)
	:Car(entityPos,entityScale, entityRotationAmount, entityTexture, entityNumElements, m, h)
{
	cameraPos = entityPos;
	speed = 110;
}


Opponent::~Opponent()
{
}

void Opponent::update(double deltaTime) {
	Car::update(deltaTime);

	cameraPos += velocity;
	//std::cout << "VELOCITY X: " << velocity.x << std::endl;
	//std::cout << "VELOCITY Y: " << velocity.y << std::endl;
	//std::cout << "ENEMY POS X: " << cameraPos.x << std::endl;
	//std::cout << "ENEMY POS Y: " << cameraPos.y << std::endl;
}

void Opponent::controller(float deltaTime, float skillMod)
{
	//std::cout << speed << std::endl;
	//drive(deltaTime, 1);
	turn(1, deltaTime);
}

void Opponent::SetPosition(glm::vec3 pos)
{
	position = -2.0f * pos + cameraPos;
}
