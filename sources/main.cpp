#include "W4Framework.h"
#include "DebugHelper.h"

W4_USE_UNSTRICT_INTERFACE

class ColorRun : public IGame
{
public:
	void onStart() override
	{
		DebugHelper::buildGizmo();
		auto cam = Render::getScreenCamera();
		cam->setWorldTranslation({ 5.0f, 5.0f, -5.0f });
		cam->setWorldRotation(Rotator(math::PI / 4, -math::PI / 4, 0.0f));
	}

	void onUpdate(float dt) override
	{

	}
private:
};

W4_RUN(ColorRun)
