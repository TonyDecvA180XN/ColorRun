#include "Level.h"
#include <array>
#include "MathUtils.h"

Level::Level(Hub & InHub, INDEX Number) :
	Playhead(make::sptr<Node>())
{
	// setup level
	switch (Number)
	{
		case 1:
		{
			CreateLevel1(InHub);
			break;
		}
		case 2:
		{
			CreateLevel1(InHub);
			break;
		}
		case 3:
		{
			CreateLevel1(InHub);
			break;
		}
		case 4:
		{
			CreateLevel1(InHub);
			break;
		}
		case 5:
		{
			CreateLevel1(InHub);
			break;
		}
		default:
		{
			break;
		}
	}

	// load HDRI
	sptr<Cubemap> EnvironmentTexture = Cubemap::fromImages(
		{
			resources::Image::get("textures/env/sky/px.png"s),
			resources::Image::get("textures/env/sky/nx.png"s),
			resources::Image::get("textures/env/sky/py.png"s),
			resources::Image::get("textures/env/sky/ny.png"s),
			resources::Image::get("textures/env/sky/pz.png"s),
			resources::Image::get("textures/env/sky/nz.png"s)
		});
	EnvironmentTexture->setFiltering(Filtering::Level1);
	

	// tune camera
	sptr<class Camera> Camera = Render::getScreenCamera();
	Camera->setFov(60.0f);
	Camera->setWorldTranslation({ 0.f, 5.f, -10.f });
	Camera->setWorldRotation(Rotator(30_deg, 0, 0.0f));
	Camera->setClearMask(ClearMask::Color | ClearMask::Depth | ClearMask::Skybox);
	Camera->getSkybox()->setCubemap(EnvironmentTexture);

	// create surround lights
	sptr<PointLight> FrontLight = make::sptr<PointLight>("FrontLight"s);
	FrontLight->setWorldTranslation({ 0.f, 5.f, 3.f });
	FrontLight->setColor({ 1.f, 1.f, 1.f });
	FrontLight->setDecayRate(core::LightDecayRate::Linear);
	FrontLight->setIntensity(0.5f);

	sptr<PointLight> BackLight = make::sptr<PointLight>("BackLight"s);
	BackLight->setWorldTranslation({ 0.f, 5.f, -3.f });
	BackLight->setColor({ 1.f, 1.f, 1.f });
	BackLight->setDecayRate(core::LightDecayRate::Linear);
	BackLight->setIntensity(0.5f);

	// setup sun light
	Render::getPass(0)->getDirectionalLight()->setColor(math::vec3(1.f, 1.f, 1.f));
	Render::getPass(0)->getDirectionalLight()->setDirection(math::vec3(1.0f, -1.0f, 1.0f));

	// add floor
	constexpr SIZE FloorTilingFactor = 16;
	constexpr FLOAT FloorSizeAbsolute = 256.f;
	sptr<Mesh> Floor = Mesh::create::plane({ FloorTilingFactor, FloorTilingFactor }, TRUE);
	Floor->setWorldScale({ FloorSizeAbsolute / FloorTilingFactor, FloorSizeAbsolute / FloorTilingFactor, 1.f });
	Floor->setWorldRotation(Rotator(90_deg, 0, 0));
	Floor->setWorldTranslation({ 0, -0.1f, FloorSizeAbsolute / 2 - 16.f});
	Floor->setMaterialInst(InHub.ResolveMaterial("lambert"s));
	Floor->getMaterialInst()->setTexture(TextureId::TEXTURE_0, InHub.ResolveTexture("textures/grass.jpg"s));
	
	// link parents
	Playhead->addChild(Camera);
	Playhead->addChild(FrontLight);
	Playhead->addChild(BackLight);
	InHub.GetSceneRoot()->addChild(Playhead);
	InHub.GetSceneRoot()->addChild(Floor);
}

Level::~Level()
{
	for (auto & child : Render::getRoot()->getAllChildren())
	{
		Render::getRoot()->removeChild(child->getName());
	}
}

INT Level::Update(FLOAT DeltaTime)
{
	constexpr FLOAT MovementSpeed = 8.f;
	Playhead->translateWorld({ 0.f, 0.f, MovementSpeed * DeltaTime });

	FLOAT PlayheadPosition = Playhead->getWorldTranslation().z;
	Road->Update(PlayheadPosition);
	for (sptr<Obstacle> Each : Obstacles)
	{
		Each->Update(PlayheadPosition);
	}
	for (sptr<Pawn> Each : Pawns)
	{
		Each->Update(PlayheadPosition, Playhead);
	}
	for (auto Each = Pawns.begin(); Each != Pawns.end();)
	{
		if ((*Each)->IsDead())
			Each = Pawns.erase(Each);
		else
			++Each;
	}
	
	if (Pawns.size() == 0)
		return -1;
	if (PlayheadPosition >= Road->GetLength())
		return Pawns.size();
	return 0;

}

