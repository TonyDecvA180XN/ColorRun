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

	sptr<gui::Image> ColorBG = createWidget<class gui::Image>(Root, "ui/button.png", ivec2{ 700, 180 });
	ColorBG->setHorizontalAlign(HorizontalAlign::Center);
	ColorBG->setVerticalAlign(VerticalAlign::Center);
	ColorBG->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	ColorBG->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 2 / 16) });

	// create current color display
	auto colorLabel = createWidget<Label>(Root, "Current Color:");
	colorLabel->setPosition({ static_cast<INT>(cWidth * 15 / 32), static_cast<INT>(cHeight * 2 / 16) });
	colorLabel->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	colorLabel->setHorizontalAlign(HorizontalAlign::Center);
	colorLabel->setVerticalAlign(VerticalAlign::Center);
	colorLabel->setSize({600, 120});
	colorLabel->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
	colorLabel->setFontSize(56);
	colorLabel->setTextAlign(HorizontalAlign::Left, VerticalAlign::Center);
	colorLabel->setBgColor({ 0, 0, 0, 0 });

	m_currentColor = createWidget<gui::Image>(Root, Filepath + "000.png", ivec2{ 100, 100 });
	m_currentColor->setHorizontalAlign(HorizontalAlign::Center);
	m_currentColor->setVerticalAlign(VerticalAlign::Center);
	m_currentColor->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	m_currentColor->setPosition({ static_cast<INT>(cWidth * 46 / 64), static_cast<INT>(cHeight * 2 / 16) });
	m_currentColor->setOpacity(1.f);

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
		m_selectors[i]->setOpacity(0.9f);
		
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
	sptr<gui::Image> Title = createWidget<gui::Image>(Root, "ui/logo.png", ivec2{ static_cast<INT>(1902.f/1520*750), 750});
	Title->setHorizontalAlign(HorizontalAlign::Center);
	Title->setVerticalAlign(VerticalAlign::Center);
	Title->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Title->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 2 / 8) });
	//Title->setSize({ static_cast<INT>(1902.f/1520*750), 750});

	constexpr SIZE NumLevels = 5;
	for (INDEX i = 0; i != NumLevels; ++i)
	{
		sptr<ImageButton> Button = createWidget<class ImageButton>(Root, "ui/button.png", "ui/button_pressed.png", ivec2{ 450, 150 });
		Button->setHorizontalAlign(HorizontalAlign::Center);
		Button->setVerticalAlign(VerticalAlign::Center);
		Button->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
		Button->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * (17 + 3 * i) / 32) });

		Button->onTap([this, i]() 
					  {
						  NextState = make::sptr<EGameState>(EGameState::GameStateLevel);
						  NextLevel = make::sptr<INDEX>(i + 1);
					  });

		sptr<Label> LevelLabel = createWidget<class Label>(Root, utils::format("Level %d", i + 1).c_str());
		LevelLabel->setHorizontalAlign(HorizontalAlign::Center);
		LevelLabel->setVerticalAlign(VerticalAlign::Center);
		//ReplayButton->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
		LevelLabel->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
		LevelLabel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * (17 + 3 * i) / 32) });
		LevelLabel->setSize({600, 150});
		LevelLabel->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
		LevelLabel->setFontSize(56);
		LevelLabel->setBgColor({ 0, 0, 0, 0 });
	}
}

