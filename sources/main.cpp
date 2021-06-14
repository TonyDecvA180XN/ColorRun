#include "W4Framework.h"
#include "MathUtils.h"
#include "DebugHelper.h"
#include "Level.h"

W4_USE_UNSTRICT_INTERFACE

class ColorRun : public IGame
{
public:
	void onStart() override
	{
		DebugHelper::buildGizmo();
		auto cam = Render::getScreenCamera();
		cam->setFov(80.0f);
		cam->setWorldTranslation({ 0.0f, 10.0f, -10.0f });
		cam->setWorldRotation(Rotator(45_deg, 0, 0.0f));
		cam->setClearMask(ClearMask::Color | ClearMask::Depth | ClearMask::Skybox);
		//camCenter = make::sptr<w4::render::Node>();
		//camCenter->setWorldTranslation({ 0, 5.0f, 0 });
		//camCenter->addChild(cam);
		//Render::getRoot()->addChild(camCenter);

		auto skybox = Cubemap::fromImages({
			resources::Image::get("textures/env/field/px.png"),
			resources::Image::get("textures/env/field/nx.png"),
			resources::Image::get("textures/env/field/py.png"),
			resources::Image::get("textures/env/field/ny.png"),
			resources::Image::get("textures/env/field/pz.png"),
			resources::Image::get("textures/env/field/nz.png"),
			});
		cam->getSkybox()->setCubemap(skybox);

		playhead = make::sptr<w4::render::Node>();
		playhead->addChild(cam);

		auto player = Asset::get("meshes/monkey.w4a")->getFirstRoot();
		player->translateWorld({ 0, 1, 0 });
		playhead->addChild(player);

		Render::getRoot()->addChild(playhead);

		auto light = make::sptr<PointLight>("light");
		light->setColor({0, 0, 1});
		light->setDecayRate(LightDecayRate::Linear);
		light->setIntensity(0.5f);
		light->setDebugViewColor({ 1,1,1,1 }).enableDebugView(true);
		light->translateWorld({ 3, 3, 2 });
		playhead->addChild(light);

		Render::getPass(0)->getDirectionalLight()->setColor(math::vec3(1.0f, 0.5f, 0.0f));
		Render::getPass(0)->getDirectionalLight()->setDirection(math::vec3(1.0f, -1.0f, -1.0f));
		Render::getPass(0)->getDirectionalLight()->enableShadows();

		m_level.LoadMeshes("meshes/chunks.w4a", 4);
		m_level.BuildMap(16, Render::getRoot());
	}

	void onUpdate(float dt) override
	{
		//camCenter->rotateWorld(Rotator(0, dt / 2, 0));
		playhead->translateWorld({ 0, 0, 10 * dt });
		//m_level.Update(playhead->getWorldTranslation().z);
	}
private:
	w4::sptr<w4::render::Node> camCenter;
	w4::sptr<w4::render::Node> playhead;
	Level m_level;
};

W4_RUN(ColorRun)
