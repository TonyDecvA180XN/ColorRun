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
	if (IsType(&*Source, PawnType) && IsType(&*Target, EnemyType))
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			Target->ActorState = EActorState::Dying;
			Target->LastStateChangeTime = LinkToHub->GetClock();

			Source->ActorState = EActorState::Dying;
			Source->LastStateChangeTime = LinkToHub->GetClock();
		}
	}
	if (IsType(&*Source, EnemyType) && IsType(&*Target, PawnType))
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			Source->ActorState = EActorState::Dying;
			Source->LastStateChangeTime = LinkToHub->GetClock();

			Target->ActorState = EActorState::Dying;
			Target->LastStateChangeTime = LinkToHub->GetClock();
		}
	}
}

void Collidable::SetUniformScale(FLOAT Scale)
{
	this->GetNode()->setWorldScale({ Scale, Scale, Scale });

}

void Collidable::SetElevation(FLOAT Elevation)
{
	w4::math::vec3 Position = this->GetNode()->getWorldTranslation();
	Position.y = Elevation;
	this->GetNode()->setWorldTranslation(Position);
}

