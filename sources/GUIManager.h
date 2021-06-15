#pragma once

#include "W4Framework.h"
#include "Type.h"

class GUIManager
{
public:
	void BuildUI(w4::sptr<w4::gui::Widget> root, std::string path);

private:
	w4::sptr<w4::gui::ImageButton> m_selectors[3];
	w4::sptr<w4::gui::ImageButton> m_currentColor;
};