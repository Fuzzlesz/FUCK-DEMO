#include "DEMO.h"

// ==================================================
// Construction & Registration
// ==================================================

DemoState::DemoState()
{
	// Link Overlays
	_overlay._secondWindow       = &_secondOverlay;
	_secondOverlay._parentWindow = &_overlay;

	// Register Overlays
	FUCK::RegisterWindow(&_overlay);
	FUCK::RegisterWindow(&_secondOverlay);
	FUCK::RegisterWindow(&_hudWidget);

	// Register Split Tools
	FUCK::RegisterTool(&_toolGeneral);
	FUCK::RegisterTool(&_toolVisuals);
	FUCK::RegisterTool(&_toolSystem);

	LoadSettings();
}

// ==================================================
// Tool Implementations
// ==================================================

void DemoState::ToolGeneral::Draw()
{
	if (FUCK::BeginTabBar("GeneralTabs")) {
		DemoState::GetSingleton()->DrawBasicWidgetsTab();
		DemoState::GetSingleton()->DrawAdvancedWidgetsTab();
		FUCK::EndTabBar();
	}
}

void DemoState::ToolVisuals::Draw()
{
	if (FUCK::BeginTabBar("VisualTabs")) {
		DemoState::GetSingleton()->DrawLayoutStyleTab();
		DemoState::GetSingleton()->DrawRenderingTab();
		DemoState::GetSingleton()->DrawIconsTab();
		FUCK::EndTabBar();
	}
}

void DemoState::ToolSystem::Draw()
{
	if (FUCK::BeginTabBar("SystemTabs")) {
		DemoState::GetSingleton()->DrawInputIOTab();
		DemoState::GetSingleton()->DrawTablesTab();
		DemoState::GetSingleton()->DrawGameControlTab();
		FUCK::EndTabBar();
	}
}

// ==================================================
// Demo State Core
// ==================================================

