#include "Opponent.h"



Opponent::Opponent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h, glm::vec3 firstFlag, float setMAX_SPEED)
	:Car(entityPos,entityScale, entityRotationAmount, entityTexture, entityNumElements, m, h)
{
	cameraPos = entityPos;
	speed = 0;
	flagIndex = 0;
	nextFlag = firstFlag;
	MAX_SPEED = setMAX_SPEED;
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

int Opponent::controller(float deltaTime, float skillMod)
{
	//std::cout << speed << std::endl;
	float forceX = cos(rotationAmount *(PI / 180.0f));
	float forceY = sin(rotationAmount *(PI / 180.0f));
	glm::vec3 currDir = glm::vec3(forceX, forceY, 0);
	
	//turn(1, deltaTime);

	if (hypot(hypot(nextFlag.x - position.x, nextFlag.y - position.y), nextFlag.z - position.z) < 0.1)
		return 1;
	//std::cout << hypot(hypot(nextFlag.x - position.x, nextFlag.y - position.y), nextFlag.z - position.z)<<std::endl;
	//std::cout << rotationAmount << std::endl;
	glm::vec3 newDir = nextFlag - position;
	currDir = glm::normalize(currDir);
	newDir = glm::normalize(newDir);
	//std::cout << "currDir LENGTH = " << glm::length(currDir) << std::endl;
	//std::cout << "newDir LENGTH = " << glm::length(newDir) << std::endl;
	if (newDir.x == 0)
		newDir.x += 0.001;
	float newAngle = atan2(newDir.y, newDir.x) * (180 / 3.141592653589793238463);
	std::cout << "NewAngle: " << newAngle << std::endl;
	//std::cout << "Angle: " << newAngle << std::endl;
	//std::cout << "currDir.x = " << currDir.x << std::endl;
	//std::cout << "newDir.x = " << newDir.x << std::endl;
	//std::cout << "currDir.y = " << currDir.y << std::endl;
	//std::cout << "newDir.y = " << newDir.y << std::endl;
	if (newDir.y < 0)
		newAngle += 360;

	float result = (rotationAmount + 360.0f);
	result -= newAngle;
	if (result >= 360)
		result -= 360;

	std::cout << "Angle: " << rotationAmount << std::endl;
	if (result == 180)
		turn(1, deltaTime);
	else if (result < 180)
		turn(2, deltaTime);
	else
		turn(1, deltaTime);

	drive(deltaTime, 1);
	return 0;
}

void Opponent::SetPosition(glm::vec3 pos)
{
	position = -2.0f * pos + cameraPos;
}

void Opponent::setNextFlag(glm::vec3 flag)
{
	nextFlag = flag;
}

void Opponent::setFlagIndex(int fIndex)
{
	flagIndex = fIndex;
}

glm::vec3 Opponent::getNextFlag()
{
	return nextFlag;
}

int Opponent::getFlagIndex()
{
	return flagIndex;
}