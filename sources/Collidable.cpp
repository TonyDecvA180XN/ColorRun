#include "Collidable.h"

Collidable::Collidable(Collidable && Other) :
	Entity(std::move(Other))
{
}

Collidable::Collidable(Hub & Hub) :
	Entity(Hub)
{
}

Collidable::~Collidable()
{
}
