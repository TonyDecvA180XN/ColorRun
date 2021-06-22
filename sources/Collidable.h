#pragma once

#include <W4Framework.h>
#include "Entity.h"
#include "Hub.h"

class Collidable : public Entity
{
public:
	struct FCollider
	{
		enum class EColliderType
		{
			None = 0,
			Box,
			Sphere
		};

		FLOAT Left { 0.f };
		FLOAT Right { 0.f };
		FLOAT Radius { 0.f };
		EColliderType ColliderType { EColliderType::None };

		FCollider() {};
		BOOL operator==(const FCollider & Other)
		{
			return this->Left == Other.Left && this->Right == Other.Right;
		}
	};

	Collidable() = delete;
	Collidable(Hub & InHub, FCollider InSection);
	Collidable(const Collidable & Other) = delete;
	Collidable(Collidable && Other);
	Collidable & operator=(const Collidable & Other) = delete;
	Collidable & operator=(Collidable && Other) = delete;
	~Collidable() override;

	FLOAT GetPosition() const { return m_mesh->getWorldTranslation().z; }

	BOOL operator*(const Collidable & Other);

private:
	FCollider Collider {};
};
