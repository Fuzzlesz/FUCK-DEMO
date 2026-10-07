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

	// Setup HUD widget assets and hooks
	_hudWidget.Initialize();

	// Register Split Tools. Inside a group they show in the order they're registered, and a nested group sits where
	// its first tool is. So API Updates, whose pages come last, sits below General, Visuals and System, and
	// Version 2/3 sits above Version 4/5/6. See "Sidebar Tools & Groups" in DEMO.h
	FUCK::RegisterTool(&_toolGeneral);
	FUCK::RegisterTool(&_toolVisuals);
	FUCK::RegisterTool(&_toolSystem);
	FUCK::RegisterTool(&_toolAPIv2);
	FUCK::RegisterTool(&_toolAPIv3);
	FUCK::RegisterTool(&_toolAPIv4);
	FUCK::RegisterTool(&_toolAPIv5);
	FUCK::RegisterTool(&_toolAPIv6);

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

	// Ensure the hotkey is always released when the FUCK menu closes
	if (_disableHotkey) {
		_disableHotkey = false;
		FUCK::SetHotkeyEnabled(true);
	}
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
		_cfg.chkNear     = FUCK::INI::LoadBool(ini, "Widgets", "ChkNear",     _def.chkNear);
		_cfg.chkMutexA   = FUCK::INI::LoadBool(ini, "Widgets", "MutexA",      _def.chkMutexA);
		_cfg.chkMutexB   = FUCK::INI::LoadBool(ini, "Widgets", "MutexB",      _def.chkMutexB);
		_cfg.chkFarA     = FUCK::INI::LoadBool(ini, "Widgets", "ChkFarA",     _def.chkFarA);
		_cfg.chkFarB     = FUCK::INI::LoadBool(ini, "Widgets", "ChkFarB",     _def.chkFarB);
		_cfg.toggleState = FUCK::INI::LoadBool(ini, "Widgets", "ToggleState", _def.toggleState);

		_cfg.sliderVal = FUCK::INI::LoadFloat(ini, "Widgets", "SliderVal", _def.sliderVal);
		_cfg.intVal    = FUCK::INI::LoadInt  (ini, "Widgets", "IntVal",    _def.intVal);
		_cfg.dragFloat = FUCK::INI::LoadFloat(ini, "Widgets", "DragFloat", _def.dragFloat);
		_cfg.dragInt   = FUCK::INI::LoadInt  (ini, "Widgets", "DragInt",   _def.dragInt);

		const char* inputBuf = ini.GetValue("Widgets", "InputBuffer", nullptr);
		if (inputBuf)
			FUCK::StringCopy(_cfg.inputBuffer, inputBuf);

		_cfg.showOverlay      = FUCK::INI::LoadBool (ini, "Rendering", "ShowOverlay",      _def.showOverlay);
		_cfg.overlayType      = FUCK::INI::LoadInt  (ini, "Rendering", "OverlayType",      _def.overlayType);
		_cfg.overlayThickness = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayThickness", _def.overlayThickness);

		_cfg.overlayColor[0] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorR", _def.overlayColor[0]);
		_cfg.overlayColor[1] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorG", _def.overlayColor[1]);
		_cfg.overlayColor[2] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorB", _def.overlayColor[2]);
		_cfg.overlayColor[3] = FUCK::INI::LoadFloat(ini, "Rendering", "OverlayColorA", _def.overlayColor[3]);

		_cfg.gridRows     = FUCK::INI::LoadInt  (ini, "Rendering", "GridRows",     _def.gridRows);
		_cfg.gridCols     = FUCK::INI::LoadInt  (ini, "Rendering", "GridCols",     _def.gridCols);
		_cfg.spiralAnchor = FUCK::INI::LoadInt  (ini, "Rendering", "SpiralAnchor", _def.spiralAnchor);
		_cfg.spiralRot    = FUCK::INI::LoadFloat(ini, "Rendering", "SpiralRot",    _def.spiralRot);
		_cfg.spiralScale  = FUCK::INI::LoadFloat(ini, "Rendering", "SpiralScale",  _def.spiralScale);
		_cfg.spiralTurns  = FUCK::INI::LoadFloat(ini, "Rendering", "SpiralTurns",  _def.spiralTurns);
		_cfg.showSquares  = FUCK::INI::LoadBool (ini, "Rendering", "ShowSquares",  _def.showSquares);
		_cfg.triMirror    = FUCK::INI::LoadBool (ini, "Rendering", "TriMirror",    _def.triMirror);
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
		FUCK::INI::SaveBool  (ini, "Widgets", "ChkNear",     _cfg.chkNear,     _def.chkNear);
		FUCK::INI::SaveBool  (ini, "Widgets", "MutexA",      _cfg.chkMutexA,   _def.chkMutexA);
		FUCK::INI::SaveBool  (ini, "Widgets", "MutexB",      _cfg.chkMutexB,   _def.chkMutexB);
		FUCK::INI::SaveBool  (ini, "Widgets", "ChkFarA",     _cfg.chkFarA,     _def.chkFarA);
		FUCK::INI::SaveBool  (ini, "Widgets", "ChkFarB",     _cfg.chkFarB,     _def.chkFarB);
		FUCK::INI::SaveBool  (ini, "Widgets", "ToggleState", _cfg.toggleState, _def.toggleState);
		FUCK::INI::SaveDouble(ini, "Widgets", "SliderVal",   _cfg.sliderVal,   _def.sliderVal);
		FUCK::INI::SaveInt   (ini, "Widgets", "IntVal",      _cfg.intVal,      _def.intVal);
		FUCK::INI::SaveDouble(ini, "Widgets", "DragFloat",   _cfg.dragFloat,   _def.dragFloat);
		FUCK::INI::SaveInt   (ini, "Widgets", "DragInt",     _cfg.dragInt,     _def.dragInt);
		FUCK::INI::SaveString(ini, "Widgets", "InputBuffer", _cfg.inputBuffer, _def.inputBuffer);

		// Rendering
		FUCK::INI::SaveBool  (ini, "Rendering", "ShowOverlay",      _cfg.showOverlay,      _def.showOverlay);
		FUCK::INI::SaveInt   (ini, "Rendering", "OverlayType",      _cfg.overlayType,      _def.overlayType);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayThickness", _cfg.overlayThickness, _def.overlayThickness);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorR",    _cfg.overlayColor[0],  _def.overlayColor[0]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorG",    _cfg.overlayColor[1],  _def.overlayColor[1]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorB",    _cfg.overlayColor[2],  _def.overlayColor[2]);
		FUCK::INI::SaveDouble(ini, "Rendering", "OverlayColorA",    _cfg.overlayColor[3],  _def.overlayColor[3]);
		FUCK::INI::SaveInt   (ini, "Rendering", "GridRows",         _cfg.gridRows,         _def.gridRows);
		FUCK::INI::SaveInt   (ini, "Rendering", "GridCols",         _cfg.gridCols,         _def.gridCols);
		FUCK::INI::SaveInt   (ini, "Rendering", "SpiralAnchor",     _cfg.spiralAnchor,     _def.spiralAnchor);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralRot",        _cfg.spiralRot,        _def.spiralRot);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralScale",      _cfg.spiralScale,      _def.spiralScale);
		FUCK::INI::SaveDouble(ini, "Rendering", "SpiralTurns",      _cfg.spiralTurns,      _def.spiralTurns);
		FUCK::INI::SaveBool  (ini, "Rendering", "ShowSquares",      _cfg.showSquares,      _def.showSquares);
		FUCK::INI::SaveBool  (ini, "Rendering", "TriMirror",        _cfg.triMirror,        _def.triMirror);
	});
}

