#include "Level.h"
#include <array>
#include "MathUtils.h"

Level::Level(Hub & InHub, INDEX Number) :
	Playhead(make::sptr<Node>()),
	LevelState(ELevelState::Run)
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
			resources::Image::get("textures/env/sky_release/px.png"s),
			resources::Image::get("textures/env/sky_release/nx.png"s),
			resources::Image::get("textures/env/sky_release/py.png"s),
			resources::Image::get("textures/env/sky_release/ny.png"s),
			resources::Image::get("textures/env/sky_release/pz.png"s),
			resources::Image::get("textures/env/sky_release/nz.png"s)
		});
	EnvironmentTexture->setFiltering(Filtering::Level1);
	

	// tune camera
	sptr<class Camera> Camera = Render::getScreenCamera();
	Camera->setFov(65.0f);
	Camera->setWorldTranslation({ 0.f, 8.f, -10.f });
	Camera->setWorldRotation(Rotator(30_deg, 0, 0.0f));
	Camera->setClearMask(ClearMask::Color | ClearMask::Depth | ClearMask::Skybox);
	Camera->getSkybox()->setCubemap(EnvironmentTexture);

	// create surround lights
	sptr<PointLight> FrontLight = make::sptr<PointLight>("FrontLight"s);
	FrontLight->setWorldTranslation({ 0.f, 8.f, 6.f });
	FrontLight->setColor({ 1.f, 1.f, 1.f });
	FrontLight->setDecayRate(core::LightDecayRate::Linear);
	FrontLight->setIntensity(1.f);

	sptr<PointLight> BackLight = make::sptr<PointLight>("BackLight"s);
	BackLight->setWorldTranslation({ 0.f, 6.f, -6.f });
	BackLight->setColor({ 1.f, 1.f, 0.9f });
	BackLight->setDecayRate(core::LightDecayRate::Linear);
	BackLight->setIntensity(1.f);

	// setup sun light
	Render::getPass(0)->getDirectionalLight()->setColor(math::vec3(1.f, 1.f, 1.f));
	Render::getPass(0)->getDirectionalLight()->setDirection(math::vec3(-1.0f, -3.0f, 2.0f));

	// add floor
	//constexpr SIZE FloorTilingFactor = 16;
	//constexpr FLOAT FloorSizeAbsolute = 256.f;
	//sptr<Entity> Floor = make::uptr<Entity>(InHub);
	//Floor->SetMesh(Mesh::create::plane({ FloorTilingFactor, FloorTilingFactor }, TRUE));
	//Floor->GetNode()->setWorldScale({ FloorSizeAbsolute / FloorTilingFactor, FloorSizeAbsolute / FloorTilingFactor, 1.f });
	//Floor->GetNode()->setWorldRotation(Rotator(90_deg, 0, 0));
	//Floor->GetNode()->setWorldTranslation({ 0, -0.1f, FloorSizeAbsolute / 2 - 16.f});
	//Floor->SetMaterial("lambert"s);
	//Floor->SetTexture("textures/grass.jpg"s);
	
	// link parents
	Playhead->addChild(Camera);
	Playhead->addChild(FrontLight);
	Playhead->addChild(BackLight);
	InHub.GetSceneRoot()->addChild(Playhead);
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
	if (LevelState == ELevelState::Run)
	{
		constexpr FLOAT MovementSpeed = 8.f;
		Playhead->translateWorld({ 0.f, 0.f, MovementSpeed * DeltaTime });

		FLOAT PlayheadPosition = Playhead->getWorldTranslation().z;
		//Road->Update(PlayheadPosition);
		for (sptr<Obstacle> Each : Obstacles)
		{
			Each->Update(PlayheadPosition);
		}
		for (sptr<Pawn> Each : Pawns)
		{
			Each->UpdateRun(PlayheadPosition, Playhead);
		}
		for (auto Each = Pawns.begin(); Each != Pawns.end();)
		{
			if ((*Each)->IsReadyToDelete())
				Each = Pawns.erase(Each);
			else
				++Each;
		}
		for (sptr<Enemy> Each : Enemies)
		{
			Each->UpdateRun(PlayheadPosition);
		}
		
		if (PlayheadPosition >= Length)
		{
			LevelState = ELevelState::Battle;
			for (sptr<Pawn> Each : Pawns)
				Each->Play("Attack");
			for (sptr<Enemy> Each : Enemies)
				Each->Play("Attack");
		}
		if (Pawns.empty())
			return -1;
	}
	else if (LevelState == ELevelState::Battle)
	{
		for (sptr<Pawn> Each : Pawns)
		{
			vec3 ClosestEnemyPosition = { 0.f, 0.f, 1.f };
			FLOAT ClosestEnemyDistance = INF;
			
			for (sptr<Enemy> Opponent : Enemies)
			{
				if (!Opponent->IsDead())
				{
					FLOAT EnemyDistance = (Each->GetNode()->getWorldTranslation() - Opponent->GetNode()->getWorldTranslation()).length();
					if (EnemyDistance < ClosestEnemyDistance)
					{
						ClosestEnemyDistance = EnemyDistance;
						ClosestEnemyPosition = Opponent->GetNode()->getWorldTranslation();
					}
				}
			}
			Each->UpdateBattle(ClosestEnemyPosition);
		}
		for (sptr<Enemy> Each : Enemies)
		{
			vec3 ClosestEnemyPosition = { 0.f, 0.f, 1.f };
			FLOAT ClosestEnemyDistance = INF;
			
			for (sptr<Pawn> Opponent : Pawns)
			{
				if (!Opponent->IsDead())
				{
					FLOAT EnemyDistance = (Each->GetNode()->getWorldTranslation() - Opponent->GetNode()->getWorldTranslation()).length();
					if (EnemyDistance < ClosestEnemyDistance)
					{
						ClosestEnemyDistance = EnemyDistance;
						ClosestEnemyPosition = Opponent->GetNode()->getWorldTranslation();
					}
				}
			}

			Each->UpdateBattle(ClosestEnemyPosition);
		}

		for (auto Each = Pawns.begin(); Each != Pawns.end();)
		{
			if ((*Each)->IsReadyToDelete())
				Each = Pawns.erase(Each);
			else
				++Each;
		}
		for (auto Each = Enemies.begin(); Each != Enemies.end();)
		{
			if ((*Each)->IsReadyToDelete())
				Each = Enemies.erase(Each);
			else
				++Each;
		}

		if (Pawns.empty())
			return -1;
		if (Enemies.empty())
			return Pawns.size();
	}
	
	return 0;
}

