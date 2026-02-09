#include "DEMO.h"

	// Constructor
	DemoTool::DemoTool()
{
	_overlay._secondWindow = &_secondOverlay;
	_secondOverlay._parentWindow = &_overlay;
	FUCK::RegisterWindow(&_overlay);
	FUCK::RegisterWindow(&_secondOverlay);
}

// ==========================================
// Demo Tool Core Implementation
// ==========================================

void DemoTool::OnOpen()
{
	logger::info("Demo Tool Opened");

	if (_inputBuffer[0] == '\0') {
		const char* ph = "$DEMO_InputPlaceholder"_T;
		if (ph)
			strncpy_s(_inputBuffer, ph, sizeof(_inputBuffer));
	}
	LoadSettings();

	if (_stepperOptions.empty()) {
		_stepperOptions = {
			"$DEMO_Stepper_Novice"_T,
			"$DEMO_Stepper_Apprentice"_T,
			"$DEMO_Stepper_Adept"_T,
			"$DEMO_Stepper_Expert"_T,
			"$DEMO_Stepper_Master"_T
		};
	}
}

void DemoTool::OnClose()
{
	logger::info("Demo Tool Closed");
	SaveSettings();

	if (_loadedImage) {
		FUCK::ReleaseImage(_loadedImage);
		_loadedImage = nullptr;
	}
}

bool DemoTool::OnAsyncInput(const void* inputEvent)
{
	if (_inputCaptured) {
		std::uint32_t key = 0;
		std::int32_t mod1 = -1, mod2 = -1;

		auto result = FUCK::GetInputBind(inputEvent, &key, &mod1, &mod2);

		if (result == BindResult::kBound) {
			_lastPressedKey = key;

			return true;
		}

		return true;
	}

	if (FUCK::UpdateManagedHotkey(inputEvent, _overlay._toggleHotkey)) {
		return true;
	}

	if (FUCK::ProcessManagedHotkey(inputEvent, _overlay._toggleHotkey)) {
		_overlay._isOpen = !_overlay._isOpen;
		return true;
	}

	return false;
}
void DemoTool::Draw()
{
	if (!FUCK::GetInterface()) {
		FUCK::TextColored(ImVec4(1, 0, 0, 1), "$DEMO_ErrNoApi"_T);
		return;
	}

	if (FUCK::BeginTabBar("DemoMainTabs")) {
		DrawBasicWidgetsTab();
		DrawAdvancedWidgetsTab();
		DrawLayoutStyleTab();
		DrawRenderingTab();
		DrawInputIOTab();
		DrawTablesTab();
		DrawGameControlTab();
		DrawIconsTab();
		FUCK::EndTabBar();
	}
}

// ==========================================
// Settings Implementation
// ==========================================

