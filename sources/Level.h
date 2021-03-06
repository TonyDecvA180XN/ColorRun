#pragma once

#include <W4Framework.h>
#include "Type.h"
#include "Obstacle.h"
#include "Pawn.h"
#include "Enemy.h"

W4_USE_UNSTRICT_INTERFACE

class Level
{
public:

	enum class ELevelState
	{
		None = 0,
		Run,
		Battle
	};

	Level() = delete;
	Level(const Level & other) = delete;
	Level(Level && other) = default;
	Level(Hub & InHub, INDEX Number);
	~Level();

	INT Update(FLOAT DeltaTime);

	void OnColorChanged(vec4 Color);

private:
	void CreateLevel1(Hub & InHub);
	//void CreateLevel2(Hub & InHub);
	//void CreateLevel3(Hub & InHub);
	//void CreateLevel4(Hub & InHub);
	//void CreateLevel5(Hub & InHub);

	vec3 GetFormationOffset(FLOAT GridStep, INDEX Index);

	ELevelState LevelState { ELevelState::None };

	FLOAT Length {};
	sptr<Node> Playhead {};
	std::vector<sptr<Obstacle>> Obstacles {};
	std::vector<sptr<Pawn>> Pawns {};
	std::vector<sptr<Enemy>> Enemies {};
};
