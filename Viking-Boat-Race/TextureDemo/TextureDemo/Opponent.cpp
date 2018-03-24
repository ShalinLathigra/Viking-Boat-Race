#include "Opponent.h"



Opponent::Opponent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h, glm::vec3 firstFlag)
	:Car(entityPos,entityScale, entityRotationAmount, entityTexture, entityNumElements, m, h)
{
	cameraPos = entityPos;
	speed = 6;
	flagIndex = 0;
	nextFlag = firstFlag;
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
	//drive(deltaTime, 1);
	float forceX = cos(rotationAmount *(PI / 180.0f));
	float forceY = sin(rotationAmount *(PI / 180.0f));
	glm::vec3 currDir = glm::vec3(forceX, forceY, 0);

	if (hypot(hypot(nextFlag.x - position.x, nextFlag.y - position.y), nextFlag.z - position.z) < 0.01)
		return 1;
	//std::cout << hypot(hypot(nextFlag.x - position.x, nextFlag.y - position.y), nextFlag.z - position.z)<<std::endl;
	//std::cout << rotationAmount << std::endl;
	glm::vec3 newDir = nextFlag - position;
	newDir = glm::normalize(newDir);
	if (rotationAmount > 0 && rotationAmount < 90)
	{
		//std::cout << "FIRST QUADRANT" << std::endl;
		if (currDir.x < newDir.x && currDir.y > newDir.y)
			turn(2, deltaTime);
		else if (currDir.x > newDir.x && currDir.y < newDir.y)
			turn(1, deltaTime);
	}
	else if (rotationAmount > 90 && rotationAmount < 180)
	{
		//std::cout << "SECOND QUADRANT" << std::endl;
		if (currDir.x < newDir.x && currDir.y > newDir.y)
			turn(2, deltaTime);
		else if (currDir.x > newDir.x && currDir.y < newDir.y)
			turn(1, deltaTime);
	}
	else if (rotationAmount > 180 && rotationAmount < 270)
	{
		//std::cout << "THIRD QUADRANT" << std::endl;
		if (currDir.x < newDir.x && currDir.y > newDir.y)
			turn(2, deltaTime);
		else if (currDir.x > newDir.x && currDir.y < newDir.y)
			turn(1, deltaTime);
	}
	else
	{
		//std::cout << "FOURTH QUADRANT"<<std::endl;
		if (currDir.x > newDir.x && currDir.y < newDir.y)
			turn(2, deltaTime);
		else if (currDir.x < newDir.x && currDir.y > newDir.y)
			turn(1, deltaTime);
	}
	std::cout << "Current Waypoint: " << nextFlag.x << "," << nextFlag.y << std::endl;
	drive(deltaTime, 1);
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