void DemoState::SaveKeybinds()
{
	GetSettings().SaveKeybinds([this](CSimpleIniA& ini) {
		FUCK::INI::SaveInt(ini, "Overlay", "Hotkey",      _toggleHotkey.kKey,  _defHotkey.kKey);
		FUCK::INI::SaveInt(ini, "Overlay", "Modifier1",   _toggleHotkey.kMod1, _defHotkey.kMod1);
		FUCK::INI::SaveInt(ini, "Overlay", "Modifier2",   _toggleHotkey.kMod2, _defHotkey.kMod2);
		FUCK::INI::SaveInt(ini, "Overlay", "GPHotkey",    _toggleHotkey.gKey,  _defHotkey.gKey);
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

void HudWidget::Initialize()
{
	std::string imagePath = GetSettings().GetConfigDirectory() + _imageFilename;
	_hudImage             = FUCK::Image(imagePath.c_str(), false);

	_menuListener = FUCK::MenuEventListener([this](const char* menuName, bool opening) {
		if (menuName == RE::HUDMenu::MENU_NAME) {
			_hudMenuOpen = opening;
		}
	});
}

FUCK::WindowFlags HudWidget::GetFlags() const
{
	auto& cfg = DemoState::GetSingleton()->_cfg;

	FUCK::WindowFlags flags = FUCK::WindowFlags::kNoDecoration  |
	                          FUCK::WindowFlags::kNoBackground  |
	                          FUCK::WindowFlags::kAutoResize    |
	                          FUCK::WindowFlags::kRenderDuringTM;

	if (!FUCK::IsMenuOpen()) {
		flags = flags | FUCK::WindowFlags::kPassInputToGame;
	}

	if (!cfg.hudKeepOpen)
		flags = flags | FUCK::WindowFlags::kCloseOnGameMenu;

	// The host moves and saves it while the FUCK menu is open. The other edit flags build on this one
	flags = flags | FUCK::WindowFlags::kEditable;
	if (cfg.hudScalable)
		flags = flags | FUCK::WindowFlags::kScalable;
	if (cfg.hudAnchorRight)
		flags = flags | FUCK::WindowFlags::kAnchorRight;
	if (cfg.hudRotatable)
		flags = flags | FUCK::WindowFlags::kRotatable;

	return flags;
}

ImVec2 HudWidget::GetDefaultPos() const
{
	ImVec2 pos = FUCK::Scale(100.0f, 100.0f);

	// With kAnchorRight, the default position is the top-right corner
	if (DemoState::GetSingleton()->_cfg.hudAnchorRight)
		pos.x += GetDefaultSize().x;

	return pos;
}

ImVec2 HudWidget::GetDefaultSize() const
{
	if (_hudImage.IsLoaded()) {
		ImVec2 imageSize = FUCK::Scale(ImVec2(_hudImage.GetWidth(), _hudImage.GetHeight()));
		float  padding   = FUCK::Scale(15.0f);
		return { imageSize.x + (padding * 2.0f), imageSize.y + (padding * 2.0f) };
	}
	return FUCK::Scale(100.0f, 100.0f);
}

void HudWidget::Draw()
{
	bool  isInteractable = FUCK::IsMenuOpen();
	auto& cfg            = DemoState::GetSingleton()->_cfg;

	if (!_hudMenuOpen && !cfg.hudKeepOpen && !isInteractable) {
		return;
	}

	if (_hudImage.IsLoaded()) {
		// The host's scale handles set GetWindowScale(), and the widget applies it. It's 1 without kScalable
		float  scale     = FUCK::GetWindowScale();
		ImVec2 imageSize = FUCK::Scale(ImVec2(_hudImage.GetWidth() * scale, _hudImage.GetHeight() * scale));
		float  padding   = FUCK::Scale(15.0f);

		// Reserves the space, so the auto-sized window fits the image
		ImVec2 start = FUCK::GetCursorScreenPos();
		FUCK::Dummy({ imageSize.x + padding * 2.0f, imageSize.y + padding * 2.0f });

		// The host's rotation handle sets GetWindowRotation(), which drawn content like images can follow. It's 0 without kRotatable
		ImVec2 imagePos = { start.x + padding, start.y + padding };
		FUCK::DrawImageRotated(_hudImage.GetID(), imagePos, imageSize, FUCK::GetWindowRotation());
	} else {
		FUCK::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "$DEMO_ImageMissing"_T);
	}
}

// ==================================================
// Demo Overlay Window Implementation
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
	if (cfg.reqRenderDuringTM)
		flags = flags | FUCK::WindowFlags::kRenderDuringTM;
	if (cfg.reqIgnoreUserScale)
		flags = flags | FUCK::WindowFlags::kIgnoreUserScale;
	if (cfg.reqCustomPosition)
		flags = flags | FUCK::WindowFlags::kCustomPosition;
	if (cfg.reqCloseOnClickOut)
		flags = flags | FUCK::WindowFlags::kCloseOnClickOutside;
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

	// kCustomPosition leaves the position to the plugin. While it's ticked, this pins the window to the centre of the screen
	if (cfg.reqCustomPosition) {
		ImVec2 size    = GetDefaultSize();
		ImVec2 display = FUCK::GetDisplaySize();
		FUCK::SetWindowSize(size);
		FUCK::SetWindowPos({ (display.x - size.x) * 0.5f, (display.y - size.y) * 0.5f });
	}

	ImVec2 curPos  = FUCK::GetWindowPos();
	ImVec2 curSize = FUCK::GetWindowSize();

	FUCK::Header("$DEMO_Section_WindowMetrics"_T);
	FUCK::Text("$DEMO_MetricSize"_T, curSize.x, curSize.y);
	FUCK::Text("$DEMO_MetricPos"_T, curPos.x, curPos.y);

	FUCK::Header("$DEMO_Section_WindowFlags"_T);
	FUCK::Text("$DEMO_WindowFlags_Desc"_T);

	// Session only, so the overlay starts plain each time
	FUCK::Checkbox("$DEMO_Flag_Blur"_T, &cfg.reqBlur, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_Blur"_T);
	FUCK::Checkbox("$DEMO_Flag_HideHUD"_T, &cfg.reqHideHUD, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_HideHUD"_T);
	FUCK::Checkbox("$DEMO_Flag_PauseHard"_T, &cfg.reqPauseHard, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_PauseHard"_T);
	FUCK::Checkbox("$DEMO_Flag_PauseSoft"_T, &cfg.reqPauseSoft, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_PauseSoft"_T);
	FUCK::Checkbox("$DEMO_Flag_CloseOnEsc"_T, &cfg.reqCloseOnEsc, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnEsc"_T);
	FUCK::Checkbox("$DEMO_Flag_CloseOnMenu"_T, &cfg.reqCloseOnMenu, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnMenu"_T);

	if (FUCK::Checkbox("$DEMO_Flag_PassInput"_T, &cfg.reqPassInput, false, false)) {
		if (cfg.reqPassInput) {
			FUCK::SetMenuOpen(false);
			std::thread([this]() {
				std::this_thread::sleep_for(std::chrono::seconds(3));
				DemoState::GetSingleton()->_cfg.reqPassInput = false;
			}).detach();
		}
	}
	FUCK::SetTooltip("$DEMO_PassInputTip"_T);
	FUCK::Checkbox("$DEMO_Flag_BlockVanity"_T, &cfg.reqBlockVanity, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_BlockVanity"_T);
	FUCK::Checkbox("$DEMO_Flag_NoBackground"_T, &cfg.reqNoBackground, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_NoBackground"_T);
	FUCK::Checkbox("$DEMO_Flag_NoDecoration"_T, &cfg.reqNoDecoration, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_NoDecoration"_T);
	FUCK::Checkbox("$DEMO_Flag_ExtendBorder"_T, &cfg.reqExtendBorder, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_ExtendBorder"_T);
	FUCK::Checkbox("$DEMO_Flag_RenderDuringTM"_T, &cfg.reqRenderDuringTM, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_RenderDuringTM"_T);
	FUCK::Checkbox("$DEMO_Flag_IgnoreUserScale"_T, &cfg.reqIgnoreUserScale, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_IgnoreUserScale"_T);
	FUCK::Checkbox("$DEMO_Flag_CustomPosition"_T, &cfg.reqCustomPosition, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CustomPosition"_T);
	FUCK::Checkbox("$DEMO_Flag_CloseOnClickOut"_T, &cfg.reqCloseOnClickOut, false, false);
	FUCK::SetTooltip("$DEMO_FlagTip_CloseOnClickOut"_T);

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
