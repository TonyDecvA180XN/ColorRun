#pragma once

#include <W4Framework.h>
#include "Entity.h"
#include "Hub.h"

class Collidable : public Entity
{
public:
	Collidable() = delete;
	Collidable(Hub & Hub);
	Collidable(const Collidable & Other) = delete;
	Collidable(Collidable && Other);
	Collidable & operator=(const Collidable & Other) = delete;
	Collidable & operator=(Collidable && Other) = delete;
	~Collidable() override;

private:
	struct Section
	{
		FLOAT Left { 0.f };
		FLOAT Right { 0.f };
	};
};
