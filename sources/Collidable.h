#pragma once

#include <W4Framework.h>
#include "Entity.h"
#include "Hub.h"

class Obstacle;
class Pawn;
class Enemy;

class Collidable : public Entity
{
public:

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
	Collidable(Hub & InHub, EActorState InActorState);
	Collidable(const Collidable & Other) = delete;
	Collidable(Collidable && Other);
	Collidable & operator=(const Collidable & Other) = delete;
	Collidable & operator=(Collidable && Other) = delete;
	~Collidable() override = default;

	void Update(FLOAT PlayheadPosition) override {};

	BOOL IsDead() const { return ActorState == EActorState::Dying || ActorState == EActorState::Dead; };

	BOOL IsReadyToDelete() const { return ActorState == EActorState::Dead; };

	void SetMesh(std::string filename, std::string model, FLOAT CollisionSize);

	FLOAT GetPosition() const { return Mesh->getWorldTranslation().z; }

	w4::math::vec4 GetColor() const { return Color; }

	virtual void SetColor(const w4::math::vec4 InColor);

private:

	static const Obstacle * const ObstacleType;
	static const Pawn * const PawnType;
	static const Enemy * const EnemyType;

protected:

	static void OnCollision(const w4::core::Collider & SourceCollider, const w4::core::Collider & TargetCollider);

	void SetUpdatedTime() { LastStateChangeTime = LinkToHub->GetClock(); }

	void SetUniformScale(FLOAT Scale);

	void SetElevation(FLOAT Elevation);

	w4::sptr<w4::core::Collider> Collider { nullptr };
	w4::math::vec4 Color = {};

	EActorState ActorState { EActorState::None };
	FLOAT LastStateChangeTime { 0.f };
};
