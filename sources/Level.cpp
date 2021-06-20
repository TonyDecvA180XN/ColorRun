#include "Level.h"
#include <array>

Level::Level(Hub & InHub, INDEX Number)
{
	switch (Number)
	{
		case 1:
		{
			CreateLevel1(InHub);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Level::Update(FLOAT PlayheadPosition)
{
	Road->Update(PlayheadPosition);
	for (Entity & Entity : Entities)
	{
		Entity.Update(PlayheadPosition);
	}
}

void Level::CreateLevel1(Hub & InHub)
{
	constexpr SIZE NumRoadChunks = 16;

	Road = w4::make::sptr<class Road>();
	Road->LoadMeshes("meshes/chunks.w4a"s, 1);
	Road->BuildMap(NumRoadChunks, InHub.GetSceneRoot());
	
	constexpr SIZE NumObstacles = 6;
	//constexpr SIZE NumClutterObjects = 12;

	std::array<w4::math::vec3, NumObstacles> ObstaclePositions =
	{
		{
			{ -3.f, 0.f, 4 * 8.f },
			{ +0.f, 0.f, 6 * 8.f },
			{ +3.f, 0.f, 8 * 8.f },
			{ +0.f, 0.f, 10 * 8.f },
			{ +0.f, 0.f, 11 * 8.f },
			{ -3.f, 0.f, 15 * 8.f }
		}
	};

	//std::array<w4::math::vec3, NumClutterObjects> ClutterPositions =
	//{
	//	{
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ +6.f, 0.f, 5 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f },
	//		{ -4.f, 0.f, 4 * 8.f }
	//		
	//	}
	//};
	

	for (INDEX i = 0; i != NumObstacles; ++i)
	{
		Entities.emplace_back(InHub);
		Entities.at(i).SetMesh("meshes/wall.w4a"s, "wall"s);
		Entities.at(i).SetMaterial("lambert"s);
		Entities.at(i).SetTexture("textures/wall.jpg");
		Entities.at(i).Transform().setWorldScale({ 0.5f, 1.f, 1.f });
		Entities.at(i).Transform().setWorldTranslation(ObstaclePositions.at(i));
	}
	
	//for (INDEX i = 0; i != NumClutterObjects; ++i)
	//{
	//	INDEX Offset = i + NumObstacles;
	//	Entities.emplace_back(InHub);
	//	Entities.at(Offset).SetMesh("meshes/wall.w4a"s, "wall"s);
	//	Entities.at(Offset).SetMaterial("lambert"s);
	//	Entities.at(Offset).SetTexture("textures/wall.jpg");
	//	Entities.at(Offset).Transform().setWorldTranslation(ClutterPositions.at(i));
	//}
}
