#pragma once

inline FUCK::PluginSettings& GetSettings()
{
	static FUCK::PluginSettings s;
	return s;
}

// ==================================================
// Secondary Simple Overlay
// ==================================================

class SimpleOverlay : public FUCK::IWindow
{
public:
	const char* Id() const override { return "SimpleOverlay"; }
	const char* Title() const override { return "$DEMO_SecondOverlayTitle"_T; }
	void        Draw() override;
	bool        IsOpen() const override { return _isOpen; }
	void        SetOpen(bool a_open) override { _isOpen = a_open; }

	FUCK::WindowFlags GetFlags() const override;

	ImVec2 GetDefaultPos() const override;
	ImVec2 GetDefaultSize() const override;

	bool     _isOpen       = false;
	IWindow* _parentWindow = nullptr;
};

// ==================================================
// HUD Widget Demo
// ==================================================

class HudWidget : public FUCK::IWindow
{
public:
	const char* Id() const override { return "HudWidget"; }
	const char* Title() const override { return "$DEMO_HudWidgetTitle"_T; }

	void Initialize();

	void Draw() override;
	bool IsOpen() const override { return _isOpen; }
	void SetOpen(bool a_open) override { _isOpen = a_open; }

	FUCK::WindowFlags GetFlags() const override;

	ImVec2 GetDefaultPos() const override;
	ImVec2 GetDefaultSize() const override;

	bool _isOpen      = false;
	bool _wantOpen    = false;
	bool _hudMenuOpen = false;

	const std::string _imageFilename = "test.png";

	FUCK::Image             _hudImage;
	FUCK::MenuEventListener _menuListener;
};

// ==================================================
// Demo Overlay Window (Main)
// ==================================================

class DemoOverlay : public FUCK::IWindow
{
public:
	const char* Id() const override { return "DemoOverlay"; }
	const char* Title() const override { return "$DEMO_OverlayTitleHint"_T; }
	void        Draw() override;
	bool        IsOpen() const override { return _isOpen; }
	void        SetOpen(bool a_open) override { _isOpen = a_open; }

	FUCK::WindowFlags GetFlags() const override;

	ImVec2 GetDefaultSize() const override;
	ImVec2 GetDefaultPos() const override;

	bool _isOpen = false;

	const ImVec2 _baseSize{ 430.0f, 855.0f };

	SimpleOverlay* _secondWindow = nullptr;
};

// ==================================================
// Demo State Manager & Tools
// ==================================================

class DemoState : public REX::Singleton<DemoState>
{
public:
	struct DemoConfig
	{
		// Basic Widgets
		bool         chkNear          = false;
		bool         chkMutexA        = true;
		bool         chkMutexB        = false;
		bool         chkFarA          = false;
		bool         chkFarB          = true;
		bool         toggleState      = false;
		float        sliderVal        = 50.0f;
		std::int32_t intVal           = 5;
		float        dragFloat        = 0.5f;
		std::int32_t dragInt          = 10;
		char         inputBuffer[256] = { 0 };

		// Overlay flags, session only
		bool reqBlur            = false;
		bool reqHideHUD         = false;
		bool reqPauseHard       = false;
		bool reqPauseSoft       = false;
		bool reqCloseOnEsc      = false;
		bool reqCloseOnMenu     = false;
		bool reqPassInput       = false;
		bool reqBlockVanity     = false;
		bool reqNoBackground    = false;
		bool reqNoDecoration    = false;
		bool reqExtendBorder    = false;
		bool reqRenderDuringTM  = false;
		bool reqIgnoreUserScale = false;
		bool reqCustomPosition  = false;
		bool reqCloseOnClickOut = false;

		bool secondPassInput = false;

		// HUD widget flags, session only
		bool hudKeepOpen    = false;
		bool hudScalable    = false;
		bool hudAnchorRight = false;
		bool hudRotatable   = false;

		// Rendering Overlays Demo
		bool                 showOverlay      = false;
		int                  overlayType      = 0;
		float                overlayThickness = 2.0f;
		std::array<float, 4> overlayColor     = { 1.0f, 1.0f, 1.0f, 0.5f };
		int                  gridRows         = 10;
		int                  gridCols         = 10;
		int                  spiralAnchor     = 0;
		float                spiralRot        = 0.0f;
		float                spiralScale      = 1.0f;
		float                spiralTurns      = 6.0f;
		bool                 showSquares      = false;
		bool                 triMirror        = false;
	};

	struct HotkeyConfig
	{
		std::uint32_t kKey  = 35;  // H
		std::int32_t  kMod1 = 42;  // Shift
		std::int32_t  kMod2 = -1;
		std::uint32_t gKey  = 0;
		std::int32_t  gMod1 = -1;
		std::int32_t  gMod2 = -1;
	};

	DemoState();

	void OnOpen();
	void OnClose();
	bool OnAsyncInput(const void* inputEvent);

	// Helper to save/load settings
	void LoadSettings();
	void SaveSettings();
	void SaveKeybinds();

	DemoOverlay* GetOverlay() { return &_overlay; }

	// ==================================================
	// Sidebar Tools & Groups
	// ==================================================

	// A tool's Group() is its sidebar group. A group exists once a tool names it, and tools naming the same one share it.
	//   nullptr               top level, no group
	//   "Camera"              a group called Camera
	//   "Camera::Presets"     Presets, inside Camera. Up to 4 levels deep
	//
	// The top level is alphabetical. Inside a group, things follow registration order (see DemoState's constructor).
	// If a tool's name or group changes at runtime, call FUCK::RefreshTools(). The user can rearrange it all in Settings.
	//
	// The demo builds this tree:
	//   DEMO - API General
	//   ├ General Demo
	//   ├ Visuals
	//   ├ System
	//   └ API Updates
	//     ├ Version 2/3
	//     │ ├ Version 2
	//     │ └ Version 3
	//     └ Version 4/5/6
	//       ├ Version 4
	//       ├ Version 5
	//       └ Version 6

