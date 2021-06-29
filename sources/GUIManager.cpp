#include "GUIManager.h"
#include "MathUtils.h"
#define b2f(b, i) (((*b)[i]) ? 1.0f : 0.0f)

using namespace std::string_literals;

const std::string GUIManager::Filepath = "ui/"s;

GUIManager::GUIManager()
{
	setVirtualResolution({ cWidth, cHeight });
	Root = createWidget<Widget>(nullptr, "GUIRoot"s);
}

void GUIManager::SetUILevel()
{
	Clear();

	// create current color display
	auto colorLabel = createWidget<Label>(Root, "Current Color:       ");
	colorLabel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 8) });
	colorLabel->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	colorLabel->setHorizontalAlign(HorizontalAlign::Center);
	colorLabel->setVerticalAlign(VerticalAlign::Center);
	colorLabel->setSize({600, 120});
	colorLabel->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
	colorLabel->setFontSize(64);
	colorLabel->setTextAlign(HorizontalAlign::Left, VerticalAlign::Center);

	m_currentColor = createWidget<gui::Image>(Root, Filepath + "000.png", ivec2{ 100, 100 });
	m_currentColor->setHorizontalAlign(HorizontalAlign::Center);
	m_currentColor->setVerticalAlign(VerticalAlign::Center);
	m_currentColor->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	m_currentColor->setPosition({ static_cast<INT>(cWidth * 45 / 64), static_cast<INT>(cHeight / 8) });

	// panel
	auto colorPanel = createWidget<gui::Image>(Root, Filepath + "panel.png", ivec2{ 400, 400 });
	colorPanel->setHorizontalAlign(HorizontalAlign::Center);
	colorPanel->setVerticalAlign(VerticalAlign::Center);
	colorPanel->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	colorPanel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 3 / 4) });
	
	// create color circle
	for (INDEX i = 0; i != 3; ++i)
	{
		std::string filename = "000";
		filename[i] = '1';
		filename = Filepath + filename + ".png";
		m_selectors[i] = createWidget<gui::Image>(Root, filename, ivec2{100, 100});
		m_selectors[i]->setHorizontalAlign(HorizontalAlign::Center);
		m_selectors[i]->setVerticalAlign(VerticalAlign::Center);
		m_selectors[i]->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
		
		FLOAT angle = 90_deg + (i * 120_deg);
		FLOAT radius = 80;
		m_selectorPositions[i] = make::sptr<ivec2>(
			static_cast<INT>(radius * std::cosf(angle) + cWidth / 2),
			static_cast<INT>(radius * std::sinf(angle) + cHeight * 3 / 4)
		);

		m_selectors[i]->setPosition(*m_selectorPositions[i]);
	}
	
	m_currentColor->setImage("ui/111.png");
	m_currentColor->setSize({ 100, 100 });

	m_combo = make::sptr<std::bitset<3>>();
	m_combo->reset();
}

void GUIManager::SetUIMenu()
{
	Clear();

	// create background image
	sptr<Widget> Background = gui::createWidget<gui::Image>(Root, Filepath + "bg.png", ivec2{ 1080, 1920 });
	Background->setHorizontalAlign(HorizontalAlign::Center);
	Background->setVerticalAlign(VerticalAlign::Center);
	Background->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	Background->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 2) });

	// create welcome text
	sptr<Label> Title = createWidget<Label>(Root, "COLOR RUN");
	Title->setHorizontalAlign(HorizontalAlign::Center);
	Title->setVerticalAlign(VerticalAlign::Center);
	Title->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	Title->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Title->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 8) });
	//Title->setSize({600, 150});
	Title->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
	Title->setFontSize(72);

	// create game inviting text
	sptr<Label> Subtitle = createWidget<Label>(Root, "Choose level");
	Subtitle->setHorizontalAlign(HorizontalAlign::Center);
	Subtitle->setVerticalAlign(VerticalAlign::Center);
	Subtitle->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	Subtitle->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Subtitle->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 2 / 8) });
	//Subtitle->setSize({600, 150});
	Subtitle->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
	Subtitle->setFontSize(48);

	constexpr SIZE NumLevels = 5;
	for (INDEX i = 0; i != NumLevels; ++i)
	{
		sptr<Button> Button = createWidget<class Button>(Root, utils::format("Level %d", i + 1).c_str());
		Button->setHorizontalAlign(HorizontalAlign::Center);
		Button->setVerticalAlign(VerticalAlign::Center);
		//ReplayButton->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
		Button->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
		Button->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * (3 + i) / 8) });
		Button->setSize({600, 150});
		Button->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
		Button->setFontSize(64);

		Button->onTap([this, i]() 
					  {
						  NextState = make::sptr<EGameState>(EGameState::GameStateLevel);
						  NextLevel = make::sptr<INDEX>(i + 1);
					  });
	}
}

