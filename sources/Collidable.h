#pragma once

#include <W4Framework.h>
#include "Entity.h"
#include "Hub.h"

class Collidable : public Entity
{
public:
	enum class EActorType
	{
		None = 0,
		MainCharacter,
		Pawn,
		Obstacle
	};

	enum class EActorState
	{
		None = 0,
		Ready,
		Nascent,
		Alive,
		Dying,
		Dead
	};

	Collidable() = delete;
	Collidable(Hub & InHub, EActorType InActorType, EActorState InActorState = EActorState::Ready);
	Collidable(const Collidable & Other) = delete;
	Collidable(Collidable && Other);
	Collidable & operator=(const Collidable & Other) = delete;
	Collidable & operator=(Collidable && Other) = delete;
	~Collidable() override;

	void Update(FLOAT PlayheadPosition) override;

	void SetMesh(std::string filename, std::string model, FLOAT CollisionSize);

	FLOAT GetPosition() const { return Mesh->getWorldTranslation().z; }

	w4::math::vec4 GetColor() const { return Color; }

	void SetColor(const w4::math::vec4 InColor);

	static void OnCollision(const w4::core::Collider & SourceCollider, const w4::core::Collider & TargetCollider);

private:

	void SetUpdatedTime() { LastStateChangeTime = LinkToHub->GetClock(); }

	w4::sptr<w4::core::Collider> Collider { nullptr };
	w4::math::vec4 Color = {};

	EActorType ActorType { EActorType::None };
	EActorState ActorState { EActorState::None };
	FLOAT LastStateChangeTime { 0.f };
};
