#pragma once
#include "GameEntity.h"
class Ballista :public GameEntity
{
public:
	void update(double) override;
	Ballista(GLint entityTexture, GLint entityNumElements);
	~Ballista();
};

