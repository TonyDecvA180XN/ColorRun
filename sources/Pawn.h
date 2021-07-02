#pragma once

#include <W4Framework.h>
#include "Collidable.h"

class Pawn : public Collidable
{
public:
	Pawn() = delete;
	Pawn(Hub & InHub, EActorState InActorState = EActorState::Alive, EMeshType InMeshType = EMeshType::Static);
	Pawn(const Pawn & Other) = delete;
	Pawn(Pawn && Other);
	Pawn & operator=(const Pawn & Other) = delete;
	Pawn & operator=(Pawn && Other) = delete;
	~Pawn() override = default;

	void UpdateRun(FLOAT PlayheadPosition, w4::sptr<w4::render::Node> Playhead);

	void UpdateBattle(w4::math::vec3 Direction);
	
	void SetColor(const w4::math::vec4 InColor, BOOL Sure);

	static w4::math::vec4 CrowdColor;
};
