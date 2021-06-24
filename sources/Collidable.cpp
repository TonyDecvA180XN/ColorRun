#include "Collidable.h"

void SetUniformScale(Entity & OutEntity, FLOAT Scale)
{
	OutEntity.Transform()->setWorldScale({ Scale, Scale, Scale });
}

void SetElevation(Entity & OutEntity, FLOAT Elevation)
{
	w4::math::vec3 Position = OutEntity.Transform()->getWorldTranslation();
	Position.y = Elevation;
	OutEntity.Transform()->setWorldTranslation(Position);
}

Collidable::Collidable(Hub & InHub, EActorType InActorType, EActorState InActorState) :
	Entity(InHub), ActorType(InActorType), ActorState(InActorState), LastStateChangeTime(InHub.GetClock())
{
}

Collidable::Collidable(Collidable && Other) :
	Entity(std::move(Other))
{
}


Collidable::~Collidable()
{
}

void Collidable::Update(FLOAT PlayheadPosition)
{
	static constexpr FLOAT SpawnDistance = 24.f;
	static constexpr FLOAT DespawnDistance = 16.f;
	static constexpr FLOAT SpawnTime = 1.f;
	static constexpr FLOAT DespawnTime = 1.f;

	if (ActorType == EActorType::Obstacle)
	{
		switch (ActorState)
		{
			case Collidable::EActorState::Ready:
			{
				Collider->setIntersecting(FALSE);
				SetUniformScale(*this, 0.f);
				SetElevation(*this, 0.f);
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
				FLOAT TimeElapsed = std::abs(hub->GetClock() - LastStateChangeTime);
				TimeElapsed = std::min(TimeElapsed, SpawnTime);
				FLOAT Growth = TimeElapsed / SpawnTime;
				SetUniformScale(*this, Growth);
				FLOAT Elevation = - 4.f * (Growth * Growth) + 3.f * Growth + 1.f;
				SetElevation(*this, Elevation);
				
				if (hub->GetClock() >= LastStateChangeTime + SpawnTime)
				{
					ActorState = EActorState::Alive;
					SetUpdatedTime();
					SetUniformScale(*this, 1.f);
					SetElevation(*this, 0.f);
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
				FLOAT TimeElapsed = std::abs(hub->GetClock() - LastStateChangeTime);
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
}

void Collidable::SetMesh(std::string filename, std::string model, FLOAT CollisionSize)
{
	Entity::SetMesh(filename, model);
	Collider = m_mesh->addCollider<w4::core::Sphere>(CollisionSize);
	Collider->setIntersectionBeginCallback(
		[this](const w4::core::CollisionInfo & Info)
		{
			OnCollision(*(Info.source), *(Info.target));
		});
	Collider->setIntersecting(FALSE);
}

void Collidable::SetColor(const w4::math::vec4 InColor)
{
	Color = InColor;
	m_material->setParam("baseColor", Color);
}

void Collidable::OnCollision(const w4::core::Collider & SourceCollider, const w4::core::Collider & TargetCollider)
{
	w4::sptr<Collidable> Source = std::dynamic_pointer_cast<Collidable>(hub->ResolveEntity(SourceCollider.getParent()->getName()));
	w4::sptr<Collidable> Target = std::dynamic_pointer_cast<Collidable>(hub->ResolveEntity(TargetCollider.getParent()->getName()));

	if (Source->ActorType == EActorType::MainCharacter && Target->ActorType == EActorType::Obstacle)
	{
		// To be decided
		if (Target->ActorState == EActorState::Alive && Source->GetColor() == Target->GetColor())
		{
			Target->ActorState = EActorState::Dying;
			Target->LastStateChangeTime = hub->GetClock();
		}
	}
	if (Target->ActorType == EActorType::Obstacle && Source->ActorType == EActorType::MainCharacter)
	{
		// To be decided
		if (Source->ActorState == EActorState::Alive && Source->GetColor() == Target->GetColor())
		{
			Source->ActorState = EActorState::Dying;
			Source->LastStateChangeTime = hub->GetClock();
		}
	}
	if (Source->ActorType == EActorType::Pawn && Target->ActorType == EActorType::Obstacle)
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			if (Source->GetColor() != Target->GetColor())
			{
				Source->ActorState = EActorState::Dying;
				Source->SetUpdatedTime();
			}
		}
	}
	if (Source->ActorType == EActorType::Obstacle && Target->ActorType == EActorType::Pawn)
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			if (Target->GetColor() != Source->GetColor())
			{
				Target->ActorState = EActorState::Dying;
				Target->SetUpdatedTime();
			}

		}
	}
	// To be decided
}

