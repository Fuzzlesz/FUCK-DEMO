#include "DEMO.h"

void DemoState::DrawBasicWidgetsTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabBasicWidgets"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_Buttons"_T);
	FUCK::Button("$DEMO_HostButton"_T);
	FUCK::SetTooltip("$DEMO_ButtonTip"_T);

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_IconButtons"_T);

	ImVec2 iconSize;
	ImTextureID iconTex = FUCK::GetIconForKey(1, &iconSize);  // Esc key icon
	if (iconTex) {
		if (FUCK::ButtonIconWithLabel("$DEMO_IconBtn_NearIconLeft"_T, iconTex, iconSize, false, false))
			_iconBtnClicks++;
		if (FUCK::ButtonIconWithLabel("$DEMO_IconBtn_NearIconRight"_T, iconTex, iconSize, false, true))
			_iconBtnClicks++;
		FUCK::Spacing();
		if (FUCK::ButtonIconWithLabel("$DEMO_IconBtn_FarIconLeft"_T, iconTex, iconSize, true, false))
			_iconBtnClicks++;
		if (FUCK::ButtonIconWithLabel("$DEMO_IconBtn_FarIconRight"_T, iconTex, iconSize, true, true))
			_iconBtnClicks++;

		if (_iconBtnClicks > 0) {
			FUCK::Spacing();
			FUCK::Text("$DEMO_Clicks"_T, _iconBtnClicks);
		}
	} else {
		FUCK::TextDisabled("$DEMO_NoIconTexture"_T);
	}

	FUCK::Spacing();

	// Checkboxes
	FUCK::Header("$DEMO_Section_Checkboxes"_T);
	FUCK::TextDisabled("$DEMO_Header_Mutex"_T);

	bool changed = false;

	FUCK::PushID("MutexGroup");
	{
		if (FUCK::Checkbox("$DEMO_OptionA"_T, &_cfg.chkMutexA, false, true)) {
			changed = true;
			if (_cfg.chkMutexA)
				_cfg.chkMutexB = false;
		}
		if (FUCK::Checkbox("$DEMO_OptionB"_T, &_cfg.chkMutexB, false, false)) {
			changed = true;
			if (_cfg.chkMutexB)
				_cfg.chkMutexA = false;
		}
	}
	FUCK::PopID();

	FUCK::Spacing();
	FUCK::TextDisabled("$DEMO_Header_Independent"_T);

	FUCK::PushID("FarGroup");
	{
		changed |= FUCK::Checkbox("$DEMO_FarLabelA"_T, &_cfg.chkFarA, true, true);
		changed |= FUCK::Checkbox("$DEMO_FarLabelB"_T, &_cfg.chkFarB, true, false);
	}
	FUCK::PopID();

	FUCK::Spacing();

	// Toggle Buttons
	FUCK::Header("$DEMO_Section_ToggleSwitches"_T);
	FUCK::TextDisabled("$DEMO_Header_Far"_T);
	changed |= FUCK::ToggleButton("$DEMO_ToggleLabel"_T, &_cfg.toggleState);
	changed |= FUCK::ToggleButton("$DEMO_ToggleLabel_Right"_T, &_cfg.toggleState, true, false);

	FUCK::Spacing();
	FUCK::TextDisabled("$DEMO_Header_Near"_T);

	FUCK::PushID("NearToggles");
	{
		changed |= FUCK::ToggleButton("$DEMO_ToggleLabel"_T, &_cfg.toggleState, false, true);
		changed |= FUCK::ToggleButton("$DEMO_ToggleLabel_Right"_T, &_cfg.toggleState, false, false);
	}
	FUCK::PopID();

	if (changed)
		DemoState::GetSingleton()->SaveSettings();

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_Sliders"_T);
	FUCK::TextWrapped("$DEMO_SliderDesc"_T);
	FUCK::Spacing();
	FUCK::SliderFloat("$DEMO_HostFloat"_T, &_cfg.sliderVal, 0.0f, 100.0f, "%.1f");
	if (FUCK::IsItemDeactivatedAfterEdit())
		DemoState::GetSingleton()->SaveSettings();

	FUCK::DragInt("$DEMO_DragInt"_T, &_cfg.dragInt, 1.0f, 0, 100, "%d");
	if (FUCK::IsItemDeactivatedAfterEdit())
		DemoState::GetSingleton()->SaveSettings();

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_TextInput"_T);
	FUCK::SetNextItemWidth(-1);
	FUCK::InputText("$DEMO_InputField"_T, _cfg.inputBuffer, sizeof(_cfg.inputBuffer));
	if (FUCK::IsItemDeactivatedAfterEdit())
		DemoState::GetSingleton()->SaveSettings();

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_TextDisplay"_T);
	FUCK::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "$DEMO_ColoredText"_T);
	FUCK::TextColoredWrapped(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "$DEMO_TextWrappedDesc"_T);
	FUCK::TextDisabled("$DEMO_DisabledText"_T);
	FUCK::CenteredText("$DEMO_CentredText"_T, false);

	FUCK::EndTabItem();
}