void DemoState::OnOpen()
{
	logger::info("Demo Tool Opened");

	if (_cfg.inputBuffer[0] == '\0') {
		const char* ph = "$DEMO_InputPlaceholder"_T;
		if (ph)
			FUCK::StringCopy(_cfg.inputBuffer, ph);
	}

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

void DemoState::OnClose()
{
	logger::info("Demo Tool Closed");
}

bool DemoState::OnAsyncInput(const void* inputEvent)
{
	if (_inputCaptured) {
		std::uint32_t key  = 0;
		std::int32_t  mod1 = -1, mod2 = -1;

		if (FUCK::GetInputBind(inputEvent, &key, &mod1, &mod2) == FUCK::BindResult::kBound) {
			_lastPressedKey = key;
		}
		return true;
	}

	if (FUCK::UpdateManagedHotkey(inputEvent, _toggleHotkey)) {
		SaveKeybinds();
		return true;
	}

	if (FUCK::ProcessManagedHotkey(inputEvent, _toggleHotkey)) {
		_overlay._isOpen = !_overlay._isOpen;
		return true;
	}

	return false;
}

// ==================================================
// Settings Implementation
// ==================================================

void DemoState::LoadSettings()
{
	GetSettings().Load([this](CSimpleIniA& ini) {
		_cfg.chkNear     = FUCK::INI::LoadBool(ini, "Widgets", "ChkNear", _def.chkNear);
		_cfg.chkMutexA   = FUCK::INI::LoadBool(ini, "Widgets", "MutexA", _def.chkMutexA);
		_cfg.chkMutexB   = FUCK::INI::LoadBool(ini, "Widgets", "MutexB", _def.chkMutexB);
		_cfg.chkFarA     = FUCK::INI::LoadBool(ini, "Widgets", "ChkFarA", _def.chkFarA);
		_cfg.chkFarB     = FUCK::INI::LoadBool(ini, "Widgets", "ChkFarB", _def.chkFarB);
		_cfg.toggleState = FUCK::INI::LoadBool(ini, "Widgets", "ToggleState", _def.toggleState);

		_cfg.sliderVal = FUCK::INI::LoadFloat(ini, "Widgets", "SliderVal", _def.sliderVal);
		_cfg.intVal    = FUCK::INI::LoadInt  (ini, "Widgets", "IntVal", _def.intVal);
		_cfg.dragFloat = FUCK::INI::LoadFloat(ini, "Widgets", "DragFloat", _def.dragFloat);
		_cfg.dragInt   = FUCK::INI::LoadInt  (ini, "Widgets", "DragInt", _def.dragInt);

		const char* inputBuf = ini.GetValue("Widgets", "InputBuffer", nullptr);
		if (inputBuf)
			FUCK::StringCopy(_cfg.inputBuffer, inputBuf); 

		_cfg.reqBlur         = FUCK::INI::LoadBool(ini, "OverlayFlags", "Blur", _def.reqBlur);
		_cfg.reqHideHUD      = FUCK::INI::LoadBool(ini, "OverlayFlags", "HideHUD", _def.reqHideHUD);
		_cfg.reqPauseHard    = FUCK::INI::LoadBool(ini, "OverlayFlags", "PauseHard", _def.reqPauseHard);
		_cfg.reqPauseSoft    = FUCK::INI::LoadBool(ini, "OverlayFlags", "PauseSoft", _def.reqPauseSoft);
		_cfg.reqCloseOnEsc   = FUCK::INI::LoadBool(ini, "OverlayFlags", "CloseOnEsc", _def.reqCloseOnEsc);
		_cfg.reqCloseOnMenu  = FUCK::INI::LoadBool(ini, "OverlayFlags", "CloseOnMenu", _def.reqCloseOnMenu);
		_cfg.reqPassInput    = FUCK::INI::LoadBool(ini, "OverlayFlags", "PassInput", _def.reqPassInput);
		_cfg.reqBlockVanity  = FUCK::INI::LoadBool(ini, "OverlayFlags", "BlockVanity", _def.reqBlockVanity);
		_cfg.reqNoBackground = FUCK::INI::LoadBool(ini, "OverlayFlags", "NoBackground", _def.reqNoBackground);
		_cfg.reqNoDecoration = FUCK::INI::LoadBool(ini, "OverlayFlags", "NoDecoration", _def.reqNoDecoration);
		_cfg.reqExtendBorder = FUCK::INI::LoadBool(ini, "OverlayFlags", "ExtendBorder", _def.reqExtendBorder);

		_cfg.secondPassInput  = FUCK::INI::LoadBool(ini, "SecondWindow", "PassInput", _def.secondPassInput);

		_cfg.hudKeepOpen = FUCK::INI::LoadBool (ini, "HudWidget", "KeepOpen", _def.hudKeepOpen);
		_cfg.hudScale    = FUCK::INI::LoadFloat(ini, "HudWidget", "Scale", _def.hudScale);

		_cfg.showOverlay      = FUCK::INI::LoadBool (ini, "Rendering", "ShowOverlay", _def.showOverlay);
		_cfg.overlayType      = FUCK::INI::LoadInt  (ini, "Rendering", "OverlayType", _def.overlayType);
		_cfg.overlayThickness = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayThickness", _def.overlayThickness);

		_cfg.overlayColor[0] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorR", _def.overlayColor[0]);
		_cfg.overlayColor[1] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorG", _def.overlayColor[1]);
		_cfg.overlayColor[2] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorB", _def.overlayColor[2]);
		_cfg.overlayColor[3] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorA", _def.overlayColor[3]);

		_cfg.gridRows     = FUCK::INI::LoadInt  (ini, "Rendering", "GridRows", _def.gridRows);
		_cfg.gridCols     = FUCK::INI::LoadInt  (ini, "Rendering", "GridCols", _def.gridCols);
		_cfg.spiralAnchor = FUCK::INI::LoadInt  (ini, "Rendering", "SpiralAnchor", _def.spiralAnchor);
		_cfg.spiralRot    = FUCK::INI::LoadFloat(ini, "Rendering", "SpiralRot", _def.spiralRot);
		_cfg.spiralScale  = FUCK::INI::LoadFloat(ini, "Rendering", "SpiralScale", _def.spiralScale);
		_cfg.spiralTurns  = FUCK::INI::LoadFloat(ini, "Rendering", "SpiralTurns", _def.spiralTurns);
		_cfg.showSquares  = FUCK::INI::LoadBool (ini, "Rendering", "ShowSquares", _def.showSquares);
		_cfg.triMirror    = FUCK::INI::LoadBool (ini, "Rendering", "TriMirror", _def.triMirror);
	});

	GetSettings().LoadKeybinds([this](CSimpleIniA& ini) {
		_toggleHotkey.kKey  = FUCK::INI::LoadInt(ini, "Overlay", "Hotkey", _defHotkey.kKey);
		_toggleHotkey.kMod1 = FUCK::INI::LoadInt(ini, "Overlay", "Modifier1", _defHotkey.kMod1);
		_toggleHotkey.kMod2 = FUCK::INI::LoadInt(ini, "Overlay", "Modifier2", _defHotkey.kMod2);
		_toggleHotkey.gKey  = FUCK::INI::LoadInt(ini, "Overlay", "GPHotkey", _defHotkey.gKey);
		_toggleHotkey.gMod1 = FUCK::INI::LoadInt(ini, "Overlay", "GPModifier1", _defHotkey.gMod1);
		_toggleHotkey.gMod2 = FUCK::INI::LoadInt(ini, "Overlay", "GPModifier2", _defHotkey.gMod2);
	});
}

void DemoState::SaveSettings()
{
	GetSettings().Save([this](CSimpleIniA& ini) {
		// General Widgets
		FUCK::INI::SaveBool  (ini, "Widgets", "ChkNear", _cfg.chkNear, _def.chkNear);
		FUCK::INI::SaveBool  (ini, "Widgets", "MutexA", _cfg.chkMutexA, _def.chkMutexA);
		FUCK::INI::SaveBool  (ini, "Widgets", "MutexB", _cfg.chkMutexB, _def.chkMutexB);
		FUCK::INI::SaveBool  (ini, "Widgets", "ChkFarA", _cfg.chkFarA, _def.chkFarA);
		FUCK::INI::SaveBool  (ini, "Widgets", "ChkFarB", _cfg.chkFarB, _def.chkFarB);
		FUCK::INI::SaveBool  (ini, "Widgets", "ToggleState", _cfg.toggleState, _def.toggleState);
		FUCK::INI::SaveDouble(ini, "Widgets", "SliderVal", _cfg.sliderVal, _def.sliderVal);
		FUCK::INI::SaveInt   (ini, "Widgets", "IntVal", _cfg.intVal, _def.intVal);
		FUCK::INI::SaveDouble(ini, "Widgets", "DragFloat", _cfg.dragFloat, _def.dragFloat);
		FUCK::INI::SaveInt   (ini, "Widgets", "DragInt", _cfg.dragInt, _def.dragInt);
		FUCK::INI::SaveString(ini, "Widgets", "InputBuffer", _cfg.inputBuffer, _def.inputBuffer);

		// Overlay Flags
		FUCK::INI::SaveBool(ini, "OverlayFlags", "Blur", _cfg.reqBlur, _def.reqBlur);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "HideHUD", _cfg.reqHideHUD, _def.reqHideHUD);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "PauseHard", _cfg.reqPauseHard, _def.reqPauseHard);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "PauseSoft", _cfg.reqPauseSoft, _def.reqPauseSoft);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "CloseOnEsc", _cfg.reqCloseOnEsc, _def.reqCloseOnEsc);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "CloseOnMenu", _cfg.reqCloseOnMenu, _def.reqCloseOnMenu);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "PassInput", _cfg.reqPassInput, _def.reqPassInput);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "BlockVanity", _cfg.reqBlockVanity, _def.reqBlockVanity);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "NoBackground", _cfg.reqNoBackground, _def.reqNoBackground);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "NoDecoration", _cfg.reqNoDecoration, _def.reqNoDecoration);
		FUCK::INI::SaveBool(ini, "OverlayFlags", "ExtendBorder", _cfg.reqExtendBorder, _def.reqExtendBorder);

		// Second Window
		FUCK::INI::SaveBool     (ini, "SecondWindow", "PassInput", _cfg.secondPassInput, _def.secondPassInput);

		// HUD Widget
		FUCK::INI::SaveBool     (ini, "HudWidget", "KeepOpen", _cfg.hudKeepOpen, _def.hudKeepOpen);
		FUCK::INI::SaveDouble   (ini, "HudWidget", "Scale", _cfg.hudScale, _def.hudScale);

		// Rendering
		FUCK::INI::SaveBool  (ini, "Rendering", "ShowOverlay", _cfg.showOverlay, _def.showOverlay);
		FUCK::INI::SaveInt   (ini, "Rendering", "OverlayType", _cfg.overlayType, _def.overlayType);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayThickness", _cfg.overlayThickness, _def.overlayThickness);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorR", _cfg.overlayColor[0], _def.overlayColor[0]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorG", _cfg.overlayColor[1], _def.overlayColor[1]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorB", _cfg.overlayColor[2], _def.overlayColor[2]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorA", _cfg.overlayColor[3], _def.overlayColor[3]);
		FUCK::INI::SaveInt   (ini, "Rendering", "GridRows", _cfg.gridRows, _def.gridRows);
		FUCK::INI::SaveInt   (ini, "Rendering", "GridCols", _cfg.gridCols, _def.gridCols);
		FUCK::INI::SaveInt   (ini, "Rendering", "SpiralAnchor", _cfg.spiralAnchor, _def.spiralAnchor);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralRot", _cfg.spiralRot, _def.spiralRot);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralScale", _cfg.spiralScale, _def.spiralScale);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralTurns", _cfg.spiralTurns, _def.spiralTurns);
		FUCK::INI::SaveBool  (ini, "Rendering", "ShowSquares", _cfg.showSquares, _def.showSquares);
		FUCK::INI::SaveBool  (ini, "Rendering", "TriMirror", _cfg.triMirror, _def.triMirror);
	});
}

