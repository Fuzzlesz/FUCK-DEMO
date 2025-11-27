#pragma once
#include "FUCK_API.h"
#include "System/Settings.h"

// ==========================================
// Secondary Simple Overlay
// ==========================================
class SimpleOverlay : public IWindow
{
public:
	const char*	Title() const override { return TRANSLATE("$DEMO_SecondOverlayTitle"); }
	void		Draw() override;
	bool		IsOpen() const override { return _isOpen; }
	void		SetOpen(bool a_open) override { _isOpen = a_open; }

	WindowFlags GetFlags() const override
	{
		WindowFlags f = WindowFlags::kNoDecoration;
		if (_passInput)
			f = f | WindowFlags::kPassInputToGame;
		return f;
	}

	ImVec2 GetDefaultPos() const override { return _pos; }
	ImVec2 GetDefaultSize() const override { return _size; }

	void UpdateState(const ImVec2& currentPos, const ImVec2& currentSize) override
	{
		_pos = currentPos;
		_size = currentSize;
	}

	bool _isOpen = false;
	bool _passInput = false;
	ImVec2 _pos{ 1670.0f, 635.0f };
	ImVec2 _size{ 400.0f, 300.0f };

	IWindow* _parentWindow = nullptr;
};

// ==========================================
// Demo Overlay Window (Main)
// ==========================================
class DemoOverlay : public IWindow
{
public:
	// --- IWindow Interface Implementation ---
	const char*	Title() const override { return TRANSLATE("$DEMO_OverlayTitleHint"); }
	void		Draw() override;
	bool		IsOpen() const override { return _isOpen; }
	void		SetOpen(bool a_open) override { _isOpen = a_open; }
	WindowFlags	GetFlags() const override;

	ImVec2 GetDefaultSize() const override { return _windowSize; }
	ImVec2 GetDefaultPos() const override { return _windowPos; }

	void UpdateState(const ImVec2& currentPos, const ImVec2& currentSize) override
	{
		_windowPos = currentPos;
		_windowSize = currentSize;
	}

	bool _isOpen = false;

	// Window Metrics
	ImVec2 _windowPos{ 2075.0f, 120.0f };
	ImVec2 _windowSize{ 430.0f, 855.0f };

	FUCK::ManagedHotkey _toggleHotkey{ 35, 0, 42, -1, -1, -1 };
	SimpleOverlay* _secondWindow = nullptr;

private:
	// Window Configuration Flags
	bool _reqBlur = false;
	bool _reqHideHUD = false;
	bool _reqPauseHard = false;
	bool _reqPauseSoft = false;
	bool _reqCloseOnEsc = false;
	bool _reqCloseOnMenu = false;
	bool _reqPassInput = false;
	bool _reqBlockVanity = false;
	bool _reqNoBackground = false;
	bool _reqNoDecoration = false;
	bool _reqExtendBorder = false;

	friend class DemoTool;
};

// ==========================================
// Demo Sidebar Tool
// ==========================================
class DemoTool : public ITool, public REX::Singleton<DemoTool>
{
public:
	DemoTool();

	// --- ITool Interface Implementation ---
	const char*		Name() const override { return TRANSLATE("$DEMO_ToolName"); }
	void			OnOpen() override;
	void			OnClose() override;
	void			Draw() override;
	bool			OnAsyncInput(const void* inputEvent) override;
	virtual bool	ShowInSidebar() const { return true; }


	DemoOverlay* GetOverlay() { return &_overlay; }

	void SaveSettings();

private:
	void LoadSettings();

	void DrawBasicWidgetsTab();
	void DrawAdvancedWidgetsTab();
	void DrawLayoutStyleTab();
	void DrawRenderingTab();
	void DrawInputIOTab();
	void DrawTablesTab();
	void DrawGameControlTab();
	void DrawIconsTab();

	// --- State Variables ---

	// Standard Mutex Group
	bool _chkNear = false;
	bool _chkMutexA = true;
	bool _chkMutexB = false;

	// Independent Group
	bool _chkFarA = false;
	bool _chkFarB = true;

	bool _toggleState = false;
	float _sliderVal = 50.0f;
	int _intVal = 5;
	float _dragFloat = 0.5f;
	int _dragInt = 10;
	char _inputBuffer[256] = { 0 };

	// Feature Demos
	bool _useChildWindow = false;
	void* _loadedImage = nullptr;
	bool _showSpinner = false;
	bool _renderSuspended = false;
	char _ImagePath[256] = "Data/Interface/test.png";
	int _iconBtnClicks = 0;

	// Overlay Demo
	bool _showOverlay = false;
	int _overlayType = 0;  // 0=Grid, 1=Columns, 2=Spiral, 3=X, 4=Diag
	float _overlayThickness = 2.0f;
	float _overlayColor[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
	int _gridRows = 10;
	int _gridCols = 10;
	int _spiralAnchor = 0;  // 0=BR, 1=BL, 2=TL, 3=TR
	float _spiralRot = 0.0f;
	float _spiralScale = 1.0f;
	float _spiralTurns = 6.0f;

	// Input Capture Demo
	int _keyTestIndex = 1;
	bool _inputCaptured = false;
	std::uint32_t _lastPressedKey = 0;

	// Game State
	bool _vanityBlocked = false;
	bool _hardPaused = false;
	bool _softPaused = false;
	bool _cursorForced = false;

	// Form Inspector
	struct FormRow
	{
		std::uint32_t id;
		std::string name;
		std::string editorID;
		std::string typeStr;
		float weight;
		std::int32_t value;
	};
	std::vector<FormRow> _formRows;
	std::uint32_t _comboSelectedFormID = 0;

	// Utilities
	char _pathBuffer[256] = "Data/My Bad\\Path//File.dds";
	char _sanitizedBuffer[256] = { 0 };

	std::uint32_t _selectedFormID = 0;
	int _stepperValue = 0;
	std::vector<std::string> _stepperOptions;

	DemoOverlay _overlay;
	SimpleOverlay _secondOverlay;
};
