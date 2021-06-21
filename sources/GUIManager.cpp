#include "GUIManager.h"
#include "MathUtils.h"
#define b2f(b) (b ? 1.0f : 0.0f)

using namespace std::string_literals;

void GUIManager::BuildUI(w4::sptr<w4::gui::Widget> root, std::string path)
{
    w4::gui::setVirtualResolution({ cWidth, cHeight });
    
    // create current color display
    auto colorLabel = w4::gui::createWidget<w4::gui::Label>(root, "Current Color:       ");
    colorLabel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 8) });
    colorLabel->setSizePolicy(w4::gui::SizePolicy::Fixed, w4::gui::SizePolicy::Fixed);
    colorLabel->setHorizontalAlign(w4::gui::HorizontalAlign::Center);
    colorLabel->setVerticalAlign(w4::gui::VerticalAlign::Center);
    colorLabel->setSize({600, 120});
    colorLabel->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
    colorLabel->setFontSize(64);
    colorLabel->setTextAlign(w4::gui::HorizontalAlign::Left, w4::gui::VerticalAlign::Center);

    m_currentColor = w4::gui::createWidget<w4::gui::Image>(root, path + "000.png", w4::math::ivec2{ 100, 100 });
    m_currentColor->setHorizontalAlign(w4::gui::HorizontalAlign::Center);
    m_currentColor->setVerticalAlign(w4::gui::VerticalAlign::Center);
    m_currentColor->setSizePolicy(w4::gui::SizePolicy::Fixed, w4::gui::SizePolicy::Fixed);
    m_currentColor->setPosition({ static_cast<INT>(cWidth * 45 / 64), static_cast<INT>(cHeight / 8) });

    // panel
    auto colorPanel = w4::gui::createWidget<w4::gui::Image>(root, path + "panel.png", w4::math::ivec2{ 400, 400 });
    colorPanel->setHorizontalAlign(w4::gui::HorizontalAlign::Center);
    colorPanel->setVerticalAlign(w4::gui::VerticalAlign::Center);
    colorPanel->setSizePolicy(w4::gui::SizePolicy::Fixed, w4::gui::SizePolicy::Fixed);
    colorPanel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 3 / 4) });
    
    // create color circle
    for (INDEX i = 0; i != 3; ++i)
    {
        std::string filename = "000";
        filename[i] = '1';
        filename = path + filename + ".png";
        m_selectors[i] = w4::gui::createWidget<w4::gui::Image>(root, filename, w4::math::ivec2{100, 100});
        m_selectors[i]->setHorizontalAlign(w4::gui::HorizontalAlign::Center);
        m_selectors[i]->setVerticalAlign(w4::gui::VerticalAlign::Center);
        m_selectors[i]->setSizePolicy(w4::gui::SizePolicy::Fixed, w4::gui::SizePolicy::Fixed);
        
        FLOAT angle = 90_deg + (i * 120_deg);
        FLOAT radius = 80;
        m_selectorPositions[i] = {
            static_cast<INT>(radius * std::cosf(angle) + cWidth / 2),
            static_cast<INT>(radius * std::sinf(angle) + cHeight * 3 / 4)
        };

        m_selectors[i]->setPosition(m_selectorPositions[i]);
    }
    
    m_currentColor->setImage("ui/111.png");
    m_currentColor->setSize({ 100, 100 });

    m_combo.reset();
}

std::optional<w4::math::vec4> GUIManager::Update(w4::math::vec2 cursor)
{
    for (INDEX i = 0; i != 3; ++i)
    {
        w4::math::ivec2 virtualCursor{ 
            static_cast<INT>(cursor.w * cWidth),
            static_cast<INT>(cursor.h * cHeight)
        };
        if (std::hypotf(m_selectorPositions[i].x - virtualCursor.x, m_selectorPositions[i].y - virtualCursor.y) <= 50)
        {
            // bit ordering is reversed
            if (!m_combo.test(2 - i))
            {
                m_combo.set(2 - i);
                if (m_combo.all())
                {
                    m_currentColor->setImage("ui/000.png");
					return std::make_optional<w4::math::vec4>(0.f, 0.f, 0.f, 1.f);
                }
                else
                {
                    m_currentColor->setImage("ui/" + m_combo.to_string() + ".png");
					return std::make_optional<w4::math::vec4>(b2f(m_combo[2]), b2f(m_combo[1]), b2f(m_combo[0]), 1.f);
                }
            }
        }
    }
    return std::nullopt;
}

void GUIManager::Reset()
{
    m_combo.reset();
}