void DemoState::SaveKeybinds()
{
	GetSettings().SaveKeybinds([this](CSimpleIniA& ini) {
		FUCK::INI::SaveInt(ini, "Overlay", "Hotkey", _toggleHotkey.kKey, _defHotkey.kKey);
		FUCK::INI::SaveInt(ini, "Overlay", "Modifier1", _toggleHotkey.kMod1, _defHotkey.kMod1);
		FUCK::INI::SaveInt(ini, "Overlay", "Modifier2", _toggleHotkey.kMod2, _defHotkey.kMod2);
		FUCK::INI::SaveInt(ini, "Overlay", "GPHotkey", _toggleHotkey.gKey, _defHotkey.gKey);
		FUCK::INI::SaveInt(ini, "Overlay", "GPModifier1", _toggleHotkey.gMod1, _defHotkey.gMod1);
		FUCK::INI::SaveInt(ini, "Overlay", "GPModifier2", _toggleHotkey.gMod2, _defHotkey.gMod2);
	});
}

// ==================================================
// Simple Secondary Overlay
// ==================================================

FUCK::WindowFlags SimpleOverlay::GetFlags() const
{
	FUCK::WindowFlags f = FUCK::WindowFlags::kNoDecoration;
	if (DemoState::GetSingleton()->_cfg.secondPassInput)
		f = f | FUCK::WindowFlags::kPassInputToGame;
	return f;
}

