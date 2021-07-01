#pragma once

#include <W4Framework.h>
#include "Collidable.h"

class Enemy : public Collidable
{
public:
	Enemy() = delete;
	Enemy(Hub & InHub, EActorState InActorState = EActorState::Alive);
	Enemy(const Enemy & Other) = delete;
	Enemy(Enemy && Other);
	Enemy & operator=(const Enemy & Other) = delete;
	Enemy & operator=(Enemy && Other) = delete;
	~Enemy() override = default;

	//BOOL IsDead() const { return ActorState == EActorState::Dead; }

	void Update(FLOAT PlayheadPosition) override;
	
};
