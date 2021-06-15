#include "GUIManager.h"
#include "MathUtils.h"

void GUIManager::BuildUI(w4::sptr<w4::gui::Widget> root, std::string path)
{
	static constexpr SIZE cWidth = 1080, cHeight = 1920;

	w4::gui::setVirtualResolution({ cWidth, cHeight });
	
	// create current color display
	std::string filename = "000";
	m_currentColor = w4::gui::createWidget<w4::gui::ImageButton>(root, path + filename + ".png", path + filename + ".png", w4::math::ivec2{ 100, 100 });
	m_currentColor->setHorizontalAlign(w4::gui::HorizontalAlign::Center);
	m_currentColor->setVerticalAlign(w4::gui::VerticalAlign::Center);
	m_currentColor->setSizePolicy(w4::gui::SizePolicy::Fixed, w4::gui::SizePolicy::Fixed);
	m_currentColor->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 4) });
	
	// create color circle
	for (INDEX i = 0; i != 3; ++i)
	{
		std::string filename = "000";
		filename[i] = '1';
		filename = path + filename + ".png";
		m_selectors[i] = w4::gui::createWidget<w4::gui::ImageButton>(root, filename, filename, w4::math::ivec2{100, 100});
		m_selectors[i]->setHorizontalAlign(w4::gui::HorizontalAlign::Center);
		m_selectors[i]->setVerticalAlign(w4::gui::VerticalAlign::Center);
		m_selectors[i]->setSizePolicy(w4::gui::SizePolicy::Fixed, w4::gui::SizePolicy::Fixed);
		
		FLOAT angle = 90_deg + (i * 120_deg);
		FLOAT radius = 100;
		m_selectors[i]->setPosition(
			{ 
			static_cast<INT>(radius * std::cosf(angle) + cWidth / 2),
			static_cast<INT>(radius * std::sinf(angle) + cHeight * 3 / 4)
			});
		m_selectors[i]->onTap([this, filename]()
			{
				this->m_currentColor->setImage(filename);
			});
	}
}