ImVec2 SimpleOverlay::GetDefaultPos() const
{
	return FUCK::Scale(800.0f, 400.0f);
}

ImVec2 SimpleOverlay::GetDefaultSize() const
{
	return FUCK::UIScale(400.0f, 300.0f);
}

void SimpleOverlay::Draw()
{
	if (FUCK::Checkbox("$DEMO_Sec_PassInput"_T, &DemoState::GetSingleton()->_cfg.secondPassInput, false, false)) {
		DemoState::GetSingleton()->SaveSettings();
		if (DemoState::GetSingleton()->_cfg.secondPassInput) {
			FUCK::SetMenuOpen(false);
			if (_parentWindow) {
				_parentWindow->SetOpen(false);
			}
		}
	}
	FUCK::SetTooltip("$DEMO_Sec_PassInputTip"_T);

	FUCK::Separator();
	FUCK::Spacing();

	if (DemoState::GetSingleton()->_cfg.secondPassInput) {
		FUCK::TextColoredWrapped(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "$DEMO_Sec_CloseHint"_T);
	}

	FUCK::TextWrapped("$DEMO_SecondOverlayContent"_T);
	FUCK::Spacing();
	if (FUCK::Button("$DEMO_SecondOverlayClose"_T)) {
		_isOpen = false;
	}
}

// ==================================================
// HUD Widget Implementation
// ==================================================

