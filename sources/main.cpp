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

		CurrentLevel = &Levels[0];
		
		


		auto ui = createWidget<Widget>(nullptr, "RootUI");

		//m_gui.LoadImages("textures/gui/");

		m_gui.BuildUI(ui, "ui/"s);

		w4::event::Touch::Begin::subscribe(std::bind(&ColorRun::onTouchBegin, this, std::placeholders::_1));
		w4::event::Touch::Move::subscribe(std::bind(&ColorRun::onTouchMove, this, std::placeholders::_1));
		w4::event::Touch::End::subscribe(std::bind(&ColorRun::onTouchEnd, this, std::placeholders::_1));


	}

	void onUpdate(float dt) override
	{
		ClockCounter += dt;
		
		for (Level & Level : Levels)
		{
			Level.Update(dt);
		}
	}

	void ProcessCombo(const point & InCursor)
	{
		vec2 coords
		{
			static_cast<FLOAT>(InCursor.x) / Platform::getSize().w,
			static_cast<FLOAT>(InCursor.y) / Platform::getSize().h,
		};

		std::optional<vec4> UpdatedColor = m_gui.Update(coords);
		if (UpdatedColor.has_value())
		{
			CurrentColor = UpdatedColor.value();
			CurrentLevel->OnColorChanged(CurrentColor);
		}
	}

	void onTouchBegin(const event::Touch::Begin & evt)
	{
		ProcessCombo(evt.point);
	}

	void onTouchMove(const event::Touch::Move & evt)
	{
		ProcessCombo(evt.point);
	}

	void onTouchEnd(const event::Touch::End & evt)
	{
		ProcessCombo(evt.point);
		m_gui.Reset();
	}

private:
	GUIManager m_gui;

	sptr<Hub> hub;
	std::vector<Level> Levels {};
	Level * CurrentLevel { nullptr };
	FLOAT ClockCounter { 0.f };
	vec4 CurrentColor { 0.f, 0.f, 0.f, 0.f };
};

W4_RUN(ColorRun)
