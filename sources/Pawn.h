#pragma once

#include <W4Framework.h>
#include "Collidable.h"

class Pawn : public Collidable
{
public:
	Pawn() = delete;
	Pawn(Hub & InHub, EActorState InActorState = EActorState::Alive) : Collidable(InHub, InActorState) {};
	Pawn(const Pawn & Other) = delete;
	Pawn(Pawn && Other);
	Pawn & operator=(const Pawn & Other) = delete;
	Pawn & operator=(Pawn && Other) = delete;
	~Pawn() override = default;

	void Update(FLOAT PlayheadPosition) override;
};
