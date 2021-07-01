#include "Enemy.h"

Enemy::Enemy(Hub & InHub, EActorState InActorState) :
	Collidable(InHub, InActorState)
{
}

Enemy::Enemy(Enemy && Other) :
	Collidable(std::move(Other))
{
}

void Enemy::Update(FLOAT PlayheadPosition)
{
	SetElevation(1.f + 0.25f * std::sinf(0.5f * PlayheadPosition));
}
