#include "DEMO.h"

void DemoState::DrawGameControlTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabGameControl"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_OverlayWindow"_T);
	if (FUCK::Button("$DEMO_ToggleOverlay"_T)) {
		_overlay._isOpen = !_overlay._isOpen;
	}
	FUCK::TextWrapped("$DEMO_OverlayDesc"_T);

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_HUDWidget"_T);
	auto& hudWidget = DemoState::GetSingleton()->_hudWidget;

	if (FUCK::Button(hudWidget._isOpen ? "$DEMO_CloseHUDWidget"_T : "$DEMO_OpenHUDWidget"_T)) {
		hudWidget._wantOpen = !hudWidget._isOpen;
		hudWidget.SetOpen(!hudWidget._isOpen);
	}

	if (hudWidget._isOpen) {
		FUCK::Spacing();
		if (FUCK::Checkbox("$DEMO_KeepHudOpen"_T, &_cfg.hudKeepOpen, false, false)) {
			DemoState::GetSingleton()->SaveSettings();
		}
		FUCK::SetTooltip("$DEMO_KeepHudOpenTip"_T);

		FUCK::SliderFloat("$DEMO_HudScale"_T, &_cfg.hudScale, 0.1f, 3.0f, "%.2f");
		if (FUCK::IsItemDeactivatedAfterEdit()) {
			DemoState::GetSingleton()->SaveSettings();
		}

		FUCK::Spacing();
	}

	FUCK::TextWrapped("$DEMO_HUDWidgetDesc"_T);

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Utils"_T);

	std::string demoDir = GetSettings().GetConfigDirectory();
	FUCK::TextDisabled("$DEMO_PluginConfigDir"_T);
	FUCK::TextWrapped("%s", demoDir.c_str());

	FUCK::Spacing();

	FUCK::LeftLabel("$DEMO_RawPath"_T);
	FUCK::SetNextItemWidth(-1);
	FUCK::InputText("##RawPathInput", _pathBuffer, sizeof(_pathBuffer));

	if (FUCK::Button("$DEMO_Sanitize"_T)) {
		FUCK::SanitizePath(_sanitizedBuffer, _pathBuffer, sizeof(_sanitizedBuffer));
	}

	if (_sanitizedBuffer[0] != '\0') {
		FUCK::TextColored(ImVec4(0, 1, 0, 1), "$DEMO_Sanitized"_T, _sanitizedBuffer);
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Translations"_T);
	FUCK::Text("$DEMO_TabGeneral"_T);

	FUCK::EndTabItem();
}
