#pragma once

#include <W4Framework.h>
#include "Type.h"
#include "Road.h"
#include "Obstacle.h"
#include "Pawn.h"

W4_USE_UNSTRICT_INTERFACE

class Level
{
public:

	Level() = delete;
	Level(const Level & other) = delete;
	Level(Level && other) = default;
	Level(Hub & InHub, INDEX Number);
	~Level();

	INT Update(FLOAT DeltaTime);

	void OnColorChanged(vec4 Color);

	FLOAT GetLength() const { return this->Road->GetLength(); };

private:
	void CreateLevel1(Hub & InHub);
	//void CreateLevel2(Hub & InHub);
	//void CreateLevel3(Hub & InHub);
	//void CreateLevel4(Hub & InHub);
	//void CreateLevel5(Hub & InHub);

	vec3 GetFormationOffset(FLOAT GridStep, INDEX Index);

	sptr<Road> Road {};
	sptr<Node> Playhead {};
	std::vector<sptr<Obstacle>> Obstacles {};
	std::vector<sptr<Pawn>> Pawns {};
};
