#include "Pawn.h"
#include "MathUtils.h"

w4::math::vec4 Pawn::CrowdColor = { 1.f, 1.f, 1.f, 1.f };

Pawn::Pawn(Hub & InHub, EActorState InActorState) :
	Collidable(InHub, InActorState)
{
}

Pawn::Pawn(Pawn && Other) :
	Collidable(std::move(Other))
{
}

void Pawn::UpdateRun(FLOAT PlayheadPosition, w4::sptr<w4::render::Node> Playhead)
{
	static constexpr FLOAT SpawnDistance = 24.f;
	//static constexpr FLOAT DespawnDistance = 16.f;
	static constexpr FLOAT SpawnTime = 1.f;
	static constexpr FLOAT DespawnTime = 1.f;
	static constexpr FLOAT JoinDistance = 1.f;

	switch (ActorState)
	{
	case Collidable::EActorState::Ready:
	{
		//SetElevation(1.f + 0.25f * std::sinf(0.5f * PlayheadPosition));

		SetUniformScale(0.f);
		SetElevation(1.f);
		FLOAT PawnDistance = std::abs(GetPosition() - PlayheadPosition);
		if (PawnDistance < SpawnDistance)
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
		SetUniformScale(Growth);
		FLOAT Elevation = -4.f * (Growth * Growth) + 3.f * Growth + 1.f;
		SetElevation(Elevation + 1.f);

		FLOAT CrowdDistance = std::abs(PlayheadPosition - GetNode()->getWorldTranslation().z);
		if (CrowdDistance <= JoinDistance)
		{
			ActorState = EActorState::Alive;
			SetUpdatedTime();
			SetUniformScale(1.f);
			SetElevation(1.f);
			Parent(Playhead);
			SetColor(CrowdColor);
			//GetNode()->translateWorld(GetFormationOffset(2.f, i));
			//GetNode()->translateWorld({ 0.f, 1.f, 0.f });
		}
		break;
	}
	case Collidable::EActorState::Alive:
	{
		Collider->setIntersecting(TRUE);
		break;
	}
	case Collidable::EActorState::Dying:
	{
		Collider->setIntersecting(FALSE);
		FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
		SetElevation(TimeElapsed * 10.f);
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
	{
		break;
	}
	}
}

void Pawn::UpdateBattle(w4::math::vec3 ConvergePoint)
{
	static constexpr FLOAT DespawnTime = 1.f;
	static constexpr FLOAT JoinDistance = 1.f;

	switch (ActorState)
	{
		case Collidable::EActorState::Alive:
		{
			Collider->setIntersecting(TRUE);
			w4::math::vec3 LookVector = ConvergePoint - GetNode()->getWorldTranslation();
			w4::math::Rotator Rotator = LookVector.toRotator({ 0.f, 1.f, 0.f });
			GetNode()->setWorldRotation(Rotator);

			constexpr FLOAT AttackSpeed = 0.02f;
			GetNode()->translateWorld(LookVector.normalize() * AttackSpeed);
			break;
		}
		case Collidable::EActorState::Dying:
		{
			Collider->setIntersecting(FALSE);
			FLOAT TimeElapsed = std::abs(LinkToHub->GetClock() - LastStateChangeTime);
			SetElevation(TimeElapsed * 10.f);
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

void Pawn::SetColor(const w4::math::vec4 InColor)
{
	if (ActorState == EActorState::Alive)
	{
		Collidable::SetColor(InColor);
	}
}
