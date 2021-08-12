#include "Enemy.h"
#include "MathUtils.h"

Enemy::Enemy(Hub & InHub, EActorState InActorState, EMeshType InMeshType) :
	Collidable(InHub, InActorState, InMeshType)
{
}

Enemy::Enemy(Enemy && Other) :
	Collidable(std::move(Other))
{
}

void Enemy::UpdateRun(FLOAT PlayheadPosition)
{
	//SetElevation(1.f + 0.25f * std::sinf(0.5f * PlayheadPosition));
}

void Enemy::UpdateBattle(w4::math::vec3 ConvergePoint)
{
	static constexpr FLOAT DespawnTime = 1.f;

	switch (ActorState)
	{
		case Collidable::EActorState::Alive:
		{
			Collider->setIntersecting(TRUE);
			w4::math::vec3 LookVector = ConvergePoint - GetNode()->getWorldTranslation();
			w4::math::Rotator Rotator = LookVector.toRotator({ 0.f, 1.f, 0.f });
			w4::math::Rotator RotOffset{ 0, 0, 0 };
			GetNode()->setWorldRotation(Rotator + RotOffset);

			constexpr FLOAT AttackSpeed = 0.02f;
			GetNode()->translateWorld(LookVector.normalize() * AttackSpeed);
			if (ShouldDie)
			{
				ActorState = EActorState::Dying;
				SetUpdatedTime();
				Play("Death");
			}
			break;
		}
		case Collidable::EActorState::Dying:
		{
			Collider->setIntersecting(FALSE);
			FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
			//SetElevation(TimeElapsed * 10.f);
			if (TimeElapsed >= DespawnTime)
			{
				ActorState = EActorState::Dead;
				SetUpdatedTime();
				Mesh->setEnabled(FALSE);
			}
			break;
		}
		case Collidable::EActorState::Dead:
		{
			Collider->setIntersecting(FALSE);
			break;
		}
		default:
			break;
	}
}