void Level::OnColorChanged(vec4 Color)
{
	Pawn::CrowdColor = Color;
	for (sptr<Pawn> Each : Pawns)
	{
		Each->SetColor(Pawn::CrowdColor, FALSE);
	}
}

void Level::CreateLevel1(Hub & InHub)
{
	//constexpr SIZE NumRoadChunks = 16;

	//Road = w4::make::sptr<class Road>();
	//Road->LoadMeshes("meshes/chunks.w4a"s, 1);
	//Road->BuildMap(NumRoadChunks, InHub.GetSceneRoot());
	
	Length = 380.f;
	constexpr SIZE NumObstacles = 12;
	//constexpr SIZE NumClutterObjects = 12;
	constexpr SIZE NumPawnsLocal = 6; // initial crowd
	constexpr SIZE NumPawnsGlobal = 9; // spread across level

	std::array<w4::math::vec3, NumObstacles> ObstaclePositions =
	{
		{
			{ -3, 0, 30 },
			{ +0, 0, 50 },
			{ +3, 0, 80 },
			{ -3, 0, 120 },
			{ +3, 0, 150 },
			{ +0, 0, 180 },
			{ -3, 0, 190 },
			{ +3, 0, 250 },
			{ -3, 0, 260 },
			{ +0, 0, 280 },
			{ +0, 0, 320 },
			{ +0, 0, 350 }
			
		}
	};

	std::array<vec4, NumObstacles> ObstacleColors = 
	{
		{
			color(color::Red).rgba,
			color(0x00FF00FF).rgba,
			color(color::Blue).rgba,
			color(color::Cyan).rgba,
			color(color::Magenta).rgba,
			color(color::Yellow).rgba,
			color(color::Black).rgba,
			color(0x00FF00FF).rgba,
			color(color::Blue).rgba,
			color(color::Black).rgba,
			color(color::Red).rgba,
			color(color::Cyan).rgba,
		}
	};

	std::array<w4::math::vec3, NumPawnsGlobal> PawnPositions =
	{
		{
			{ +3.f, 0.f, 4 * 8.f },
			{ +1.f, 0.f, 5 * 8.f },
			{ -3.f, 0.f, 8 * 8.f },
			{ -1.5f, 0.f, 10 * 8.f },
			{ -1.f, 0.f, 12 * 8.f },
			{ +1.5f, 0.f, 16 * 8.f },
			{ -3.f, 0.f, 20 * 8.f },
			{ -0.f, 0.f, 28 * 8.f },
			{ +3.f, 0.f, 32 * 8.f },
		}
	};

	
	// add obstacles
	for (INDEX i = 0; i != NumObstacles; ++i)
	{
		Obstacles.push_back(make::uptr<Obstacle>(InHub));
		Obstacles.back()->SetMesh("meshes/stonewall.w4a"s, "wall"s, 1.f*40);
		Obstacles.back()->SetMaterial("lambert"s);
		Obstacles.back()->GetNode()->setWorldTranslation(ObstaclePositions[i]);
		Obstacles.back()->GetNode()->setWorldScale({ 0.1f, 0.1f, 0.1f });
		Obstacles.back()->GetNode()->translateWorld({ 0, 0.2f, 0 });
		Obstacles.back()->SetColor(ObstacleColors[i]);
	}

	// add initial crowd
	for (INDEX i = 0; i != NumPawnsLocal; ++i)
	{
		Pawns.push_back(make::uptr<Pawn>(InHub, Collidable::EActorState::Alive, EMeshType::Skinned));
		Pawns.back()->SetSkinnedMesh("stickman.w4a"s, "mesh"s, 0.75f);
		Pawns.back()->SetMaterial("lambert"s);
		Pawns.back()->GetNode()->translateWorld(GetFormationOffset(1.5f, i));
		Pawns.back()->GetNode()->translateWorld({ 0.f, 0.f, 0.f });
		//Pawns.back()->GetNode()->setLocalScale({ 1.f, 1.f, 1.f });
		Pawns.back()->GetNode()->setWorldRotation(Rotator());
		Pawns.back()->SetColor({ 1.f, 1.f, 1.f, 1.f }, TRUE);
		Pawns.back()->Parent(Playhead);
		Pawns.back()->Play("Run");
	}

	// add crowd recruits
	for (INDEX i = 0; i != NumPawnsGlobal; ++i)
	{
		Pawns.push_back(make::uptr<Pawn>(InHub, Collidable::EActorState::Ready, EMeshType::Skinned));
		Pawns.back()->SetSkinnedMesh("stickman.w4a"s, "mesh"s, 0.75f);
		Pawns.back()->SetMaterial("lambert"s);
		//Pawns.back()->GetNode()->setLocalScale({ 1.f, 1.f, 1.f });
		Pawns.back()->GetNode()->translateWorld(PawnPositions[i]);
		//Pawns.back()->GetNode()->translateWorld({ 0.f, 0.f, 0.f });
		Pawns.back()->GetNode()->setWorldRotation(Rotator());
		Pawns.back()->SetColor({ 1.f, 1.f, 1.f, 1.f }, TRUE);
		Pawns.back()->Play("Standby");
	}

	sptr<Entity> a = make::uptr<Entity>(InHub);
	a->SetMesh("meshes/road.w4a", ""s);
	a->GetNode()->setWorldRotation(Rotator(0, 180_deg, 0));
	a->GetNode()->setWorldTranslation({ 0, 0.2f, 20.f });
	a->GetNode()->setWorldScale({ 0.1f, 0.1f, 0.1f });
	a->GetNode()->traversalTyped<Mesh>([&InHub](cref<Mesh> Node)
									   {
										   std::string NodeName(Node->getName());
										   if (NodeName.rfind("leaf", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("darkgreen"));
										   }
										   else if (NodeName.rfind("b", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("brown"));
										   }
										   else if (NodeName.rfind("B", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("brown"));
										   }
										   else if (NodeName.rfind("Chest", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("brown"));
										   }
										   else if (NodeName.rfind("Fire", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("brown"));
										   }
										   else if (NodeName.rfind("Road", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("grey"));
										   }
										   else if (NodeName.rfind("Fense", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("brown"));
										   }
										   else if (NodeName.rfind("Landscape", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("green"));
										   }
										   else if (NodeName.rfind("hill", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("darkgrey"));
										   }
										   else if (NodeName.rfind("gate", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("brown"));
										   }
										   else if (NodeName.rfind("Hay", 0) == 0)
										   {
											   Node->setMaterialInst(InHub.ResolveColor("yellow"));
										   }
										   else
										   {
												W4_LOG_INFO(("Uncolored Mesh: " + NodeName).c_str());
										   }
									   });
	//a->GetMaterial();

	constexpr SIZE NumEnemies = 5;
	
	// add enemy side army
	for (INDEX i = 0; i != NumEnemies; ++i)
	{
		Enemies.push_back(make::uptr<Enemy>(InHub, Collidable::EActorState::Alive, EMeshType::Skinned));
		Enemies.back()->SetSkinnedMesh("stickman.w4a"s, "mesh"s, 1.f);
		Enemies.back()->SetMaterial("lambert"s);
		Enemies.back()->SetColor({ 0.9f, 0.9f, 0.9f, 1.0f });
		Enemies.back()->GetNode()->setWorldRotation(Rotator());
		Enemies.back()->GetNode()->rotateWorld(Rotator(0, 180_deg, 0));
		Enemies.back()->GetNode()->translateWorld(GetFormationOffset(3.f, i));
		Enemies.back()->GetNode()->translateWorld({ 0.f, 0.f, Length + 16.f });
		Enemies.back()->Play("Standby");
	}
}

vec3 Level::GetFormationOffset(FLOAT GridStep, INDEX Index)
{
	INDEX Row = static_cast<INDEX>(((std::sqrtf(8 * Index + 1) - 1.f) / 2));
	INDEX RowStart = (Row * (Row + 1)) / 2;
	INDEX Column = Index - RowStart;
	
	FLOAT RowLength = 2 * GridStep * Row;
	return { -RowLength / 2 + 2 * GridStep * Column, 0.f, -GridStep * Row  };
}