void DemoTool::LoadSettings()
{
	Settings::GetSingleton()->Load([this](CSimpleIniA& ini) {
		_chkNear = ini.GetBoolValue("Widgets", "ChkNear", _chkNear);
		_chkMutexA = ini.GetBoolValue("Widgets", "MutexA", _chkMutexA);
		_chkMutexB = ini.GetBoolValue("Widgets", "MutexB", _chkMutexB);
		_chkFarA = ini.GetBoolValue("Widgets", "ChkFarA", _chkFarA);
		_chkFarB = ini.GetBoolValue("Widgets", "ChkFarB", _chkFarB);
		_toggleState = ini.GetBoolValue("Widgets", "ToggleState", _toggleState);
		_sliderVal = (float)ini.GetDoubleValue("Widgets", "SliderVal", _sliderVal);
		_intVal = ini.GetLongValue("Widgets", "IntVal", _intVal);
		_dragFloat = (float)ini.GetDoubleValue("Widgets", "DragFloat", _dragFloat);
		_dragInt = ini.GetLongValue("Widgets", "DragInt", _dragInt);
		const char* inputBuf = ini.GetValue("Widgets", "InputBuffer", nullptr);
		if (inputBuf) {
			strncpy_s(_inputBuffer, inputBuf, sizeof(_inputBuffer));
		}

		// Overlay
		_overlay._windowPos.x = (float)ini.GetDoubleValue("Overlay", "X", _overlay._windowPos.x);
		_overlay._windowPos.y = (float)ini.GetDoubleValue("Overlay", "Y", _overlay._windowPos.y);
		_overlay._windowSize.x = (float)ini.GetDoubleValue("Overlay", "Width", _overlay._windowSize.x);
		_overlay._windowSize.y = (float)ini.GetDoubleValue("Overlay", "Height", _overlay._windowSize.y);

		// Keys
		_overlay._toggleHotkey.kKey = (std::uint32_t)ini.GetLongValue("Overlay", "Hotkey", _overlay._toggleHotkey.kKey);
		_overlay._toggleHotkey.kMod1 = (std::int32_t)ini.GetLongValue("Overlay", "Modifier1", _overlay._toggleHotkey.kMod1);
		_overlay._toggleHotkey.kMod2 = (std::int32_t)ini.GetLongValue("Overlay", "Modifier2", _overlay._toggleHotkey.kMod2);
		_overlay._toggleHotkey.gKey = (std::uint32_t)ini.GetLongValue("Overlay", "GPHotkey", _overlay._toggleHotkey.gKey);
		_overlay._toggleHotkey.gMod1 = (std::int32_t)ini.GetLongValue("Overlay", "GPModifier1", _overlay._toggleHotkey.gMod1);
		_overlay._toggleHotkey.gMod2 = (std::int32_t)ini.GetLongValue("Overlay", "GPModifier2", _overlay._toggleHotkey.gMod2);

		// Flags
		_overlay._reqBlur = ini.GetBoolValue("OverlayFlags", "Blur", _overlay._reqBlur);
		_overlay._reqHideHUD = ini.GetBoolValue("OverlayFlags", "HideHUD", _overlay._reqHideHUD);
		_overlay._reqBlockVanity = ini.GetBoolValue("OverlayFlags", "BlockVanity", _overlay._reqBlockVanity);
		_overlay._reqCloseOnEsc = ini.GetBoolValue("OverlayFlags", "CloseOnEsc", _overlay._reqCloseOnEsc);
		_overlay._reqCloseOnMenu = ini.GetBoolValue("OverlayFlags", "CloseOnMenu", _overlay._reqCloseOnMenu);

		// Second Window
		_secondOverlay._pos.x = (float)ini.GetDoubleValue("SecondWindow", "X", _secondOverlay._pos.x);
		_secondOverlay._pos.y = (float)ini.GetDoubleValue("SecondWindow", "Y", _secondOverlay._pos.y);
	});
}

