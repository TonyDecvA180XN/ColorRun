#pragma once

#include <W4Framework.h>
#include "Collidable.h"

class Obstacle : public Collidable
{
public:
	Obstacle() = delete;
	Obstacle(Hub & InHub, EActorState InActorState = EActorState::Ready) : Collidable(InHub, InActorState) {};
	Obstacle(const Obstacle & Other) = delete;
	Obstacle(Obstacle && Other);
	Obstacle & operator=(const Obstacle & Other) = delete;
	Obstacle & operator=(Obstacle && Other) = delete;
	~Obstacle() override = default;

	void Update(FLOAT PlayheadPosition) override;
};
