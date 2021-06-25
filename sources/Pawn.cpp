#include "Pawn.h"

Pawn::Pawn(Pawn && Other) :
	Collidable(std::move(Other))
{
}

void Pawn::Update(FLOAT PlayheadPosition)
{
	static constexpr FLOAT DespawnTime = 1.f;

	switch (ActorState)
	{
		case Collidable::EActorState::Ready:
		{
			
			break;
		}
		case Collidable::EActorState::Nascent:
		{
			
			break;
		}
		case Collidable::EActorState::Alive:
		{
			Collider->setIntersecting(TRUE);
			break;
		}
		case Collidable::EActorState::Dying:
		{
			Collider->setIntersecting(FALSE);
			FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
			SetElevation(*this, TimeElapsed * 10.f);
			if (TimeElapsed >= DespawnTime)
			{
				ActorState = EActorState::Dead;
				SetUpdatedTime();
			}
			break;
		}
		case Collidable::EActorState::Dead:
		{
			Collider->setIntersecting(FALSE);
			break;
		}
		default:
		{
			break;
		}
	}
}
