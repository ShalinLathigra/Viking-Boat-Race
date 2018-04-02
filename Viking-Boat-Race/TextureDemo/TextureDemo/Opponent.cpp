#include "Opponent.h"



Opponent::Opponent(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements, float m, int h, glm::vec3 firstFlag, float setMAX_SPEED)
	:Car(entityPos,entityScale, entityRotationAmount, entityTexture, entityNumElements, m, h)
{
	cameraPos = entityPos;
	speed = 0;
	flagIndex = 0;
	nextFlag = firstFlag;
	MAX_SPEED = setMAX_SPEED;
	natSteerDir = 0;
	natSteerTimer = 0;
}


Opponent::~Opponent()
{
}

void Opponent::update(double deltaTime) {
	Car::update(deltaTime);

	cameraPos += velocity;
}

int Opponent::controller(float deltaTime, float skillMod)
{
	float forceX = cos(rotationAmount *(PI / 180.0f));
	float forceY = sin(rotationAmount *(PI / 180.0f));
	glm::vec3 currDir = glm::vec3(forceX, forceY, 0);

	if (hypot(hypot(nextFlag.x - position.x, nextFlag.y - position.y), nextFlag.z - position.z) < 0.1)
		return 1;
	glm::vec3 newDir = nextFlag - position;
	currDir = glm::normalize(currDir);
	newDir = glm::normalize(newDir);
	if (newDir.x == 0)
		newDir.x += 0.001;
	float newAngle = atan2(newDir.y, newDir.x) * (180 / 3.141592653589793238463);
	if (newDir.y < 0)
		newAngle += 360;

	float result = (rotationAmount + 360.0f);
	result -= newAngle;
	if (result >= 360)
		result -= 360;

	if (result == 180 && natSteerDir == 0)
		turn(1, deltaTime);
	else if (result < 180 && natSteerDir == 0)
		turn(2, deltaTime);
	else if (natSteerDir == 0)
		turn(1, deltaTime);

	//Basically adding some slight turning in natural movement to add realism//
	if (natSteerDir == 0)
	{
		if (natSteerTimer == 0)
		{
			int chance = rand() % 200;
			std::cout << chance << std::endl;
			if (chance == 1)
			{
				natSteerDir = 1;
				natSteerTimer = 30;
			}
			else if (chance == 2)
			{
				natSteerDir = 2;
				natSteerTimer = 30;
			}
		}
	}
	else
	{
		std::cout << "YEEEEEEEEEET" << std::endl;
		if (natSteerDir == 1)
			turn(1, deltaTime);
		else
			turn(2, deltaTime);
		natSteerTimer--;
		if (natSteerTimer == 0)
			natSteerDir = 0;
	}

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

int Opponent::getCurrentLap()
{
	return currentLap;
}

void Opponent::setCurrentLap(int lap)
{
	currentLap = lap;
}