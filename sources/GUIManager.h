#pragma once

#include "W4Framework.h"
#include <optional>
#include "Type.h"
#include "GameState.h"

W4_USE_UNSTRICT_INTERFACE

class GUIManager
{
public:
	static constexpr SIZE cWidth = 1080, cHeight = 1920;
	static const std::string Filepath;

	GUIManager();

	void SetUILevel();
	void SetUIMenu();
	void SetUIResult(INDEX CurrentLevel, INT Result);

	std::optional<math::vec4> UpdateUILevel(vec2 cursor, const BOOL InIsReleased);
	void UpdateUIMenu() {};
	void UpdateUIResult() {};

	BOOL RequestStateSwitch(EGameState & OutNextState, INDEX & OutNextLevel);

	void Clear();

private:
	sptr<gui::Widget> Root { nullptr };

	sptr<EGameState> NextState { nullptr };
	sptr<INDEX> NextLevel { 0 };

	sptr<gui::Image> m_selectors[3] = { nullptr, nullptr, nullptr };
	sptr<math::ivec2> m_selectorPositions[3] = { nullptr, nullptr, nullptr };
	sptr<gui::Image> m_currentColor = { nullptr };
	sptr<std::bitset<3>> m_combo = { nullptr };
};