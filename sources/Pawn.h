#pragma once

#include <W4Framework.h>
#include "Collidable.h"

class Pawn : public Collidable
{
public:
	Pawn() = delete;
	Pawn(Hub & InHub, EActorState InActorState = EActorState::Alive);
	Pawn(const Pawn & Other) = delete;
	Pawn(Pawn && Other);
	Pawn & operator=(const Pawn & Other) = delete;
	Pawn & operator=(Pawn && Other) = delete;
	~Pawn() override = default;

	BOOL IsDead() const { return ActorState == EActorState::Dead; }

	void Update(FLOAT PlayheadPosition, w4::sptr<w4::render::Node> Playhead);
	
	void SetColor(const w4::math::vec4 InColor) override;
};
