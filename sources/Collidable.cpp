#include "Collidable.h"
#include "TypeCheck.h"

const Obstacle * const ObstacleType = nullptr;
const Pawn * const PawnType = nullptr;

std::string c2s(w4::math::vec4 c)
{
	return std::to_string(c.x) + ", "s + std::to_string(c.y) + ", "s + std::to_string(c.z);
}

Collidable::Collidable(Hub & InHub, EActorState InActorState, EMeshType InMeshType) :
	Entity(InHub, InMeshType),
	ActorState(InActorState),
	LastStateChangeTime(InHub.GetClock())
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
	Collider->setIntersectionCallback(
		[this](const w4::core::CollisionInfo & Info)
		{
			OnCollision(*(Info.source), *(Info.target));
		});
	Collider->setIntersecting(FALSE);
}

void Collidable::SetMesh(w4::sptr<w4::render::Mesh> InMesh, FLOAT CollisionSize)
{
	Entity::SetMesh(InMesh);
	Collider = Mesh->addCollider<w4::core::Sphere>(CollisionSize);
	Collider->setIntersectionCallback(
		[this](const w4::core::CollisionInfo & Info)
		{
			OnCollision(*(Info.source), *(Info.target));
		});
	Collider->setIntersecting(FALSE);
}

void Collidable::SetSkinnedMesh(std::string filename, std::string model, FLOAT CollisionSize)
{
	Entity::SetSkinnedMesh(filename, model);
	Collider = SkinnedMesh->addCollider<w4::core::Sphere>(CollisionSize);
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
	//Material->setParam("baseColor", Color);
	Material->setTexture(w4::resources::TextureId::TEXTURE_0, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, Color)));
}

void Collidable::OnCollision(const w4::core::Collider & SourceCollider, const w4::core::Collider & TargetCollider)
{
	w4::sptr<Collidable> Source = std::dynamic_pointer_cast<Collidable>(LinkToHub->ResolveEntity(SourceCollider.getParent()->getName()));
	w4::sptr<Collidable> Target = std::dynamic_pointer_cast<Collidable>(LinkToHub->ResolveEntity(TargetCollider.getParent()->getName()));

	if (IsType(&*Source, PawnType) && IsType(&*Target, ObstacleType))
	{
		//W4_LOG_INFO(c2s(Source->GetColor()).c_str());
		//W4_LOG_INFO(c2s(Target->GetColor()).c_str());
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			if (Source->GetColor() == Target->GetColor())
			{
				Target->ShouldDie = TRUE;
			}
			else
			{
				Source->ShouldDie = TRUE;
			}
		}
	}
	if (IsType(&*Source, ObstacleType) && IsType(&*Target, PawnType))
	{
		//W4_LOG_INFO(c2s(Source->GetColor()).c_str());
		//W4_LOG_INFO(c2s(Target->GetColor()).c_str());

		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			if (Source->GetColor() == Target->GetColor())
			{
				Source->ShouldDie = TRUE;
			}
			else
			{
				Target->ShouldDie = TRUE;
			}
		}
	}
	if (IsType(&*Source, PawnType) && IsType(&*Target, EnemyType))
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			Target->ShouldDie = TRUE;

			Source->ShouldDie = TRUE;	
		}
	}
	if (IsType(&*Source, EnemyType) && IsType(&*Target, PawnType))
	{
		if (Source->ActorState == EActorState::Alive && Target->ActorState == EActorState::Alive)
		{
			Source->ShouldDie = TRUE;

			Target->ShouldDie = TRUE;
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