void Level::OnColorChanged(vec4 Color)
{
	Pawn::CrowdColor = Color;
	for (sptr<Pawn> Each : Pawns)
	{
		Each->SetColor(Color);
	}
}

void Level::CreateLevel1(Hub & InHub)
{
	constexpr SIZE NumRoadChunks = 16;

	Road = w4::make::sptr<class Road>();
	Road->LoadMeshes("meshes/chunks.w4a"s, 1);
	Road->BuildMap(NumRoadChunks, InHub.GetSceneRoot());
	
	constexpr SIZE NumObstacles = 6;
	constexpr SIZE NumClutterObjects = 12;
	constexpr SIZE NumPawnsLocal = 6; // initial crowd
	constexpr SIZE NumPawnsGlobal = 3; // spread across level



	std::array<w4::math::vec3, NumObstacles> ObstaclePositions =
	{
		{
			{ -3.f, 0.f, 4 * 8.f },
			{ +0.f, 0.f, 6 * 8.f },
			{ +3.f, 0.f, 8 * 8.f },
			{ +0.f, 0.f, 10 * 8.f },
			{ +0.f, 0.f, 11 * 8.f },
			{ -3.f, 0.f, 13 * 8.f }
		}
	};

	std::array<vec4, NumObstacles> ObstacleColors = 
	{
		{
			{ 1.f, 0.f, 0.f, 1.f },
			{ 0.f, 1.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f, 1.f },
			{ 0.f, 1.f, 1.f, 1.f },
			{ 1.f, 0.f, 1.f, 1.f },
			{ 1.f, 1.f, 0.f, 1.f },
		}
	};

	std::array<w4::math::vec3, NumPawnsGlobal> PawnPositions =
	{
		{
			{ +3.f, 0.f, 4 * 8.f },
			{ +0.f, 0.f, 5 * 8.f },
			{ -3.f, 0.f, 8 * 8.f },
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
	
	// add obstacles
	for (INDEX i = 0; i != NumObstacles; ++i)
	{
		Obstacles.push_back(make::uptr<Obstacle>(InHub));
		Obstacles.back()->SetMesh("meshes/wall.w4a"s, "wall"s, 1.f);
		Obstacles.back()->SetMaterial("default"s);
		Obstacles.back()->GetNode()->setWorldTranslation(ObstaclePositions[i]);
		Obstacles.back()->SetColor(ObstacleColors[i]);
	}

	// add initial crowd
	for (INDEX i = 0; i != NumPawnsLocal; ++i)
	{
		Pawns.push_back(make::uptr<Pawn>(InHub, Collidable::EActorState::Alive));
		Pawns.back()->SetMesh("meshes/monkey.w4a"s, "monkey"s, 0.9f);
		Pawns.back()->SetMaterial("default"s);
		Pawns.back()->GetNode()->translateWorld(GetFormationOffset(1.5f, i));
		Pawns.back()->GetNode()->translateWorld({ 0.f, 1.f, 0.f });
		Pawns.back()->SetColor({ 1.f, 1.f, 1.f, 1.f });
		Pawns.back()->Parent(Playhead);
	}

	// add crowd recruits
	for (INDEX i = 0; i != NumPawnsGlobal; ++i)
	{
		Pawns.push_back(make::uptr<Pawn>(InHub, Collidable::EActorState::Ready));
		Pawns.back()->SetMesh("meshes/monkey.w4a"s, "monkey"s, 0.9f);
		Pawns.back()->SetMaterial("default"s);
		Pawns.back()->GetNode()->translateWorld(PawnPositions[i]);
		Pawns.back()->GetNode()->translateWorld({ 0.f, 1.f, 0.f });
		Pawns.back()->SetColor({ 1.f, 1.f, 1.f, 1.f });
		//Pawns.back()->Parent(Playhead);
	}
	
	//for (INDEX i = 0; i != NumClutterObjects; ++i)
	//{
	//	INDEX Offset = i + NumObstacles;
	//	Entities.emplace_back(InHub);
	//	Entities[Offset].SetMesh("meshes/wall.w4a"s, "wall"s);
	//	Entities[Offset].SetMaterial("lambert"s);
	//	Entities[Offset].SetTexture("textures/wall.jpg");
	//	Entities[Offset].GetNode().setWorldTranslation(ClutterPositions[i]);
	//}
}

vec3 Level::GetFormationOffset(FLOAT GridStep, INDEX Index)
{
	INDEX Row = static_cast<INDEX>(((std::sqrtf(8 * Index + 1) - 1.f) / 2));
	INDEX RowStart = (Row * (Row + 1)) / 2;
	INDEX Column = Index - RowStart;
	
	FLOAT RowLength = 2 * GridStep * Row;
	return { -RowLength / 2 + 2 * GridStep * Column, 0.f, -GridStep * Row  };
}