	// Basic Tool, in the General group. A single level can be returned as it is
	class ToolGeneral : public FUCK::ITool
	{
	public:
		const char* Name() const override { return "$DEMO_Tool_General"_T; }
		const char* Group() const override { return "$DEMO_Group_General"_T; }
		void        Draw() override;
		// Every tool's RenderOverlay() runs each frame, open or not, so only this one draws the demo's overlays.
		// It's only called while the host is drawing. To keep an overlay up with the menu closed, draw it from an open window
		void        RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
		void        OnOpen() override { DemoState::GetSingleton()->OnOpen(); }
		void        OnClose() override { DemoState::GetSingleton()->OnClose(); }
		bool        OnAsyncInput(const void* e) override { return DemoState::GetSingleton()->OnAsyncInput(e); }
	};

	// Visuals Tool, in the General group
	class ToolVisuals : public FUCK::ITool
	{
	public:
		const char* Name() const override { return "$DEMO_Tool_Visuals"_T; }
		const char* Group() const override { return "$DEMO_Group_General"_T; }
		void        Draw() override;
	};

	// System Tool, in the General group
	class ToolSystem : public FUCK::ITool
	{
	public:
		const char* Name() const override { return "$DEMO_Tool_System"_T; }
		const char* Group() const override { return "$DEMO_Group_General"_T; }
		void        Draw() override;
	};

	DemoConfig         _cfg;
	const DemoConfig   _def;
	const HotkeyConfig _defHotkey;

	FUCK::ManagedHotkey _toggleHotkey;

private:
	// Draw Implementations
	void DrawBasicWidgetsTab();
	void DrawAdvancedWidgetsTab();
	void DrawLayoutStyleTab();
	void DrawRenderingTab();
	void DrawOverlays();
	void DrawInputIOTab();
	void DrawTablesTab();
	void DrawGameControlTab();
	void DrawIconsTab();
	void DrawVers2Page();
	void DrawVers3Page();
	void DrawVers4Page();
	void DrawVers5Page();
	void DrawVers6Page();

	// Feature Demos
	std::string _multilineBuffer = "Line 1\nLine 2\nLine 3\nLine 4\nLine 5";
	bool        _disableHotkey   = false;
	int         _iconBtnClicks   = 0;
	bool        _useChildWindow  = false;
	bool        _showSpinner     = false;
	bool        _renderSuspended = false;

	FUCK::Image _loadedImage;
	char        _ImagePath[256] = { 0 };

	const std::string _defaultDemoImage = "test2.png";

	// Input Capture Demo
	int           _keyTestIndex   = 1;
	bool          _inputCaptured  = false;
	std::uint32_t _lastPressedKey = 0;

	// Game State
	bool _vanityBlocked = false;
	bool _hardPaused    = false;
	bool _softPaused    = false;
	bool _cursorForced  = false;

	// Form Inspector
	struct FormRow
	{
		std::uint32_t id;
		std::string   name;
		std::string   editorID;
		std::string   typeStr;
		float         weight;
		std::int32_t  value;
	};
	std::vector<FormRow> _formRows;
	std::uint32_t        _comboSelectedFormID = 0;

	// Utilities
	char _pathBuffer[256]      = "Data/My Bad\\Path//File.dds";
	char _sanitizedBuffer[256] = { 0 };

	std::uint32_t            _selectedFormID = 0;
	int                      _stepperValue   = 0;
	std::vector<std::string> _stepperOptions;

	DemoOverlay   _overlay;
	SimpleOverlay _secondOverlay;
	HudWidget     _hudWidget;

	// Tool Instances
	ToolGeneral _toolGeneral;
	ToolVisuals _toolVisuals;
	ToolSystem  _toolSystem;

	// The API Updates pages need only a name, a group and a page to draw, so each is a FUCK::SimpleTool rather than a
	// class of its own. Their groups are two levels down, General > API Updates > Version 2/3, joined from translated
	// names with "::". Declared before the tools, so they're built first
	const std::string _groupV2to3 = std::format("{}::{}::{}", "$DEMO_Group_General"_T, "$DEMO_Group_APIUpdates"_T, "$DEMO_Group_APIv2to3"_T);
	const std::string _groupV4to6 = std::format("{}::{}::{}", "$DEMO_Group_General"_T, "$DEMO_Group_APIUpdates"_T, "$DEMO_Group_APIv4to6"_T);

	FUCK::SimpleTool _toolAPIv2{ "$DEMO_Tool_APIv2"_T, _groupV2to3, [this] { DrawVers2Page(); } };
	FUCK::SimpleTool _toolAPIv3{ "$DEMO_Tool_APIv3"_T, _groupV2to3, [this] { DrawVers3Page(); } };
	FUCK::SimpleTool _toolAPIv4{ "$DEMO_Tool_APIv4"_T, _groupV4to6, [this] { DrawVers4Page(); } };
	FUCK::SimpleTool _toolAPIv5{ "$DEMO_Tool_APIv5"_T, _groupV4to6, [this] { DrawVers5Page(); } };
	FUCK::SimpleTool _toolAPIv6{ "$DEMO_Tool_APIv6"_T, _groupV4to6, [this] { DrawVers6Page(); } };
};