void GUIManager::SetUIResult(INDEX CurrentLevel, INT Result)
{
	Clear();

	// create background image
	sptr<Widget> Background = gui::createWidget<gui::Image>(Root, Filepath + "bg.png", ivec2{ 1080, 1920 });
	Background->setHorizontalAlign(HorizontalAlign::Center);
	Background->setVerticalAlign(VerticalAlign::Center);
	Background->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	Background->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 2) });
	Background->setOpacity(0.75f);

	// create welcome text
	std::string ResultText = (Result > 0) ? "LEVEL COMPLETE"s : "GAME OVER"s;
	sptr<Label> Title = createWidget<Label>(Root, ResultText);
	Title->setHorizontalAlign(HorizontalAlign::Center);
	Title->setVerticalAlign(VerticalAlign::Center);
	Title->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	Title->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Title->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 8) });
	//Title->setSize({600, 150});
	Title->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
	Title->setFontSize(72);

	// create score text
	if (Result > 0)
	{
		sptr<Label> Subtitle = createWidget<Label>(Root, utils::format("Your result: %d", Result));
		Subtitle->setHorizontalAlign(HorizontalAlign::Center);
		Subtitle->setVerticalAlign(VerticalAlign::Center);
		Subtitle->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
		Subtitle->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
		Subtitle->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 2 / 8) });
		//Subtitle->setSize({600, 150});
		Subtitle->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
		Subtitle->setFontSize(48);
	}

	// replay button
	sptr<Button> ReplayButton = createWidget<class Button>(Root, utils::format("Replay Level %d", CurrentLevel).c_str());
	ReplayButton->setHorizontalAlign(HorizontalAlign::Center);
	ReplayButton->setVerticalAlign(VerticalAlign::Center);
	//ReplayButton->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	ReplayButton->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	ReplayButton->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 4 / 8) });
	//ReplayButton->setSize({600, 150});
	ReplayButton->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
	ReplayButton->setFontSize(64);

	ReplayButton->onTap([this, CurrentLevel]() 
					{
						NextState = make::sptr<EGameState>(EGameState::GameStateLevel);
						NextLevel = make::sptr<INDEX>(CurrentLevel);
					});

	// return to menu button
	sptr<Button> MenuButton = createWidget<class Button>(Root, "Back to Menu");
	MenuButton->setHorizontalAlign(HorizontalAlign::Center);
	MenuButton->setVerticalAlign(VerticalAlign::Center);
	//MenuButton->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	MenuButton->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	MenuButton->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 5 / 8) });
	//MenuButton->setSize({600, 150});
	MenuButton->setFont("https://fonts.googleapis.com/css2?family=Roboto", "Roboto");
	MenuButton->setFontSize(64);

	MenuButton->onTap([this]() 
					{
						NextState = make::sptr<EGameState>(EGameState::GameStateMenu);
					});
}

std::optional<vec4> GUIManager::UpdateUILevel(vec2 cursor, const BOOL InIsReleased)
{
	for (INDEX i = 0; i != 3; ++i)
	{
		ivec2 virtualCursor{ 
			static_cast<INT>(cursor.w * cWidth),
			static_cast<INT>(cursor.h * cHeight)
		};
		if (std::hypotf(m_selectorPositions[i]->x - virtualCursor.x, m_selectorPositions[i]->y - virtualCursor.y) <= 50)
		{
			// bit ordering is reversed
			if (!m_combo->test(2 - i))
			{
				m_combo->set(2 - i);
				if (m_combo->all())
				{
					m_currentColor->setImage("ui/000.png");
					if (InIsReleased) { m_combo->reset(); }
					return std::make_optional<vec4>(0.f, 0.f, 0.f, 1.f);
				}
				else
				{
					m_currentColor->setImage("ui/" + m_combo->to_string() + ".png");
					if (InIsReleased) { m_combo->reset(); }
					return std::make_optional<vec4>(b2f(m_combo, 2), b2f(m_combo, 1), b2f(m_combo, 0), 1.f);
				}
			}
		}
	}
	if (InIsReleased) { m_combo->reset(); }
	return std::nullopt;
}

BOOL GUIManager::RequestStateSwitch(EGameState & OutNextState, INDEX & OutNextLevel)
{
	if (NextState != nullptr)
	{
		OutNextState = *NextState;
		OutNextLevel = *NextLevel;
		NextState = nullptr;
		NextLevel = nullptr;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void GUIManager::Clear()
{
	for (INDEX i = 0; i != 3; ++ i)
	{
		m_selectors[i] = nullptr;
		m_selectorPositions[i] = nullptr;
	}
	m_currentColor = nullptr;
	m_combo = nullptr;

	removeAllWidgets();
}
