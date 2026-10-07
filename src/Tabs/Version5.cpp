#include "DEMO.h"

void DemoState::DrawVers5Page()
{
	// --- Panel Frame & Scaling ---
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

	// --- Gamepad Tweaks ---
	FUCK::Header("$DEMO_GamepadTweaks"_T);
	FUCK::TextWrapped("$DEMO_GamepadTweaksDesc"_T);
	FUCK::Spacing();

	static float gpVal = 50.0f;
	FUCK::PushGamepadTweakFastDisabled();
	FUCK::PushGamepadTweakSlowDisabled();
	FUCK::SliderFloat("$DEMO_GamepadSlider"_T, &gpVal, 0.0f, 100.0f);
	FUCK::PopGamepadTweakSlowDisabled();
	FUCK::PopGamepadTweakFastDisabled();
}