void GUIManager::SetUIResult(INDEX CurrentLevel, INT Result)
{
	Clear();

	// create background image
	sptr<Widget> Background = gui::createWidget<gui::Image>(Root, "ui/bg.png", ivec2{ 1080, 1920 });
	Background->setHorizontalAlign(HorizontalAlign::Center);
	Background->setVerticalAlign(VerticalAlign::Center);
	Background->setSizePolicy(SizePolicy::Fixed, SizePolicy::Fixed);
	Background->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight / 2) });
	//Background->setOpacity(0.5f);

	// create result plate
	sptr<gui::Image> Plate = createWidget<gui::Image>(Root, "ui/plate.png", ivec2{ 900, 800 });
	Plate->setHorizontalAlign(HorizontalAlign::Center);
	Plate->setVerticalAlign(VerticalAlign::Center);
	Plate->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Plate->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 17 / 32) });

	// create welcome text
	std::string ResultText = (Result > 0) ? "LEVEL"s : "GAME"s;
	sptr<Label> Title1 = createWidget<Label>(Root, ResultText);
	Title1->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Title1->setHorizontalAlign(HorizontalAlign::Center);
	Title1->setVerticalAlign(VerticalAlign::Center);
	Title1->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	Title1->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 6 / 16) });
	Title1->setSize({ 800, 200 });
	Title1->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
	Title1->setFontSize(84);
	Title1->setBgColor({ 0, 0, 0, 0 });

	ResultText = (Result > 0) ? "COMPLETE"s : "OVER"s;
	sptr<Label> Title2 = createWidget<Label>(Root, ResultText);
	Title2->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	Title2->setHorizontalAlign(HorizontalAlign::Center);
	Title2->setVerticalAlign(VerticalAlign::Center);
	Title2->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	Title2->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 7 / 16) });
	Title2->setSize({ 800, 200 });
	Title2->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
	Title2->setFontSize(84);
	Title2->setBgColor({ 0, 0, 0, 0 });

	// create score text
	if (Result > 0)
	{
		sptr<Label> Subtitle = createWidget<Label>(Root, utils::format("Score: %d", Result));
		Subtitle->setHorizontalAlign(HorizontalAlign::Center);
		Subtitle->setVerticalAlign(VerticalAlign::Center);
		Subtitle->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
		Subtitle->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
		Subtitle->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 8 / 16) });
		//Subtitle->setSize({600, 150});
		Subtitle->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
		Subtitle->setFontSize(64);
		Subtitle->setBgColor({ 0, 0, 0, 0 });
	}

	sptr<ImageButton> ReplayPlate = createWidget<class ImageButton>(Root, "ui/button.png", "ui/button_pressed.png", ivec2{ 500, 120 });
	ReplayPlate->setHorizontalAlign(HorizontalAlign::Center);
	ReplayPlate->setVerticalAlign(VerticalAlign::Center);
	ReplayPlate->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	ReplayPlate->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 9 / 16) });

	ReplayPlate->onTap([this, CurrentLevel]() 
					{
						NextState = make::sptr<EGameState>(EGameState::GameStateLevel);
						NextLevel = make::sptr<INDEX>(CurrentLevel + 1);
					});

	// next Label
	std::string replayText = (Result > 0) ? "Next Level" : "Replay Level";
	sptr<Label> ReplayLabel = createWidget<class Label>(Root, replayText);
	ReplayLabel->setHorizontalAlign(HorizontalAlign::Center);
	ReplayLabel->setVerticalAlign(VerticalAlign::Center);
	//ReplayLabel->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	ReplayLabel->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	ReplayLabel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 9 / 16) });
	//ReplayLabel->setSize({600, 150});
	ReplayLabel->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
	ReplayLabel->setFontSize(56);
	ReplayLabel->setBgColor({ 0, 0, 0, 0 });


	// to menu button
	sptr<ImageButton> MenuButton = createWidget<class ImageButton>(Root, "ui/button.png", "ui/button_pressed.png", ivec2{ 500, 120 });
	MenuButton->setHorizontalAlign(HorizontalAlign::Center);
	MenuButton->setVerticalAlign(VerticalAlign::Center);
	MenuButton->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	MenuButton->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 41 / 64) });

	MenuButton->onTap([this]() 
					{
						NextState = make::sptr<EGameState>(EGameState::GameStateMenu);
					});

	// return to menu Label
	sptr<Label> MenuLabel = createWidget<class Label>(Root, "Back to Menu");
	MenuLabel->setHorizontalAlign(HorizontalAlign::Center);
	MenuLabel->setVerticalAlign(VerticalAlign::Center);
	//MenuLabel->setTextAlign(HorizontalAlign::Center, VerticalAlign::Center);
	MenuLabel->setSizePolicy(SizePolicy::Auto, SizePolicy::Auto);
	MenuLabel->setPosition({ static_cast<INT>(cWidth / 2), static_cast<INT>(cHeight * 41 / 64) });
	//MenuLabel->setSize({600, 150});
	MenuLabel->setFont("https://fonts.googleapis.com/css2?family=Peralta", "Peralta");
	MenuLabel->setFontSize(56);
	MenuLabel->setBgColor({ 0, 0, 0, 0 });

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
