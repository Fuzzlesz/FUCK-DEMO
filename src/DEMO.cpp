#include "DEMO.h"

// ==========================================
// Construction & Registration
// ==========================================

DemoState::DemoState()
{
	// Link Overlays
	_overlay._secondWindow = &_secondOverlay;
	_secondOverlay._parentWindow = &_overlay;

	// Pre-load the image so the window creation instantly knows the correct bounds
	_hudWidget._hudImage = FUCK::Image("Data/Interface/test.png", false);

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

// ==========================================
// Tool Implementations
// ==========================================

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

// ==========================================
// Demo State Core
// ==========================================

void DemoState::OnOpen()
{
	logger::info("Demo Tool Opened");

	if (_cfg.inputBuffer[0] == '\0') {
		const char* ph = "$DEMO_InputPlaceholder"_T;
		if (ph)
			strncpy_s(_cfg.inputBuffer, ph, sizeof(_cfg.inputBuffer));
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
		std::uint32_t key = 0;
		std::int32_t mod1 = -1, mod2 = -1;

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

// ==========================================
// Settings Implementation
// ==========================================

void DemoState::LoadSettings()
{
	GetSettings().Load([this](CSimpleIniA& ini) {
		_cfg.chkNear = ini.GetBoolValue("Widgets", "ChkNear", _def.chkNear);
		_cfg.chkMutexA = ini.GetBoolValue("Widgets", "MutexA", _def.chkMutexA);
		_cfg.chkMutexB = ini.GetBoolValue("Widgets", "MutexB", _def.chkMutexB);
		_cfg.chkFarA = ini.GetBoolValue("Widgets", "ChkFarA", _def.chkFarA);
		_cfg.chkFarB = ini.GetBoolValue("Widgets", "ChkFarB", _def.chkFarB);
		_cfg.toggleState = ini.GetBoolValue("Widgets", "ToggleState", _def.toggleState);
		_cfg.sliderVal = static_cast<float>(ini.GetDoubleValue("Widgets", "SliderVal", _def.sliderVal));
		_cfg.intVal = static_cast<std::int32_t>(ini.GetLongValue("Widgets", "IntVal", _def.intVal));
		_cfg.dragFloat = static_cast<float>(ini.GetDoubleValue("Widgets", "DragFloat", _def.dragFloat));
		_cfg.dragInt = static_cast<std::int32_t>(ini.GetLongValue("Widgets", "DragInt", _def.dragInt));

		const char* inputBuf = ini.GetValue("Widgets", "InputBuffer", nullptr);
		if (inputBuf)
			strncpy_s(_cfg.inputBuffer, inputBuf, sizeof(_cfg.inputBuffer));

		_cfg.overlayPos.x = static_cast<float>(ini.GetDoubleValue("Overlay", "X", _def.overlayPos.x));
		_cfg.overlayPos.y = static_cast<float>(ini.GetDoubleValue("Overlay", "Y", _def.overlayPos.y));
		_cfg.overlaySize.x = static_cast<float>(ini.GetDoubleValue("Overlay", "Width", _def.overlaySize.x));
		_cfg.overlaySize.y = static_cast<float>(ini.GetDoubleValue("Overlay", "Height", _def.overlaySize.y));
		if (_cfg.overlayPos.x != -1.0f && _cfg.overlayPos.y != -1.0f)
			_overlay._hasLoadedPos = true;

		_cfg.reqBlur = ini.GetBoolValue("OverlayFlags", "Blur", _def.reqBlur);
		_cfg.reqHideHUD = ini.GetBoolValue("OverlayFlags", "HideHUD", _def.reqHideHUD);
		_cfg.reqPauseHard = ini.GetBoolValue("OverlayFlags", "PauseHard", _def.reqPauseHard);
		_cfg.reqPauseSoft = ini.GetBoolValue("OverlayFlags", "PauseSoft", _def.reqPauseSoft);
		_cfg.reqCloseOnEsc = ini.GetBoolValue("OverlayFlags", "CloseOnEsc", _def.reqCloseOnEsc);
		_cfg.reqCloseOnMenu = ini.GetBoolValue("OverlayFlags", "CloseOnMenu", _def.reqCloseOnMenu);
		_cfg.reqPassInput = ini.GetBoolValue("OverlayFlags", "PassInput", _def.reqPassInput);
		_cfg.reqBlockVanity = ini.GetBoolValue("OverlayFlags", "BlockVanity", _def.reqBlockVanity);
		_cfg.reqNoBackground = ini.GetBoolValue("OverlayFlags", "NoBackground", _def.reqNoBackground);
		_cfg.reqNoDecoration = ini.GetBoolValue("OverlayFlags", "NoDecoration", _def.reqNoDecoration);
		_cfg.reqExtendBorder = ini.GetBoolValue("OverlayFlags", "ExtendBorder", _def.reqExtendBorder);

		_cfg.secondOverlayPos.x = static_cast<float>(ini.GetDoubleValue("SecondWindow", "X", _def.secondOverlayPos.x));
		_cfg.secondOverlayPos.y = static_cast<float>(ini.GetDoubleValue("SecondWindow", "Y", _def.secondOverlayPos.y));
		_cfg.secondPassInput = ini.GetBoolValue("SecondWindow", "PassInput", _def.secondPassInput);

		_cfg.hudWidgetPos.x = static_cast<float>(ini.GetDoubleValue("HudWidget", "X", _def.hudWidgetPos.x));
		_cfg.hudWidgetPos.y = static_cast<float>(ini.GetDoubleValue("HudWidget", "Y", _def.hudWidgetPos.y));
		if (_cfg.hudWidgetPos.x != -1.0f && _cfg.hudWidgetPos.y != -1.0f)
			_hudWidget._hasLoadedPos = true;

		_cfg.hudKeepOpen = ini.GetBoolValue("HudWidget", "KeepOpen", _def.hudKeepOpen);
		_cfg.hudScale = static_cast<float>(ini.GetDoubleValue("HudWidget", "Scale", _def.hudScale));

		_cfg.showOverlay = ini.GetBoolValue("Rendering", "ShowOverlay", _def.showOverlay);
		_cfg.overlayType = static_cast<int>(ini.GetLongValue("Rendering", "OverlayType", _def.overlayType));
		_cfg.overlayThickness = static_cast<float>(ini.GetDoubleValue("Rendering", "OverlayThickness", _def.overlayThickness));
		_cfg.overlayColor[0] = static_cast<float>(ini.GetDoubleValue("Rendering", "OverlayColorR", _def.overlayColor[0]));
		_cfg.overlayColor[1] = static_cast<float>(ini.GetDoubleValue("Rendering", "OverlayColorG", _def.overlayColor[1]));
		_cfg.overlayColor[2] = static_cast<float>(ini.GetDoubleValue("Rendering", "OverlayColorB", _def.overlayColor[2]));
		_cfg.overlayColor[3] = static_cast<float>(ini.GetDoubleValue("Rendering", "OverlayColorA", _def.overlayColor[3]));
		_cfg.gridRows = static_cast<int>(ini.GetLongValue("Rendering", "GridRows", _def.gridRows));
		_cfg.gridCols = static_cast<int>(ini.GetLongValue("Rendering", "GridCols", _def.gridCols));
		_cfg.spiralAnchor = static_cast<int>(ini.GetLongValue("Rendering", "SpiralAnchor", _def.spiralAnchor));
		_cfg.spiralRot = static_cast<float>(ini.GetDoubleValue("Rendering", "SpiralRot", _def.spiralRot));
		_cfg.spiralScale = static_cast<float>(ini.GetDoubleValue("Rendering", "SpiralScale", _def.spiralScale));
		_cfg.spiralTurns = static_cast<float>(ini.GetDoubleValue("Rendering", "SpiralTurns", _def.spiralTurns));
		_cfg.showSquares = ini.GetBoolValue("Rendering", "ShowSquares", _def.showSquares);
		_cfg.triMirror = ini.GetBoolValue("Rendering", "TriMirror", _def.triMirror);
	});

	GetSettings().LoadKeybinds([this](CSimpleIniA& ini) {
		_toggleHotkey.kKey = static_cast<std::uint32_t>(ini.GetLongValue("Overlay", "Hotkey", _defHotkey.kKey));
		_toggleHotkey.kMod1 = static_cast<std::int32_t>(ini.GetLongValue("Overlay", "Modifier1", _defHotkey.kMod1));
		_toggleHotkey.kMod2 = static_cast<std::int32_t>(ini.GetLongValue("Overlay", "Modifier2", _defHotkey.kMod2));
		_toggleHotkey.gKey = static_cast<std::uint32_t>(ini.GetLongValue("Overlay", "GPHotkey", _defHotkey.gKey));
		_toggleHotkey.gMod1 = static_cast<std::int32_t>(ini.GetLongValue("Overlay", "GPModifier1", _defHotkey.gMod1));
		_toggleHotkey.gMod2 = static_cast<std::int32_t>(ini.GetLongValue("Overlay", "GPModifier2", _defHotkey.gMod2));
	});
}

void DemoState::SaveSettings()
{
	GetSettings().Save([this](CSimpleIniA& ini) {
		// General Widgets
		FUCK::INI::SaveBool(ini, "Widgets", "ChkNear", _cfg.chkNear, _def.chkNear);
		FUCK::INI::SaveBool(ini, "Widgets", "MutexA", _cfg.chkMutexA, _def.chkMutexA);
		FUCK::INI::SaveBool(ini, "Widgets", "MutexB", _cfg.chkMutexB, _def.chkMutexB);
		FUCK::INI::SaveBool(ini, "Widgets", "ChkFarA", _cfg.chkFarA, _def.chkFarA);
		FUCK::INI::SaveBool(ini, "Widgets", "ChkFarB", _cfg.chkFarB, _def.chkFarB);
		FUCK::INI::SaveBool(ini, "Widgets", "ToggleState", _cfg.toggleState, _def.toggleState);
		FUCK::INI::SaveDouble(ini, "Widgets", "SliderVal", _cfg.sliderVal, _def.sliderVal);
		FUCK::INI::SaveInt(ini, "Widgets", "IntVal", _cfg.intVal, _def.intVal);
		FUCK::INI::SaveDouble(ini, "Widgets", "DragFloat", _cfg.dragFloat, _def.dragFloat);
		FUCK::INI::SaveInt(ini, "Widgets", "DragInt", _cfg.dragInt, _def.dragInt);
		FUCK::INI::SaveString(ini, "Widgets", "InputBuffer", _cfg.inputBuffer, _def.inputBuffer);

		// Overlay Position & Size
		ImVec2 overlayDefPos = _overlay.GetDefaultPos();
		ImVec2 overlayDefSize = _overlay.GetDefaultSize();
		FUCK::INI::SaveDouble(ini, "Overlay", "X", _cfg.overlayPos.x, overlayDefPos.x);
		FUCK::INI::SaveDouble(ini, "Overlay", "Y", _cfg.overlayPos.y, overlayDefPos.y);
		FUCK::INI::SaveDouble(ini, "Overlay", "Width", _cfg.overlaySize.x, overlayDefSize.x);
		FUCK::INI::SaveDouble(ini, "Overlay", "Height", _cfg.overlaySize.y, overlayDefSize.y);

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
		ImVec2 secDefPos = _secondOverlay.GetDefaultPos();
		FUCK::INI::SaveDouble(ini, "SecondWindow", "X", _cfg.secondOverlayPos.x, secDefPos.x);
		FUCK::INI::SaveDouble(ini, "SecondWindow", "Y", _cfg.secondOverlayPos.y, secDefPos.y);
		FUCK::INI::SaveBool(ini, "SecondWindow", "PassInput", _cfg.secondPassInput, _def.secondPassInput);

		// HUD Widget
		ImVec2 hudDefPos = _hudWidget.GetDefaultPos();
		FUCK::INI::SaveDouble(ini, "HudWidget", "X", _cfg.hudWidgetPos.x, hudDefPos.x);
		FUCK::INI::SaveDouble(ini, "HudWidget", "Y", _cfg.hudWidgetPos.y, hudDefPos.y);
		FUCK::INI::SaveBool(ini, "HudWidget", "KeepOpen", _cfg.hudKeepOpen, _def.hudKeepOpen);
		FUCK::INI::SaveDouble(ini, "HudWidget", "Scale", _cfg.hudScale, _def.hudScale);

		// Rendering
		FUCK::INI::SaveBool(ini, "Rendering", "ShowOverlay", _cfg.showOverlay, _def.showOverlay);
		FUCK::INI::SaveInt(ini, "Rendering", "OverlayType", _cfg.overlayType, _def.overlayType);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayThickness", _cfg.overlayThickness, _def.overlayThickness);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorR", _cfg.overlayColor[0], _def.overlayColor[0]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorG", _cfg.overlayColor[1], _def.overlayColor[1]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorB", _cfg.overlayColor[2], _def.overlayColor[2]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorA", _cfg.overlayColor[3], _def.overlayColor[3]);
		FUCK::INI::SaveInt(ini, "Rendering", "GridRows", _cfg.gridRows, _def.gridRows);
		FUCK::INI::SaveInt(ini, "Rendering", "GridCols", _cfg.gridCols, _def.gridCols);
		FUCK::INI::SaveInt(ini, "Rendering", "SpiralAnchor", _cfg.spiralAnchor, _def.spiralAnchor);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralRot", _cfg.spiralRot, _def.spiralRot);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralScale", _cfg.spiralScale, _def.spiralScale);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralTurns", _cfg.spiralTurns, _def.spiralTurns);
		FUCK::INI::SaveBool(ini, "Rendering", "ShowSquares", _cfg.showSquares, _def.showSquares);
		FUCK::INI::SaveBool(ini, "Rendering", "TriMirror", _cfg.triMirror, _def.triMirror);
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

// ==========================================
// Simple Secondary Overlay
// ==========================================

FUCK::WindowFlags SimpleOverlay::GetFlags() const
{
	FUCK::WindowFlags f = FUCK::WindowFlags::kNoDecoration;
	if (DemoState::GetSingleton()->_cfg.secondPassInput)
		f = f | FUCK::WindowFlags::kPassInputToGame;
	return f;
}

ImVec2 SimpleOverlay::GetDefaultPos() const
{
	float scale = FUCK::GetResolutionScale();
	return { DemoState::GetSingleton()->_def.secondOverlayPos.x * scale, DemoState::GetSingleton()->_def.secondOverlayPos.y * scale };
}

ImVec2 SimpleOverlay::GetDefaultSize() const
{
	float scale = FUCK::GetResolutionScale();
	return { 400.0f * scale, 300.0f * scale };
}

bool SimpleOverlay::GetRequestedPos(ImVec2& outPos)
{
	auto cfgPos = DemoState::GetSingleton()->_cfg.secondOverlayPos;
	if (cfgPos.x > 0.0f || cfgPos.y > 0.0f) {
		outPos = cfgPos;
		return true;
	}
	return false;
}

void SimpleOverlay::UpdateState(const ImVec2& currentPos, const ImVec2& /*currentSize*/)
{
	DemoState::GetSingleton()->_cfg.secondOverlayPos = currentPos;
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

// ==========================================
// HUD Widget Implementation
// ==========================================

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

ImVec2 HudWidget::GetDefaultSize() const
{
	if (const_cast<HudWidget*>(this)->_hudImage.IsLoaded()) {
		float s = FUCK::GetResolutionScale();
		float padding = 12.0f * s;
		return { (_hudImage.GetWidth() * s * DemoState::GetSingleton()->_cfg.hudScale) + (padding * 2.0f),
			(_hudImage.GetHeight() * s * DemoState::GetSingleton()->_cfg.hudScale) + (padding * 2.0f) };
	}
	return { 100.0f, 100.0f };
}

bool HudWidget::GetRequestedPos(ImVec2& outPos)
{
	if (_hasLoadedPos) {
		outPos = DemoState::GetSingleton()->_cfg.hudWidgetPos;
		return true;
	}
	return false;
}

void HudWidget::UpdateState(const ImVec2& currentPos, const ImVec2& /*currentSize*/)
{
	DemoState::GetSingleton()->_cfg.hudWidgetPos = currentPos;

	if (_lastSavedPos.x == -1.0f) {
		_lastSavedPos = currentPos;
	}
}

void HudWidget::Draw()
{
	bool isInteractable = FUCK::IsMenuOpen();
	auto& cfg = DemoState::GetSingleton()->_cfg;

	if (isInteractable && FUCK::IsMouseReleased(0)) {
		if (cfg.hudWidgetPos.x != _lastSavedPos.x || cfg.hudWidgetPos.y != _lastSavedPos.y) {
			_lastSavedPos = cfg.hudWidgetPos;
			DemoState::GetSingleton()->SaveSettings();
		}
	}

	if (!_hudImage.IsLoaded()) {
		_hudImage = FUCK::Image("Data/Interface/test.png", false);
	}

	if (_hudImage.IsLoaded()) {
		float resScale = FUCK::GetResolutionScale();
		ImVec2 imageSize = { _hudImage.GetWidth() * resScale * cfg.hudScale, _hudImage.GetHeight() * resScale * cfg.hudScale };

		float padding = 12.0f * resScale;
		FUCK::SetCursorPos({ padding, padding });

		ImVec2 pMin = FUCK::GetCursorScreenPos();
		ImVec2 pMax = { pMin.x + imageSize.x, pMin.y + imageSize.y };

		FUCK::DrawImage(_hudImage.GetID(), imageSize);

		if (isInteractable) {
			bool isHovered = FUCK::IsWindowHovered(0);

			float gap = 4.0f * resScale;
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

// ==========================================
// Demo Overlay Implementation
// ==========================================

FUCK::WindowFlags DemoOverlay::GetFlags() const
{
	auto& cfg = DemoState::GetSingleton()->_cfg;
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

ImVec2 DemoOverlay::GetDefaultPos() const
{
	float scale = FUCK::GetResolutionScale();
	ImVec2 displaySize = FUCK::GetDisplaySize();
	ImVec2 scaledSize = GetDefaultSize();

	float offset = 50.0f * scale;

	return {
		displaySize.x - scaledSize.x - offset,
		100.0f * scale
	};
}

ImVec2 DemoOverlay::GetDefaultSize() const
{
	float scale = FUCK::GetResolutionScale();
	return { _baseSize.x * scale, _baseSize.y * scale };
}

bool DemoOverlay::GetRequestedPos(ImVec2& outPos)
{
	if (_hasLoadedPos) {
		outPos = DemoState::GetSingleton()->_cfg.overlayPos;
		return true;
	}
	return false;
}

void DemoOverlay::UpdateState(const ImVec2& currentPos, const ImVec2& currentSize)
{
	DemoState::GetSingleton()->_cfg.overlayPos = currentPos;
	DemoState::GetSingleton()->_cfg.overlaySize = currentSize;

	if (_lastSavedPos.x == -1.0f) {
		_lastSavedPos = currentPos;
		_lastSavedSize = currentSize;
	}
}

void DemoOverlay::Draw()
{
	auto& cfg = DemoState::GetSingleton()->_cfg;

	if (FUCK::IsMouseReleased(0)) {  // 0 = Left Mouse Button
		if (cfg.overlayPos.x != _lastSavedPos.x || cfg.overlayPos.y != _lastSavedPos.y ||
			cfg.overlaySize.x != _lastSavedSize.x || cfg.overlaySize.y != _lastSavedSize.y) {
			_lastSavedPos = cfg.overlayPos;
			_lastSavedSize = cfg.overlaySize;
			DemoState::GetSingleton()->SaveSettings();
		}
	}

	FUCK::Header("$DEMO_Section_WindowMetrics"_T);
	FUCK::Text("$DEMO_MetricSize"_T, cfg.overlaySize.x, cfg.overlaySize.y);
	FUCK::Text("$DEMO_MetricPos"_T, cfg.overlayPos.x, cfg.overlayPos.y);
	FUCK::Spacing();

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
	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_OverlayControls"_T);
	auto state = DemoState::GetSingleton();
	FUCK::DrawManagedHotkey("$DEMO_OverlayToggleKey"_T, state->_toggleHotkey);
	FUCK::SetTooltip("$DEMO_OverlayKeyTip"_T);
	FUCK::Spacing();

	if (FUCK::Button("$DEMO_ResetHotkey"_T)) {
		state->_toggleHotkey.kKey = state->_defHotkey.kKey;
		state->_toggleHotkey.kMod1 = state->_defHotkey.kMod1;
		state->_toggleHotkey.kMod2 = state->_defHotkey.kMod2;
		state->_toggleHotkey.gKey = state->_defHotkey.gKey;
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