FUCK::WindowFlags HudWidget::GetFlags() const
{
	FUCK::WindowFlags flags = FUCK::WindowFlags::kNoDecoration |
	                          FUCK::WindowFlags::kNoBackground |
	                          FUCK::WindowFlags::kAutoResize;

	if (!FUCK::IsMenuOpen()) {
		flags = flags | FUCK::WindowFlags::kPassInputToGame;
	}

	if (!DemoState::GetSingleton()->_cfg.hudKeepOpen)
		flags = flags | FUCK::WindowFlags::kCloseOnGameMenu;

	return flags;
}

ImVec2 HudWidget::GetDefaultPos() const
{
	return FUCK::Scale(100.0f, 100.0f);
}

ImVec2 HudWidget::GetDefaultSize() const
{
	if (const_cast<HudWidget*>(this)->_hudImage.IsLoaded()) {
		ImVec2 imageSize = FUCK::Scale(ImVec2(_hudImage.GetWidth() * DemoState::GetSingleton()->_cfg.hudScale, _hudImage.GetHeight() * DemoState::GetSingleton()->_cfg.hudScale));
		float  padding   = FUCK::Scale(12.0f);
		return { imageSize.x + (padding * 2.0f), imageSize.y + (padding * 2.0f) };
	}
	return FUCK::Scale(100.0f, 100.0f);
}

void HudWidget::Draw()
{
	bool  isInteractable = FUCK::IsMenuOpen();
	auto& cfg            = DemoState::GetSingleton()->_cfg;

	if (!_hudImage.IsLoaded() && !_hasTriedLoad) {
		std::string imagePath = GetSettings().GetConfigDirectory() + _imageFilename;
		_hudImage             = FUCK::Image(imagePath.c_str(), false);
		_hasTriedLoad         = true;
	}

	if (_hudImage.IsLoaded()) {
		ImVec2 imageSize = FUCK::Scale(ImVec2(_hudImage.GetWidth() * cfg.hudScale, _hudImage.GetHeight() * cfg.hudScale));
		float  padding   = FUCK::Scale(12.0f);
		FUCK::SetCursorPos({ padding, padding });

		ImVec2 pMin = FUCK::GetCursorScreenPos();
		ImVec2 pMax = { pMin.x + imageSize.x, pMin.y + imageSize.y };

		FUCK::DrawImage(_hudImage.GetID(), imageSize);

		if (isInteractable) {
			bool isHovered = FUCK::IsWindowHovered(0);

			float gap = FUCK::Scale(4.0f);
			pMin.x -= gap;
			pMin.y -= gap;
			pMax.x += gap;
			pMax.y += gap;

			FUCK::EditorBoundsState state = isHovered ? FUCK::EditorBoundsState::kHovered : FUCK::EditorBoundsState::kNormal;
			FUCK::DrawEditorBounds(pMin, pMax, state, 2.0f);
		}
	} else {
		FUCK::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "$DEMO_ImageMissing"_T);
	}
}

// ==================================================
// Demo Overlay Implementation
// ==================================================

FUCK::WindowFlags DemoOverlay::GetFlags() const
{
	auto&             cfg   = DemoState::GetSingleton()->_cfg;
	FUCK::WindowFlags flags = FUCK::WindowFlags::kNone;
	if (cfg.reqBlur)
		flags = flags | FUCK::WindowFlags::kBlurBackground;
	if (cfg.reqHideHUD)
		flags = flags | FUCK::WindowFlags::kHideHUD;
	if (cfg.reqPauseHard)
		flags = flags | FUCK::WindowFlags::kPauseHard;
	if (cfg.reqPauseSoft)
		flags = flags | FUCK::WindowFlags::kPauseSoft;
	if (cfg.reqCloseOnEsc)
		flags = flags | FUCK::WindowFlags::kCloseOnEsc;
	if (cfg.reqCloseOnMenu)
		flags = flags | FUCK::WindowFlags::kCloseOnGameMenu;
	if (cfg.reqPassInput)
		flags = flags | FUCK::WindowFlags::kPassInputToGame;
	if (cfg.reqBlockVanity)
		flags = flags | FUCK::WindowFlags::kBlockVanity;
	if (cfg.reqNoBackground)
		flags = flags | FUCK::WindowFlags::kNoBackground;
	if (cfg.reqNoDecoration)
		flags = flags | FUCK::WindowFlags::kNoDecoration;
	if (cfg.reqExtendBorder)
		flags = flags | FUCK::WindowFlags::kExtendBorder;
	return flags;
}

