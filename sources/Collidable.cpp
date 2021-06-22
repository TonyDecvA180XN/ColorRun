#include "Collidable.h"

Collidable::Collidable(Hub & InHub, FCollider InCollider = FCollider()) :
	Entity(InHub), Collider(InCollider)
{
	FCollider UnInit;
	if (InCollider == UnInit)
	{
		W4_LOG_ERROR("COLLIDABLE SECTION WAS NOT SPECIFIED.");
	}
}

Collidable::Collidable(Collidable && Other) :
	Entity(std::move(Other))
{
}


Collidable::~Collidable()
{
}

BOOL Collidable::operator*(const Collidable & Other)
{
	if (std::abs(this->GetPosition() - Other.GetPosition()) > std::max(this->Collider.Radius, Other.Collider.Radius))
	{
		return FALSE;
	}
	else if (this->Collider.ColliderType == FCollider::EColliderType::Box && Other.Collider.ColliderType == FCollider::EColliderType::Box)
	{
		FLOAT LeftBorder = std::max(this->Collider.Left, Other.Collider.Left);
		FLOAT RightBorder = std::min(this->Collider.Right, Other.Collider.Right);
		return LeftBorder < RightBorder;
	}
	else 
	{
		// Sphere colliders UNIMPLEMENTED
	}
	return FALSE;
}