void DemoTool::SaveSettings()
{
	Settings::GetSingleton()->Save([this](CSimpleIniA& ini) {
		ini.SetBoolValue("Widgets", "ChkNear", _chkNear);
		ini.SetBoolValue("Widgets", "MutexA", _chkMutexA);
		ini.SetBoolValue("Widgets", "MutexB", _chkMutexB);
		ini.SetBoolValue("Widgets", "ChkFarA", _chkFarA);
		ini.SetBoolValue("Widgets", "ChkFarB", _chkFarB);
		ini.SetBoolValue("Widgets", "ToggleState", _toggleState);
		ini.SetDoubleValue("Widgets", "SliderVal", _sliderVal);
		ini.SetLongValue("Widgets", "IntVal", _intVal);
		ini.SetDoubleValue("Widgets", "DragFloat", _dragFloat);
		ini.SetLongValue("Widgets", "DragInt", _dragInt);
		ini.SetValue("Widgets", "InputBuffer", _inputBuffer);

		ini.SetDoubleValue("Overlay", "X", _overlay._windowPos.x);
		ini.SetDoubleValue("Overlay", "Y", _overlay._windowPos.y);
		ini.SetDoubleValue("Overlay", "Width", _overlay._windowSize.x);
		ini.SetDoubleValue("Overlay", "Height", _overlay._windowSize.y);

		ini.SetLongValue("Overlay", "Hotkey", _overlay._toggleHotkey.kKey);
		ini.SetLongValue("Overlay", "Modifier1", _overlay._toggleHotkey.kMod1);
		ini.SetLongValue("Overlay", "Modifier2", _overlay._toggleHotkey.kMod2);
		ini.SetLongValue("Overlay", "GPHotkey", _overlay._toggleHotkey.gKey);
		ini.SetLongValue("Overlay", "GPModifier1", _overlay._toggleHotkey.gMod1);
		ini.SetLongValue("Overlay", "GPModifier2", _overlay._toggleHotkey.gMod2);

		ini.SetBoolValue("OverlayFlags", "Blur", _overlay._reqBlur);
		ini.SetBoolValue("OverlayFlags", "HideHUD", _overlay._reqHideHUD);
		ini.SetBoolValue("OverlayFlags", "BlockVanity", _overlay._reqBlockVanity);
		ini.SetBoolValue("OverlayFlags", "CloseOnEsc", _overlay._reqCloseOnEsc);
		ini.SetBoolValue("OverlayFlags", "CloseOnMenu", _overlay._reqCloseOnMenu);

		ini.SetDoubleValue("SecondWindow", "X", _secondOverlay._pos.x);
		ini.SetDoubleValue("SecondWindow", "Y", _secondOverlay._pos.y);
	});
}

// ==========================================
// Simple Secondary Overlay
// ==========================================

void SimpleOverlay::Draw()
{
	if (FUCK::Checkbox("$DEMO_Sec_PassInput"_T, &_passInput, false, false)) {
		if (_passInput) {
			FUCK::SetMenuOpen(false);
			if (_parentWindow) {
				_parentWindow->SetOpen(false);
			}
		}
	}
	FUCK::SetTooltip("$DEMO_Sec_PassInputTip"_T);

	FUCK::Separator();
	FUCK::Spacing();

	if (_passInput) {
		FUCK::TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "$DEMO_Sec_CloseHint"_T);
	}

	FUCK::TextWrapped("$DEMO_SecondOverlayContent"_T);
	FUCK::Spacing();
	if (FUCK::Button("$DEMO_SecondOverlayClose"_T)) {
		_isOpen = false;
	}
}

// ==========================================
// Demo Overlay Implementation
// ==========================================

WindowFlags DemoOverlay::GetFlags() const
{
	WindowFlags flags = WindowFlags::kNone;
	if (_reqBlur)
		flags = flags | WindowFlags::kBlurBackground;
	if (_reqHideHUD)
		flags = flags | WindowFlags::kHideHUD;
	if (_reqPauseHard)
		flags = flags | WindowFlags::kPauseHard;
	if (_reqPauseSoft)
		flags = flags | WindowFlags::kPauseSoft;
	if (_reqCloseOnEsc)
		flags = flags | WindowFlags::kCloseOnEsc;
	if (_reqCloseOnMenu)
		flags = flags | WindowFlags::kCloseOnGameMenu;
	if (_reqPassInput)
		flags = flags | WindowFlags::kPassInputToGame;
	if (_reqBlockVanity)
		flags = flags | WindowFlags::kBlockVanity;
	if (_reqNoBackground)
		flags = flags | WindowFlags::kNoBackground;
	if (_reqNoDecoration)
		flags = flags | WindowFlags::kNoDecoration;
	if (_reqExtendBorder)
		flags = flags | WindowFlags::kExtendBorder;

	return flags;
}

