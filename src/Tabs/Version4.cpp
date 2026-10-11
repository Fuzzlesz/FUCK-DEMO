#include "DEMO.h"
#include "API/IconsFontAwesome6.h"

void DemoState::DrawVers4Page()
{
	// --- Sliders ---
	FUCK::Header("$DEMO_NewSliders"_T);

	static float angle = 0.0f;
	FUCK::SliderAngle("$DEMO_SliderAngle"_T, &angle);

	FUCK::Spacing();
	FUCK::TextWrapped("$DEMO_VSliderDesc"_T);
	FUCK::Spacing();

	constexpr float DEFAULT_VAL = 50.0f;

	static float vPlain = DEFAULT_VAL;
	static float vReset = DEFAULT_VAL;
	static float vMax   = DEFAULT_VAL;
	static float vBoth  = DEFAULT_VAL;

	// The same slider size lines them all up, as missing buttons keep their space
	ImVec2 sliderSize = FUCK::Scale(40.0f, 150.0f);
	bool   top        = false;
	bool   bottom     = false;

	// Centre the row using its width from last frame, as it isn't known until it's drawn
	static float rowWidth = 0.0f;
	FUCK::SetCursorPosX(FUCK::GetCursorPos().x + std::max(0.0f, (FUCK::GetContentRegionAvail().x - rowWidth) * 0.5f));
	FUCK::BeginGroup();

	// A button is drawn when its label is set
	FUCK::VSliderButton("Plain", sliderSize, &vPlain, 0.0f, 100.0f, "%.0f");
	FUCK::SameLine();

	FUCK::VSliderButton("Reset", sliderSize, &vReset, 0.0f, 100.0f, "%.0f", nullptr, ICON_FA_ARROW_ROTATE_LEFT, nullptr, &bottom);
	if (bottom)
		vReset = DEFAULT_VAL;
	FUCK::SameLine();

	FUCK::VSliderButton("Max", sliderSize, &vMax, 0.0f, 100.0f, "%.0f", ICON_FA_ARROW_UP, nullptr, &top);
	if (top)
		vMax = 100.0f;
	FUCK::SameLine();

	FUCK::VSliderButton("Both", sliderSize, &vBoth, 0.0f, 100.0f, "%.0f", ICON_FA_ARROW_UP, ICON_FA_ARROW_DOWN, &top, &bottom);
	if (top)
		vBoth = 100.0f;
	if (bottom)
		vBoth = 0.0f;

	FUCK::EndGroup();
	rowWidth = FUCK::GetItemRectMax().x - FUCK::GetItemRectMin().x;
}
