#pragma once
#include "FUCK_API.h"

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

		// Overlays
		bool reqBlur         = false;
		bool reqHideHUD      = false;
		bool reqPauseHard    = false;
		bool reqPauseSoft    = false;
		bool reqCloseOnEsc   = false;
		bool reqCloseOnMenu  = false;
		bool reqPassInput    = false;
		bool reqBlockVanity  = false;
		bool reqNoBackground = false;
		bool reqNoDecoration = false;
		bool reqExtendBorder = false;

		bool secondPassInput = false;
		bool hudKeepOpen     = false;

		float hudScale = 1.0f;

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

	// --- Sub-Tools ---
	// 1. Basic Tool (Root Level)
	class ToolGeneral : public FUCK::ITool
	{
	public:
		const char* Name() const override { return "$DEMO_Tool_General"_T; }
		const char* Group() const override { return nullptr; }  // Top level
		void        Draw() override;
		void        RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
		void        OnOpen() override { DemoState::GetSingleton()->OnOpen(); }
		void        OnClose() override { DemoState::GetSingleton()->OnClose(); }
		bool        OnAsyncInput(const void* e) override { return DemoState::GetSingleton()->OnAsyncInput(e); }
	};

	// 2. Visuals Tool (Grouped)
	class ToolVisuals : public FUCK::ITool
	{
	public:
		const char* Name() const override { return "$DEMO_Tool_Visuals"_T; }
		const char* Group() const override { return "$DEMO_Group_Advanced"_T; }  // Grouped
		void        Draw() override;
		void        RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
	};

	// 3. System Tool (Grouped)
	class ToolSystem : public FUCK::ITool
	{
	public:
		const char* Name() const override { return "$DEMO_Tool_System"_T; }
		const char* Group() const override { return "$DEMO_Group_Advanced"_T; }  // Grouped
		void        Draw() override;
		void        RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
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

	// Feature Demos
	int  _iconBtnClicks   = 0;
	bool _useChildWindow  = false;
	bool _showSpinner     = false;
	bool _renderSuspended = false;

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
};
