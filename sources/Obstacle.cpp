#include "Obstacle.h"

Obstacle::Obstacle(Obstacle && Other) :
	Collidable(std::move(Other))
{
}

void Obstacle::Update(FLOAT PlayheadPosition)
{
	static constexpr FLOAT SpawnDistance = 24.f;
	static constexpr FLOAT DespawnDistance = 16.f;
	static constexpr FLOAT SpawnTime = 1.f;
	static constexpr FLOAT DespawnTime = 1.f;

	switch (ActorState)
	{
		case Collidable::EActorState::Ready:
		{
			Collider->setIntersecting(FALSE);
			SetUniformScale(0.f);
			SetElevation(0.f);
			FLOAT ObstacleDistance = std::abs(GetPosition() - PlayheadPosition);
			if (ObstacleDistance < SpawnDistance)
			{
				ActorState = EActorState::Nascent;
				SetUpdatedTime();
			}
			break;
		}
		case Collidable::EActorState::Nascent:
		{
			FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
			TimeElapsed = std::min(TimeElapsed, SpawnTime);
			FLOAT Growth = TimeElapsed / SpawnTime;
			SetUniformScale(Growth);
			FLOAT Elevation = - 4.f * (Growth * Growth) + 3.f * Growth + 1.f;
			SetElevation(Elevation);
				
			if (LinkToHub->GetClock() >= LastStateChangeTime + SpawnTime)
			{
				ActorState = EActorState::Alive;
				SetUpdatedTime();
				SetUniformScale(1.f);
				SetElevation(0.f);
			}
			break;
		}
		case Collidable::EActorState::Alive:
		{
			Collider->setIntersecting(TRUE);
			FLOAT ObstacleDistance = std::abs(GetPosition() - PlayheadPosition);
			if (ObstacleDistance >= DespawnDistance)
			{
				ActorState = EActorState::Dead;
				SetUpdatedTime();
			}
			break;
		}
		case Collidable::EActorState::Dying:
		{
			Collider->setIntersecting(FALSE);
			FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
			SetElevation(TimeElapsed * 10.f);
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
