#include "DEMO.h"

void DemoState::DrawInputIOTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabInputIO"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_InputCapture"_T);
	FUCK::Checkbox("$DEMO_CaptureInput"_T, &_inputCaptured, false, false);
	if (_inputCaptured) {
		FUCK::TextColored(ImVec4(1, 1, 0, 1), "$DEMO_PressAnyKey"_T);
		if (_lastPressedKey > 0) {
			FUCK::Text("$DEMO_LastCapturedKey"_T, _lastPressedKey, FUCK::GetKeyName(_lastPressedKey));
		}
	}
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Queries"_T);
	FUCK::Text("$DEMO_CurrentDevice"_T,
		FUCK::GetInputDevice() == FUCK::InputDevice::kMouseKeyboard ?
			"$DEMO_Device_Kbd"_T :
			"$DEMO_Device_Game"_T);

	ImVec2 m = FUCK::GetMousePos();
	FUCK::Text("$DEMO_MousePos"_T, m.x, m.y);

	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Modifiers"_T);
	FUCK::Text("$DEMO_ModShift"_T, FUCK::IsModifierPressed(Modifier::kShift) ? "YES" : "NO");
	FUCK::Text("$DEMO_ModCtrl"_T, FUCK::IsModifierPressed(Modifier::kCtrl) ? "YES" : "NO");
	FUCK::Text("$DEMO_ModAlt"_T, FUCK::IsModifierPressed(Modifier::kAlt) ? "YES" : "NO");

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_KeyInput"_T);
	FUCK::SliderInt("$DEMO_TestKeyID"_T, &_keyTestIndex, 0, 200);
	FUCK::Text("$DEMO_KeyName"_T, FUCK::GetKeyName(_keyTestIndex));
	FUCK::Text("$DEMO_IsDown"_T, FUCK::IsInputDown(_keyTestIndex) ? "YES" : "NO");

	ImVec2 iconSize(48, 48);
	ImTextureID keyIcon = FUCK::GetIconForKey((std::uint32_t)_keyTestIndex, &iconSize);
	if (keyIcon) {
		FUCK::DrawImage(keyIcon, iconSize);
	} else {
		FUCK::TextDisabled("$DEMO_NoIcon"_T);
	}

	FUCK::Separator();

	FUCK::Header("$DEMO_Section_WidgetInteraction"_T);
	if (FUCK::Button("$DEMO_HoverMe"_T)) {}
	FUCK::Text("$DEMO_ItemHovered"_T, FUCK::IsItemHovered() ? "YES" : "NO");
	FUCK::Text("$DEMO_ItemActive"_T, FUCK::IsItemActive() ? "YES" : "NO");

	FUCK::EndTabItem();
}