ImVec2 DemoOverlay::GetDefaultSize() const
{
	return FUCK::UIScale(_baseSize);
}

ImVec2 DemoOverlay::GetDefaultPos() const
{
	ImVec2 displaySize = FUCK::GetDisplaySize();
	ImVec2 scaledSize  = GetDefaultSize();

	float offset = FUCK::Scale(50.0f);

	return {
		displaySize.x - scaledSize.x - offset,
		FUCK::Scale(100.0f)
	};
}

void DemoOverlay::Draw()
{
	auto& cfg = DemoState::GetSingleton()->_cfg;

	ImVec2 curPos  = FUCK::GetWindowPos();
	ImVec2 curSize = FUCK::GetWindowSize();

	FUCK::Header("$DEMO_Section_WindowMetrics"_T);
	FUCK::Text("$DEMO_MetricSize"_T, curSize.x, curSize.y);
	FUCK::Text("$DEMO_MetricPos"_T, curPos.x, curPos.y);

	FUCK::Header("$DEMO_Section_WindowFlags"_T);
	FUCK::Text("$DEMO_WindowFlags_Desc"_T);

	bool flagsChanged = false;
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_Blur"_T, &cfg.reqBlur, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_Blur"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_HideHUD"_T, &cfg.reqHideHUD, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_HideHUD"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_PauseHard"_T, &cfg.reqPauseHard, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_PauseHard"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_PauseSoft"_T, &cfg.reqPauseSoft, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_PauseSoft"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_CloseOnEsc"_T, &cfg.reqCloseOnEsc, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnEsc"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_CloseOnMenu"_T, &cfg.reqCloseOnMenu, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnMenu"_T);

	if (FUCK::Checkbox("$DEMO_Flag_PassInput"_T, &cfg.reqPassInput, false, false)) {
		flagsChanged = true;
		if (cfg.reqPassInput) {
			FUCK::SetMenuOpen(false);
			std::thread([this]() {
				std::this_thread::sleep_for(std::chrono::seconds(3));
				DemoState::GetSingleton()->_cfg.reqPassInput = false;
			}).detach();
		}
	}
	FUCK::SetTooltip("$DEMO_PassInputTip"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_BlockVanity"_T, &cfg.reqBlockVanity, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_BlockVanity"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_NoBackground"_T, &cfg.reqNoBackground, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_NoBackground"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_NoDecoration"_T, &cfg.reqNoDecoration, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_NoDecoration"_T);
	flagsChanged |= FUCK::Checkbox("$DEMO_Flag_ExtendBorder"_T, &cfg.reqExtendBorder, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_ExtendBorder"_T);

	if (flagsChanged)
		DemoState::GetSingleton()->SaveSettings();

	FUCK::SeparatorThick();

	FUCK::Header("$DEMO_Section_OverlayControls"_T);
	auto state = DemoState::GetSingleton();
	FUCK::DrawManagedHotkey("$DEMO_OverlayToggleKey"_T, state->_toggleHotkey);
	FUCK::SetTooltip("$DEMO_OverlayKeyTip"_T);
	FUCK::Spacing();

	if (FUCK::Button("$DEMO_ResetHotkey"_T)) {
		state->_toggleHotkey.kKey  = state->_defHotkey.kKey;
		state->_toggleHotkey.kMod1 = state->_defHotkey.kMod1;
		state->_toggleHotkey.kMod2 = state->_defHotkey.kMod2;
		state->_toggleHotkey.gKey  = state->_defHotkey.gKey;
		state->_toggleHotkey.gMod1 = state->_defHotkey.gMod1;
		state->_toggleHotkey.gMod2 = state->_defHotkey.gMod2;
		state->SaveKeybinds();
	}

	FUCK::SameLine();
	if (FUCK::Button("$DEMO_UnmapHotkey"_T)) {
		state->_toggleHotkey.Clear();
		state->SaveKeybinds();
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
		_isOpen = false;
	}
}
