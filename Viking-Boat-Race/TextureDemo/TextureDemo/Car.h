#pragma once
#include "GameEntity.h"
#include <vector>
class Car :
	public GameEntity
{
public:
	static int carNum;
	glm::vec3 momentum;
	int id;
	Car(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements,float m, int h);
	void drive(double deltaTime,int dir);
	void turn(int d,float deltaTime);
	void attack(int weaponType);
	bool equals(Car*);
	const float MAX_VELOCITY = 175;
	void update(double deltaTime) override;
	float speed;
	void checkCollisions(std::vector<Car*>, float);
	void render(Shader & shader, glm::vec3 offset);
	void boxCollisions(std::vector<Car*>, float);
	inline glm::vec3 getVel() { return velocity; }
	void applyImpulse(glm::vec3 impulse);

	inline void setPosition(glm::vec3 pos) { position = pos; }


	float speedMod;	//Controls the different surfaces
protected:
	glm::vec3 velocity;
	glm::vec3 forces;
	float rotationSpeed;
	int health;
	float mass;
	
};

