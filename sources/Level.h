#pragma once

#include <W4Framework.h>
#include "Type.h"
#include "Road.h"
#include "Entity.h"
#include "Obstacle.h"

class Level
{
public:

	Level() = delete;
	Level(const Level & other) = delete;
	Level(Level && other) = default;
	Level(const Hub & InHub, INDEX Number);

	void Update(FLOAT PlayheadPosition);

	FLOAT GetLength() const { return this->Road->GetLength(); };

private:
	void CreateLevel1(const Hub & InHub);

	std::vector<Entity> Entities {};
	w4::sptr<Road> Road {};
};
