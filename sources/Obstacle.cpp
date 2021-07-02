#include "Obstacle.h"

Obstacle::Obstacle(Hub & InHub, EActorState InActorState, EMeshType InMeshType) :
	Collidable(InHub, InActorState, InMeshType)
{
}

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
			SetElevation(0.f + 0.2f);
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
			SetUniformScale(Growth / 40);
			FLOAT Elevation = - 4.f * (Growth * Growth) + 3.f * Growth + 1.f;
			SetElevation(Elevation + 0.25);
				
			if (LinkToHub->GetClock() >= LastStateChangeTime + SpawnTime)
			{
				ActorState = EActorState::Alive;
				SetUpdatedTime();
				SetUniformScale(1.f / 40);
				SetElevation(0.f + 0.25f);
			}
			break;
		}
		case Collidable::EActorState::Alive:
		{
			Collider->setIntersecting(TRUE);
			FLOAT ObstacleDistance = std::abs(GetPosition() - PlayheadPosition);
			if (ShouldDie)
			{
				ActorState = EActorState::Dying;
				SetUpdatedTime();
			}
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
			FLOAT Elevation = 0.25 + 2 * TimeElapsed - 5 * TimeElapsed * TimeElapsed;
			SetElevation(Elevation);
			GetNode()->rotateLocal({ TimeElapsed / 10, 0, 0 });
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

void Obstacle::SetColor(const w4::math::vec4 InColor)
{
	Color = InColor;
	//Material->setParam("baseColor", Color);
	Material->setTexture(w4::resources::TextureId::TEXTURE_0, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, Color)));
}
