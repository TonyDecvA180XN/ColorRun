#include "Collidable.h"
#include "TypeCheck.h"

const Obstacle * const ObstacleType = nullptr;
const Pawn * const PawnType = nullptr;

Collidable::Collidable(Hub & InHub, EActorState InActorState) :
	Entity(InHub), ActorState(InActorState), LastStateChangeTime(InHub.GetClock())
{
}

Collidable::Collidable(Collidable && Other) :
	Entity(std::move(Other)),
	Collider(std::move(Other.Collider)),
	Color(Other.Color),
	ActorState(Other.ActorState),
	LastStateChangeTime(Other.LastStateChangeTime)
{
}

//void Collidable::Update(FLOAT PlayheadPosition)
//{
//	static constexpr FLOAT SpawnDistance = 24.f;
//	static constexpr FLOAT DespawnDistance = 16.f;
//	static constexpr FLOAT SpawnTime = 1.f;
//	static constexpr FLOAT DespawnTime = 1.f;
//
//	if (ActorType == EActorType::Obstacle)
//	{
//		switch (ActorState)
//		{
//			case Collidable::EActorState::Ready:
//			{
//				Collider->setIntersecting(FALSE);
//				SetUniformScale(*this, 0.f);
//				SetElevation(*this, 0.f);
//				FLOAT ObstacleDistance = std::abs(GetPosition() - PlayheadPosition);
//				if (ObstacleDistance < SpawnDistance)
//				{
//					ActorState = EActorState::Nascent;
//					SetUpdatedTime();
//				}
//				break;
//			}
//			case Collidable::EActorState::Nascent:
//			{
//				FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
//				TimeElapsed = std::min(TimeElapsed, SpawnTime);
//				FLOAT Growth = TimeElapsed / SpawnTime;
//				SetUniformScale(*this, Growth);
//				FLOAT Elevation = - 4.f * (Growth * Growth) + 3.f * Growth + 1.f;
//				SetElevation(*this, Elevation);
//				
//				if (LinkToHub->GetClock() >= LastStateChangeTime + SpawnTime)
//				{
//					ActorState = EActorState::Alive;
//					SetUpdatedTime();
//					SetUniformScale(*this, 1.f);
//					SetElevation(*this, 0.f);
//				}
//				break;
//			}
//			case Collidable::EActorState::Alive:
//			{
//				Collider->setIntersecting(TRUE);
//				FLOAT ObstacleDistance = std::abs(GetPosition() - PlayheadPosition);
//				if (ObstacleDistance >= DespawnDistance)
//				{
//					ActorState = EActorState::Dead;
//					SetUpdatedTime();
//				}
//				break;
//			}
//			case Collidable::EActorState::Dying:
//			{
//				Collider->setIntersecting(FALSE);
//				FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
//				SetElevation(*this, TimeElapsed * 10.f);
//				if (TimeElapsed >= DespawnTime)
//				{
//					ActorState = EActorState::Dead;
//					SetUpdatedTime();
//				}
//				break;
//			}
//			case Collidable::EActorState::Dead:
//			{
//				Collider->setIntersecting(FALSE);
//				break;
//			}
//			default:
//			{
//				break;
//			}
//		}
//	}
//}

void Collidable::SetMesh(std::string filename, std::string model, FLOAT CollisionSize)
{
	Entity::SetMesh(filename, model);
	Collider = Mesh->addCollider<w4::core::Sphere>(CollisionSize);
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
	Material->setParam("baseColor", Color);
}

void Collidable::OnCollision(const w4::core::Collider & SourceCollider, const w4::core::Collider & TargetCollider)
{
	w4::sptr<Collidable> Source = std::dynamic_pointer_cast<Collidable>(LinkToHub->ResolveEntity(SourceCollider.getParent()->getName()));
	w4::sptr<Collidable> Target = std::dynamic_pointer_cast<Collidable>(LinkToHub->ResolveEntity(TargetCollider.getParent()->getName()));

	if (IsType(&*Source, PawnType) && IsType(&*Target, ObstacleType))
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			if (Source->GetColor() == Target->GetColor())
			{
				Target->ActorState = EActorState::Dying;
				Target->LastStateChangeTime = LinkToHub->GetClock();
			}
			else
			{
				Source->ActorState = EActorState::Dying;
				Source->LastStateChangeTime = LinkToHub->GetClock();
			}
		}
	}
	if (IsType(&*Source, ObstacleType) && IsType(&*Target, PawnType))
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			if (Source->GetColor() == Target->GetColor())
			{
				Source->ActorState = EActorState::Dying;
				Source->LastStateChangeTime = LinkToHub->GetClock();
			}
			else
			{
				Target->ActorState = EActorState::Dying;
				Target->LastStateChangeTime = LinkToHub->GetClock();
			}
		}
	}
}

void Collidable::SetUniformScale(Entity & OutEntity, FLOAT Scale)
{
	OutEntity.GetNode()->setWorldScale({ Scale, Scale, Scale });

}

void Collidable::SetElevation(Entity & OutEntity, FLOAT Elevation)
{
	w4::math::vec3 Position = OutEntity.GetNode()->getWorldTranslation();
	Position.y = Elevation;
	OutEntity.GetNode()->setWorldTranslation(Position);
}

