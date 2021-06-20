#include "Level.h"

Level::Level(const Hub & InHub, INDEX Number)
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

void Level::CreateLevel1(const Hub & InHub)
{
	Road = w4::make::sptr<class Road>();
	Road->LoadMeshes("meshes/chunks.w4a"s, 1);
	Road->BuildMap(32, InHub.GetSceneRoot());



	//m_road.LoadMeshes("meshes/chunks.w4a"s, 1);
//m_road.BuildMap(32, Render::getRoot());
	//	wall.push_back(make::sptr<Entity>(hub));
//	wall[i]->SetMesh("meshes/wall.w4a"s, "wall"s);
//	wall[i]->SetMaterial("default"s);
//	wall[i]->Transform().translateWorld({ -1.5f + (i % 2) * 3, 0, 16.0f * (i + 1) });
//	wall[i]->Transform().setWorldScale({ 0.5f, 0.5f, 0.5f });
//	wall[i]->Material().setParam("baseColor"s, color::random());
//	wall[i]->Material().setParam("specColor"s, math::vec4{ 0,0,0,1 });
//	wall[i]->Parent(Render::getRoot());
}
