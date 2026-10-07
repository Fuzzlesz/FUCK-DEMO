#include "DEMO.h"
#include "System/IconsFontAwesome6.h"

void DemoState::DrawVers456Tab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabVers456"_T)) {
		return;
	}

	// --- Sliders (v4/v5) ---
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

	FUCK::Separator();

	// --- Panel Frame & Scaling (v5) ---
	FUCK::Header("$DEMO_PanelFrame"_T);
	FUCK::TextWrapped("$DEMO_PanelDesc"_T);

	FUCK::BeginPanelFrame("DemoPanel");
	FUCK::TextWrapped("$DEMO_PushScaleDesc"_T);
	FUCK::Spacing();

	// Scales font size, frame padding, item spacing and rounding for everything until PopScale
	FUCK::PushScale(1.25f);
	static float scaledVal = 10.0f;

	// Stored as 10, shown as 100
	FUCK::ScaledSliderFloat("$DEMO_ScaledSlider"_T, &scaledVal, 0.0f, 100.0f, 10.0f);
	FUCK::PopScale();
	FUCK::EndPanelFrame();

	FUCK::Separator();

	// --- Gamepad Tweaks (v5) ---
	FUCK::Header("$DEMO_GamepadTweaks"_T);
	FUCK::TextWrapped("$DEMO_GamepadTweaksDesc"_T);
	FUCK::Spacing();

	static float gpVal = 50.0f;
	FUCK::PushGamepadTweakFastDisabled();
	FUCK::PushGamepadTweakSlowDisabled();
	FUCK::SliderFloat("$DEMO_GamepadSlider"_T, &gpVal, 0.0f, 100.0f);
	FUCK::PopGamepadTweakSlowDisabled();
	FUCK::PopGamepadTweakFastDisabled();

	FUCK::Separator();

	// --- Colour Palette (v6) ---
	FUCK::Header("$DEMO_ColorPalette"_T);
	FUCK::TextWrapped("$DEMO_ColorPaletteDesc"_T);
	FUCK::TextWrapped("$DEMO_DragDropNote"_T);
	FUCK::Spacing();

	static FUCK::ColorPalette recents(12, FUCK::ColorPalette::Mode::kRecent);

	static FUCK::ColorPalette fixedGrid(16, FUCK::ColorPalette::Mode::kFixed);
	fixedGrid.columns = 8;
	fixedGrid.rows    = 2;

	static float pickedColor[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
	static float pickedColorRGB[3] = { 0.0f, 1.0f, 0.0f };

	// Add to the recents once an edit is finished, not on every change while dragging
	FUCK::ColorEdit4("$DEMO_PickerRGBA"_T, pickedColor, FUCK::ColorEditFlags::kNone);
	if (FUCK::IsItemDeactivatedAfterEdit())
		recents.Push(pickedColor);

	FUCK::ColorEdit3("$DEMO_PickerRGB"_T, pickedColorRGB, FUCK::ColorEditFlags::kNone);
	if (FUCK::IsItemDeactivatedAfterEdit())
		recents.Push(pickedColorRGB);

	FUCK::Spacing();
	FUCK::Separator();
	FUCK::Spacing();

	recents.Draw("$DEMO_RecentColours"_T, pickedColor, FUCK::Scale(30.0f));
	FUCK::Spacing();
	fixedGrid.Draw("$DEMO_FixedPalette"_T, pickedColor, FUCK::Scale(30.0f));

	FUCK::Separator();

	// --- Inline Colour Picker (v6) ---
	FUCK::Header("$DEMO_ColorPickers"_T);
	FUCK::TextWrapped("$DEMO_ColorPickersDesc"_T);
	FUCK::Spacing();

	// The picker sizes itself from the item width, so this keeps it to around half its default size
	static float inlineColor[3] = { 0.3f, 0.6f, 0.9f };
	FUCK::SetNextItemWidth(FUCK::GetContentRegionAvail().x * 0.33f);
	FUCK::ColorPicker3("##InlinePicker", inlineColor, FUCK::ColorEditFlags::kPickerHueWheel);

	FUCK::EndTabItem();
}
