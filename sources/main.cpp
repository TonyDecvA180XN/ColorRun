#include <W4Framework.h>
#include <functional>
#include <string>
#include "MathUtils.h"
#include "DebugHelper.h"
#include "Level.h"
#include "GUIManager.h"
#include "Hub.h"
#include "Entity.h"

W4_USE_UNSTRICT_INTERFACE

using namespace std::string_literals;

class ColorRun : public IGame
{
public:
	void onConfig() override
	{
		Platform::setWindowStyle("any");
	}

	void onStart() override
	{
		DebugHelper::buildGizmo();
		
		hub = make::sptr<Hub>(Render::getRoot());

		Levels.emplace_back(*hub, 1);


		

		auto cam = Render::getScreenCamera();
		cam->setFov(60.0f);
		cam->setWorldTranslation({ 0.0f, 5.0f, -10.0f });
		cam->setWorldRotation(Rotator(22.5_deg, 0, 0.0f));
		cam->setClearMask(ClearMask::Color | ClearMask::Depth | ClearMask::Skybox);
		//camCenter = make::sptr<w4::render::Node>();
		//camCenter->setWorldTranslation({ 0, 5.0f, 0 });
		//camCenter->addChild(cam);
		//Render::getRoot()->addChild(camCenter);

		auto skybox = Cubemap::fromImages({
			resources::Image::get("textures/env/sky/px.png"),
			resources::Image::get("textures/env/sky/nx.png"),
			resources::Image::get("textures/env/sky/py.png"),
			resources::Image::get("textures/env/sky/ny.png"),
			resources::Image::get("textures/env/sky/pz.png"),
			resources::Image::get("textures/env/sky/nz.png")
			});

		skybox->setFiltering(Filtering::Level1);
		cam->getSkybox()->setCubemap(skybox);


		auto tex = w4::resources::Texture::get("textures/grass.jpg");
		tex->setFiltering(Filtering::Level1);
		auto mat = Material::getDefaultLambert()->createInstance();
		mat->setTexture(TextureId::TEXTURE_0, tex);
		auto mesh = Mesh::create::plane({ 32, 32 }, TRUE);
		mesh->setMaterialInst(mat);
		mesh->setWorldScale({ 16,16,16 });
		mesh->rotateWorld(Rotator(90_deg, 0, 0));
		mesh->translateWorld({ 0, -0.1, 512 / 2 - 10 });
		Render::getRoot()->addChild(mesh);

		playhead = make::sptr<w4::render::Node>();
		playhead->addChild(cam);

		player = make::sptr<Entity>(*hub);
		player->SetMesh("meshes/monkey.w4a"s, "monkey"s);
		player->SetMaterial("default");
		player->Transform().translateWorld({ 0, 1, 0 });
		player->Parent(playhead);

		//for (int i = 0; i < 5; ++i)
		//{
		//	wall.push_back(make::sptr<Entity>(hub));
		//	wall[i]->SetMesh("meshes/wall.w4a"s, "wall"s);
		//	wall[i]->SetMaterial("default"s);
		//	wall[i]->Transform().translateWorld({ -1.5f + (i % 2) * 3, 0, 16.0f * (i + 1) });
		//	wall[i]->Transform().setWorldScale({ 0.5f, 0.5f, 0.5f });
		//	wall[i]->Material().setParam("baseColor"s, color::random());
		//	wall[i]->Material().setParam("specColor"s, math::vec4{ 0,0,0,1 });
		//	wall[i]->Parent(Render::getRoot());
		//}

		Render::getRoot()->addChild(playhead);

		auto lightF = make::sptr<PointLight>("light");
		lightF->setWorldTranslation({ 0.f, 5.f, 2.f });
		lightF->setDecayRate(core::LightDecayRate::Linear);
		lightF->setIntensity(0.5);
		playhead->addChild(lightF);

		auto lightB = make::sptr<PointLight>("light");
		lightB->setWorldTranslation({ 0.f, 5.f, -3.f });
		lightB->setDecayRate(core::LightDecayRate::None);
		lightB->setIntensity(1);
		playhead->addChild(lightB);

		Render::getPass(0)->getDirectionalLight()->setColor(math::vec3(1.f, 1.f, 1.f));
		Render::getPass(0)->getDirectionalLight()->setDirection(math::vec3(1.0f, -1.0f, 1.0f));
		//Render::getPass(0)->getDirectionalLight()->enableShadows();
		/*Render::getRoot()->traversalTyped<PointLight>([this](cref<PointLight> node)
			{
				node->setColor({ 1,1,0 });
			});*/
			//Render::getRoot()->traversalTyped<SpotLight>([this](cref<SpotLight> node)
			//	{
			//		node->setEnabled(FALSE);
			//	});

		//m_road.LoadMeshes("meshes/chunks.w4a"s, 1);
		//m_road.BuildMap(32, Render::getRoot());



		auto ui = createWidget<Widget>(nullptr, "RootUI");

		//m_gui.LoadImages("textures/gui/");

		m_gui.BuildUI(ui, "ui/"s);

		w4::event::Touch::Begin::subscribe(std::bind(&ColorRun::onTouchBegin, this, std::placeholders::_1));
		w4::event::Touch::Move::subscribe(std::bind(&ColorRun::onTouchMove, this, std::placeholders::_1));
		w4::event::Touch::End::subscribe(std::bind(&ColorRun::onTouchEnd, this, std::placeholders::_1));


	}

	void onUpdate(float dt) override
	{
		playhead->translateWorld({ 0, 0, 4 * dt });
		//m_road.Update(playhead->getWorldTranslation().z);
		auto color = std::string("Color: ") + (m_buttons[0] ? "R" : "") + (m_buttons[1] ? "G" : "") + (m_buttons[2] ? "B" : "");
		m_currentColor->setText(color);
		//player->Material().setParam("baseColor", );
		for (Level & Level : Levels)
		{
			Level.Update(playhead->getWorldTranslation().z);
		}
	}

	void onTouchBegin(const event::Touch::Begin & evt)
	{
		vec2 coords{
			static_cast<FLOAT>(evt.point.x) / Platform::getSize().w,
			static_cast<FLOAT>(evt.point.y) / Platform::getSize().h,
		};
		auto color = m_gui.Update(coords);
		if (color.a > 0)
		{
			player->Material()->setParam("baseColor", color);
		}
	}

	void onTouchMove(const event::Touch::Move & evt)
	{
		vec2 coords{
			static_cast<FLOAT>(evt.point.x) / Platform::getSize().w,
			static_cast<FLOAT>(evt.point.y) / Platform::getSize().h,
		};
		auto color = m_gui.Update(coords);
		if (color.a > 0)
		{
			player->Material()->setParam("baseColor", color);
		}
	}

	void onTouchEnd(const event::Touch::End & evt)
	{
		vec2 coords{
			static_cast<FLOAT>(evt.point.x) / Platform::getSize().w,
			static_cast<FLOAT>(evt.point.y) / Platform::getSize().h,
		};
		auto color = m_gui.Update(coords);
		if (color.a > 0)
		{
			player->Material()->setParam("baseColor", color);
		}
		m_gui.Reset();
	}

private:
	w4::sptr<w4::render::Node> camCenter;
	w4::sptr<w4::render::Node> playhead;
	GUIManager m_gui;

	BOOL m_buttons[3];
	w4::sptr<gui::Label>m_currentColor;
	sptr<Hub> hub;
	w4::sptr<Entity> player;
	std::vector<Level> Levels {};
};

W4_RUN(ColorRun)
