#pragma once

#include "W4Framework.h"
#include "Type.h"

class GUIManager
{
public:
	static constexpr SIZE cWidth = 1080, cHeight = 1920;

	void BuildUI(w4::sptr<w4::gui::Widget> root, std::string path);

	w4::math::vec4 Update(w4::math::vec2 cursor);

	void Reset();

private:
	w4::sptr<w4::gui::Image> m_selectors[3];
	w4::sptr<w4::gui::Image> m_currentColor;

	w4::math::ivec2 m_selectorPositions[3];
	std::bitset<3> m_combo;
};