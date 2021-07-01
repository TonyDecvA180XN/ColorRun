#include <W4Framework.h>
#include <functional>
#include <string>
#include "MathUtils.h"
#include "DebugHelper.h"
#include "Level.h"
#include "GUIManager.h"
#include "Hub.h"
#include "Entity.h"
#include "GameState.h"

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
		
		Hub = make::sptr<class Hub>(Render::getRoot(), ClockCounter);

		ChangeState(EGameState::GameStateMenu, 0);
	}

	void ChangeState(EGameState NextState, INT Param)
	{
		// some event BS
		static sptr<EventImpl<Touch::Begin, touchProxy>::Handle> HandlerBegin;
		static sptr<EventImpl<Touch::Move, touchProxy>::Handle> HandlerMove;
		static sptr<EventImpl<Touch::End, touchProxy>::Handle> HandlerEnd;

		if (NextState == EGameState::GameStateNone)
		{
			// Invalid case
			W4_LOG_ERROR("Switching to NoneState attempt.");
			return;
		}
		else if (CurrentState == EGameState::GameStateNone && NextState == EGameState::GameStateMenu)
		{
			// Initial condition
			W4_LOG_DEBUG("Initial menu opening.");
			GUI.SetUIMenu();
		}
		else if (CurrentState == EGameState::GameStateMenu && NextState == EGameState::GameStateLevel)
		{
			// Launch level
			W4_LOG_DEBUG("Launching level.");
			GUI.SetUILevel();
			CurrentLevel = make::uptr<Level>(*Hub, Param);
			CurrentLevelNumber = Param;
			HandlerBegin = Touch::Begin::subscribe(std::bind(&ColorRun::onTouchBegin, this, std::placeholders::_1));
			HandlerMove = Touch::Move::subscribe(std::bind(&ColorRun::onTouchMove, this, std::placeholders::_1));
			HandlerEnd = Touch::End::subscribe(std::bind(&ColorRun::onTouchEnd, this, std::placeholders::_1));
		}
		else if (CurrentState == EGameState::GameStateLevel && NextState == EGameState::GameStateResult)
		{
			// finish level and show result
			W4_LOG_DEBUG("Finishing level.");
			GUI.SetUIResult(CurrentLevelNumber, Param);
			CurrentLevel = nullptr;
			CurrentLevelNumber = 0;
			HandlerBegin->unsubscribe();
			HandlerMove->unsubscribe();
			HandlerEnd->unsubscribe();
		}
		else if (CurrentState == EGameState::GameStateResult && NextState == EGameState::GameStateLevel)
		{
			// restart level
			W4_LOG_DEBUG("Restarting level.");
			GUI.SetUILevel();
			CurrentLevel = nullptr;
			CurrentLevel = make::uptr<Level>(*Hub, Param);
			CurrentLevelNumber = Param;
			HandlerBegin = Touch::Begin::subscribe(std::bind(&ColorRun::onTouchBegin, this, std::placeholders::_1));
			HandlerMove = Touch::Move::subscribe(std::bind(&ColorRun::onTouchMove, this, std::placeholders::_1));
			HandlerEnd = Touch::End::subscribe(std::bind(&ColorRun::onTouchEnd, this, std::placeholders::_1));
		}
		else if (CurrentState == EGameState::GameStateResult && NextState == EGameState::GameStateMenu)
		{
			// back to menu
			W4_LOG_DEBUG("Back to menu.");
			GUI.SetUIMenu();
			CurrentLevel = nullptr;
		}
		
		CurrentState = NextState;
	}

	void onUpdate(float dt) override
	{
		ClockCounter += dt;
		Hub->Update(ClockCounter);
		
		EGameState NextState = EGameState::GameStateNone;
		INDEX NextLevel = 0;
		BOOL NeedStateSwitch = FALSE;
		switch (CurrentState)
		{
			case EGameState::GameStateLevel:
			{
				NextLevel = CurrentLevel->Update(dt);
				NeedStateSwitch = NextLevel != 0;
				if (NeedStateSwitch)
				{
					NextState = EGameState::GameStateResult;
				}
				break;
			}
			case EGameState::GameStateMenu:
			{
				GUI.UpdateUIMenu();
				NeedStateSwitch = GUI.RequestStateSwitch(NextState, NextLevel);
				break;
			}
			case EGameState::GameStateResult:
			{
				GUI.UpdateUIResult();
				NeedStateSwitch = GUI.RequestStateSwitch(NextState, NextLevel);
				break;
			}
			default:
			{
				break;
			}
		}

		if (NeedStateSwitch)
		{
			ChangeState(NextState, NextLevel);
		}
	}

	void ProcessCombo(const point & InCursor, const BOOL InIsReleased)
	{
		vec2 coords
		{
			static_cast<FLOAT>(InCursor.x) / Platform::getSize().w,
			static_cast<FLOAT>(InCursor.y) / Platform::getSize().h,
		};

		std::optional<vec4> UpdatedColor = GUI.UpdateUILevel(coords, InIsReleased);
		if (UpdatedColor.has_value())
		{
			CurrentColor = UpdatedColor.value();
			CurrentLevel->OnColorChanged(CurrentColor);
		}
	}

	void onTouchBegin(const event::Touch::Begin & evt)
	{
		ProcessCombo(evt.point, FALSE);
	}

	void onTouchMove(const event::Touch::Move & evt)
	{
		ProcessCombo(evt.point, FALSE);
	}

	void onTouchEnd(const event::Touch::End & evt)
	{
		ProcessCombo(evt.point, TRUE);
	}

private:
	GUIManager GUI;
	sptr<Hub> Hub;

	FLOAT ClockCounter { 0.f };
	EGameState CurrentState { EGameState::GameStateNone };
	INDEX CurrentLevelNumber { 0 };
	
	uptr<Level> CurrentLevel { nullptr };
	vec4 CurrentColor { 0.f, 0.f, 0.f, 0.f };
};

W4_RUN(ColorRun)