void DemoOverlay::Draw()
{
	FUCK::SetNextWindowPos(_windowPos, 8 /* ImGuiCond_Appearing */);
	FUCK::SetNextWindowSize(_windowSize, 8 /* ImGuiCond_Appearing */);

	ImVec2 currentPos = FUCK::GetWindowPos();
	ImVec2 currentSize = FUCK::GetWindowSize();

	if (currentPos.x != _windowPos.x || currentPos.y != _windowPos.y ||
		currentSize.x != _windowSize.x || currentSize.y != _windowSize.y) {
		_windowPos = currentPos;
		_windowSize = currentSize;
		DemoTool::GetSingleton()->SaveSettings();
	}

	FUCK::Header("$DEMO_Section_WindowMetrics"_T);
	FUCK::Text("$DEMO_MetricSize"_T, _windowSize.x, _windowSize.y);
	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_WindowFlags"_T);
	FUCK::Text("$DEMO_WindowFlags_Desc"_T);
	FUCK::Checkbox("$DEMO_Flag_Blur"_T, &_reqBlur, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_Blur"_T);
	FUCK::Checkbox("$DEMO_Flag_HideHUD"_T, &_reqHideHUD, false, false);
	FUCK::SetTooltip("DEMO_FlagTip_HideHUD"_T);
	FUCK::Checkbox("$DEMO_Flag_PauseHard"_T, &_reqPauseHard, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_PauseHard"_T);
	FUCK::Checkbox("$DEMO_Flag_PauseSoft"_T, &_reqPauseSoft, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_PauseSoft"_T);
	FUCK::Checkbox("$DEMO_Flag_CloseOnEsc"_T, &_reqCloseOnEsc, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnEsc"_T);
	FUCK::Checkbox("$DEMO_Flag_CloseOnMenu"_T, &_reqCloseOnMenu, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnMenu"_T);

	if (FUCK::Checkbox("$DEMO_Flag_PassInput"_T, &_reqPassInput, false, false)) {
		if (_reqPassInput) {
			FUCK::SetMenuOpen(false);
			std::thread([this]() {
				std::this_thread::sleep_for(std::chrono::seconds(3));
				this->_reqPassInput = false;
			}).detach();
		}
	}
	FUCK::SetTooltip("$DEMO_PassInputTip"_T);
	FUCK::Checkbox("$DEMO_Flag_BlockVanity"_T, &_reqBlockVanity, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_BlockVanity"_T);
	FUCK::Checkbox("$DEMO_Flag_NoBackground"_T, &_reqNoBackground, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_NoBackground"_T);
	FUCK::Checkbox("$DEMO_Flag_NoDecoration"_T, &_reqNoDecoration, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_NoDecoration"_T);
	FUCK::Checkbox("$DEMO_Flag_ExtendBorder"_T, &_reqExtendBorder, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_ExtendBorder"_T);

	FUCK::SeparatorThick();
	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_OverlayControls"_T);
	FUCK::DrawManagedHotkey("$DEMO_OverlayToggleKey"_T, _toggleHotkey);
	FUCK::SetTooltip("$DEMO_OverlayKeyTip"_T);
	FUCK::Spacing();

	if (FUCK::Button("$DEMO_ResetHotkey"_T)) {
		_toggleHotkey.kKey = 35;   // H
		_toggleHotkey.kMod1 = 42;  // Shift
		_toggleHotkey.kMod2 = -1;
		_toggleHotkey.gKey = 0;
		_toggleHotkey.gMod1 = -1;
		_toggleHotkey.gMod2 = -1;
	}

	FUCK::SameLine();
	if (FUCK::Button("$DEMO_UnmapHotkey"_T)) {
		_toggleHotkey.Clear();
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_MultiWindowHeader"_T);
	if (_secondWindow) {
		if (FUCK::Button("$DEMO_OpenSecondWin"_T)) {
			_secondWindow->SetOpen(true);
		}
	}

	FUCK::Spacing();
	FUCK::Separator();

	if (FUCK::Button("$DEMO_Close"_T)) {
		DemoTool::GetSingleton()->SaveSettings();
		_isOpen = false;
	}
}
